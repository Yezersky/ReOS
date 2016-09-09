//=======================================================================
// Copyright Baptiste Wicht 2013-2016.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://www.opensource.org/licenses/MIT)
//=======================================================================

#include <array.hpp>

#include "terminal.hpp"
#include "drivers/keyboard.hpp"
#include "console.hpp"
#include "assert.hpp"
#include "logging.hpp"
#include "scheduler.hpp"

namespace {

constexpr const size_t MAX_TERMINALS = 2;
size_t active_terminal;

std::array<stdio::virtual_terminal, MAX_TERMINALS> terminals;

void input_thread(void* data){
    auto& terminal = *reinterpret_cast<stdio::virtual_terminal*>(data);

    auto pid = scheduler::get_pid();

    logging::logf(logging::log_level::TRACE, "stdio: Input Thread for terminal %u started (pid:%u)\n", terminal.id, pid);

    bool shift = false;

    while(true){
        // Wait for some input
        scheduler::block_process(pid);

        // Handle keyboard input
        while(!terminal.keyboard_buffer.empty()){
            auto key = terminal.keyboard_buffer.pop();

            if(terminal.canonical){
                //Key released
                if(key & 0x80){
                    key &= ~(0x80);
                    if(key == keyboard::KEY_LEFT_SHIFT || key == keyboard::KEY_RIGHT_SHIFT){
                        shift = false;
                    }
                }
                //Key pressed
                else {
                    if(key == keyboard::KEY_LEFT_SHIFT || key == keyboard::KEY_RIGHT_SHIFT){
                        shift = true;
                    } else if(key == keyboard::KEY_BACKSPACE){
                        if(!terminal.input_buffer.empty()){
                            terminal.input_buffer.pop_last();
                            terminal.print('\b');
                        }
                    } else {
                        auto qwertz_key =
                            shift
                            ? keyboard::shift_key_to_ascii(key)
                            : keyboard::key_to_ascii(key);

                        if(qwertz_key){
                            terminal.input_buffer.push(qwertz_key);

                            terminal.print(qwertz_key);

                            if(qwertz_key == '\n'){
                                // Transfer current line to the canonical buffer
                                while(!terminal.input_buffer.empty()){
                                    terminal.canonical_buffer.push(terminal.input_buffer.pop());
                                }

                                terminal.input_queue.wake_up();
                            }
                        }
                    }
                }
            } else {
                // The complete processing of the key will be done by the
                // userspace program
                auto code = keyboard::raw_key_to_keycode(key);
                terminal.raw_buffer.push(static_cast<size_t>(code));

                terminal.input_queue.wake_up();

                thor_assert(!terminal.raw_buffer.full(), "raw buffer is full!");
            }
        }

        // Handle mouse input
        while(!terminal.mouse_buffer.empty()){
            auto key = terminal.mouse_buffer.pop();

            if(!terminal.canonical && terminal.mouse){
                terminal.raw_buffer.push(key);

                terminal.input_queue.wake_up();

                thor_assert(!terminal.raw_buffer.full(), "raw buffer is full!");
            }
        }
    }
}

} //end of anonymous namespace

void stdio::virtual_terminal::print(char key){
    //TODO If it is not the active terminal, buffer it
    k_print(key);
}

void stdio::virtual_terminal::send_input(char key){
    if(!input_thread_pid){
        return;
    }

    // Simply give the input to the input thread
    keyboard_buffer.push(key);
    thor_assert(!keyboard_buffer.full(), "keyboard buffer is full!");

    scheduler::unblock_process_hint(input_thread_pid);
}

void stdio::virtual_terminal::send_mouse_input(std::keycode key){
    if(!input_thread_pid){
        return;
    }

    // Simply give the input to the input thread
    mouse_buffer.push(size_t(key));
    thor_assert(!mouse_buffer.full(), "mouse buffer is full!");

    scheduler::unblock_process_hint(input_thread_pid);
}

size_t stdio::virtual_terminal::read_input_can(char* buffer, size_t max){
    size_t read = 0;

    while(true){
        while(!canonical_buffer.empty()){
            auto c = canonical_buffer.pop();

            buffer[read++] = c;

            if(read >= max || c == '\n'){
                return read;
            }
        }

        input_queue.sleep();
    }
}

// TODO In case of max < read, the timeout is not respected
size_t stdio::virtual_terminal::read_input_can(char* buffer, size_t max, size_t ms){
    size_t read = 0;

    while(true){
        while(!canonical_buffer.empty()){
            auto c = canonical_buffer.pop();

            buffer[read++] = c;

            if(read >= max || c == '\n'){
                return read;
            }
        }

        if(!ms){
            return read;
        }

        if(!input_queue.sleep(ms)){
            return read;
        }
    }
}

size_t stdio::virtual_terminal::read_input_raw(){
    if(raw_buffer.empty()){
        input_queue.sleep();
    }

    return raw_buffer.pop();
}

size_t stdio::virtual_terminal::read_input_raw(size_t ms){
    if(raw_buffer.empty()){
        if(!ms){
            return static_cast<size_t>(std::keycode::TIMEOUT);
        }

        if(!input_queue.sleep(ms)){
            return static_cast<size_t>(std::keycode::TIMEOUT);
        }
    }

    thor_assert(!raw_buffer.empty(), "There is a problem with the sleep queue");

    return raw_buffer.pop();
}

void stdio::virtual_terminal::set_canonical(bool can){
    logging::logf(logging::log_level::TRACE, "Switched terminal %u canonical mode from %u to %u\n", id, uint64_t(canonical), uint64_t(can));

    canonical = can;
}

void stdio::virtual_terminal::set_mouse(bool m){
    logging::logf(logging::log_level::TRACE, "Switched terminal %u mouse handling mode from %u to %u\n", id, uint64_t(mouse), uint64_t(m));

    mouse = m;
}

void stdio::init_terminals(){
    size_t id = 0;

    for(auto& terminal : terminals){
        terminal.id = id++;
        terminal.active = false;
        terminal.canonical = true;
        terminal.mouse = false;
    }

    active_terminal = 0;
    terminals[active_terminal].active = true;
}

void stdio::finalize(){
    for(auto& terminal : terminals){
        auto* user_stack = new char[scheduler::user_stack_size];
        auto* kernel_stack = new char[scheduler::kernel_stack_size];

        auto& input_process = scheduler::create_kernel_task_args("tty_input", user_stack, kernel_stack, &input_thread, &terminal);

        input_process.ppid = 1;
        input_process.priority = scheduler::DEFAULT_PRIORITY;

        scheduler::queue_system_process(input_process.pid);

        terminal.input_thread_pid = input_process.pid;
    }
}

stdio::virtual_terminal& stdio::get_active_terminal(){
    return terminals[active_terminal];
}

stdio::virtual_terminal& stdio::get_terminal(size_t id){
    thor_assert(id < MAX_TERMINALS, "Out of bound tty");

    return terminals[id];
}
