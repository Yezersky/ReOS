#include <file.hpp>
#include <system.hpp>
#include <errors.hpp>
#include <print.hpp>
#include <bmplib.hpp>

int main(int argc, char* argv[]){
    if(argc == 1){
        print_line("Usage: view file_path");
        exit(1);
    }

    auto fd = open(argv[1]);

    if(fd.valid()){
        auto info = stat(*fd);

        if(info.valid()){
            if(info->flags & STAT_FLAG_DIRECTORY){
                print_line("view: error: Is a directory");
            } else {
                auto size = info->size;
                // print(reinterpret_cast<uint64_t>(*fd)); print_line();
                display_bmp(*fd, size, argv[1]);
            }
        } else {
            printf("view: error: %s\n", std::error_message(info.error()));
        }

        close(*fd);
    } else {
        printf("view: error: %s\n", std::error_message(fd.error()));
    }

    exit(0);
}
