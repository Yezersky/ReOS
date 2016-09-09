//=======================================================================
// Copyright Baptiste Wicht 2013-2016.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://www.opensource.org/licenses/MIT)
//=======================================================================

#ifndef USER_FILE_HPP
#define USER_FILE_HPP

#include <types.hpp>
#include <expected.hpp>
#include <string.hpp>

#include "tlib/stat_info.hpp"
#include "tlib/statfs_info.hpp"
#include "tlib/config.hpp"

ASSERT_ONLY_THOR_PROGRAM

namespace tlib {

std::expected<size_t> open(const char* file, size_t flags = 0);
int64_t mkdir(const char* file);
int64_t rm(const char* file);
std::expected<size_t> read(size_t fd, char* buffer, size_t max, size_t offset = 0);
std::expected<size_t> write(size_t fd, const char* buffer, size_t max, size_t offset = 0);
std::expected<size_t> clear(size_t fd, size_t max, size_t offset = 0);
std::expected<size_t> truncate(size_t fd, size_t size);
std::expected<size_t> entries(size_t fd, char* buffer, size_t max);
void close(size_t fd);
std::expected<stat_info> stat(size_t fd);
std::expected<statfs_info> statfs(const char* file);
std::expected<size_t> mounts(char* buffer, size_t max);
std::expected<void> mount(size_t type, size_t dev_fd, size_t mp_fd);

std::string current_working_directory();
void set_current_working_directory(const std::string& directory);

} // end of namespace tlib

#endif
