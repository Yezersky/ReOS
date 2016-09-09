//=======================================================================
// Copyright Baptiste Wicht 2013-2016.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://www.opensource.org/licenses/MIT)
//=======================================================================

#include <cstdio>
#include <cstring>

#include <type_traits.hpp>
#include <string.hpp>

#include "test.hpp"

namespace {

void test_is_same(){
    check(std::is_same<char, char>::value, "Invalid is_same");
    check(std::is_same<double, double>::value, "Invalid is_same");
    check(!std::is_same<const double, double>::value, "Invalid is_same");
    check(!std::is_same<char, int>::value, "Invalid is_same");
}

void test_iterator_traits(){
    char* a = nullptr;

    check(std::is_same<std::iterator_traits<decltype(a)>::value_type, char>::value, "Invalid value_type in iterator_traits");
    check(std::is_same<std::iterator_traits<const double*>::value_type, const double>::value, "Invalid value_type in iterator_traits");
}

void test_has_trivial_assign(){
    check(std::has_trivial_assign<char>::value, "Invalid has_trivial_assign");
    check(std::has_trivial_assign<double>::value, "Invalid has_trivial_assign");
    check(std::has_trivial_assign<size_t>::value, "Invalid has_trivial_assign");
    check(!std::has_trivial_assign<std::string>::value, "Invalid has_trivial_assign");
}

} //end of anonymous namespace

void traits_tests(){
    test_is_same();
    test_iterator_traits();
    test_has_trivial_assign();
}
