//=======================================================================
// Copyright Baptiste Wicht 2013-2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <file.hpp>
#include <system.hpp>
#include <errors.hpp>
#include <print.hpp>
#include <flags.hpp>

int main(int argc, char* argv[]){
    if(argc == 1){
        print_line("Usage: writer file_path");
        exit(1);
    }

    print_line("The following text will be writed in file:");
    char buffer[64];
    auto c = read_input(buffer, 15);
    buffer[c] = '\0';

    auto fd = open(argv[1], std::OPEN_CREATE);

    if(fd.valid()){
        auto truncate_result = truncate(*fd, c);

        if(truncate_result.valid()){
            // auto s = "0123456789AB";

            auto write_result = write(*fd, buffer, c, 0);

            if(write_result.valid()){
                //TODO
            } else {
                printf("writer: error: %s\n", std::error_message(write_result.error()));
            }
        } else {
            printf("writer: error: %s\n", std::error_message(truncate_result.error()));
        }

        close(*fd);
    } else {
        printf("writer: error: %s\n", std::error_message(fd.error()));
    }

    exit(0);
}
