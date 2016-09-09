//=======================================================================
// Copyright Baptiste Wicht 2013-2016.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://www.opensource.org/licenses/MIT)
//=======================================================================

#include <tlib/system.hpp>
#include <tlib/errors.hpp>
#include <tlib/print.hpp>
#include <tlib/net.hpp>

namespace {

} // end of anonymous namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        tlib::print_line("usage: ping address_ip");
        return 1;
    }

    std::string ip(argv[1]);
    auto ip_parts = std::split(ip, '.');

    if (ip_parts.size() != 4) {
        tlib::print_line("Invalid address IP");
        return 1;
    }

    auto socket = tlib::socket_open(tlib::socket_domain::AF_INET, tlib::socket_type::RAW, tlib::socket_protocol::ICMP);

    if (!socket) {
        tlib::printf("ls: socket_open error: %s\n", std::error_message(socket.error()));
        return 1;
    }

    auto status = tlib::listen(*socket, true);
    if (!status) {
        tlib::printf("ping: listen error: %s\n", std::error_message(status.error()));
        return 1;
    }

    tlib::icmp::packet_descriptor desc;
    desc.payload_size = 0;
    desc.target_ip    = tlib::ip::make_address(std::atoui(ip_parts[0]), std::atoui(ip_parts[1]), std::atoui(ip_parts[2]), std::atoui(ip_parts[3]));
    desc.type         = tlib::icmp::type::ECHO_REQUEST;
    desc.code         = 0;

    static constexpr const size_t N = 4;
    static constexpr const size_t timeout_ms = 2000;

    for(size_t i = 0; i < N; ++i){
        auto packet = tlib::prepare_packet(*socket, &desc);

        if (!packet) {
            if(packet.error() == std::ERROR_SOCKET_TIMEOUT){
                tlib::printf("Unable to resolve MAC address for target IP\n");
                return 1;
            }

            tlib::printf("ping: prepare_packet error: %s\n", std::error_message(packet.error()));
            return 1;
        }

        auto* command_header = reinterpret_cast<tlib::icmp::echo_request_header*>(packet->payload + packet->index);

        command_header->identifier = 0x666;
        command_header->sequence   = 0x1 + i;

        status = tlib::finalize_packet(*socket, *packet);
        if (!status) {
            tlib::printf("ping: finalize_packet error: %s\n", std::error_message(status.error()));
            return 1;
        }

        auto before = tlib::ms_time();
        auto after = before;

        while(true){
            // Make sure we don't wait for more than the timeout
            if(after > before + timeout_ms){
                break;
            }

            auto remaining = timeout_ms - (after - before);

            bool handled = false;

            auto p = tlib::wait_for_packet(*socket, remaining);
            if (!p) {
                if(p.error() == std::ERROR_SOCKET_TIMEOUT){
                    tlib::printf("%s unreachable\n", ip.c_str());
                    handled = true;
                } else {
                    tlib::printf("ping: wait_for_packet error: %s\n", std::error_message(p.error()));
                    return 1;
                }
            } else {
                auto* icmp_header = reinterpret_cast<tlib::icmp::header*>(p->payload + p->index);

                auto command_type = static_cast<tlib::icmp::type>(icmp_header->type);

                if(command_type == tlib::icmp::type::ECHO_REPLY){
                    tlib::printf("Reply received from %s\n", ip.c_str());
                    handled = true;
                }

                // The rest of the packets are simply ignored

                tlib::release_packet(*p);
            }

            if(handled){
                break;
            }

            after = tlib::ms_time();
        }

        if(i < N - 1){
            tlib::sleep_ms(1000);
        }
    }

    status = tlib::listen(*socket, false);
    if (!status) {
        tlib::printf("ping: listen error: %s\n", std::error_message(status.error()));
        return 1;
    }

    tlib::socket_close(*socket);

    return 0;
}
