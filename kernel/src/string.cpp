#include "string.hpp"
#include "utils.hpp"
#include "console.hpp"

string::string(){
    _size = 0;
    _capacity = 1;
    _data = new char[_capacity];
    _data[0] = '\0';
}

string::string(const char* s){
    _size = str_len(s);
    _capacity = _size + 1;
    _data = new char[_capacity];
    memcopy(_data, s, _capacity);
}

string::string(size_t capacity){
    _size = 0;
    _capacity = capacity;
    _data = new char[_capacity];
}

string::string(const string& rhs){
    *this = rhs;
}

//TODO Does not seem to work
string& string::operator=(const string& rhs){
    if(this != &rhs){
        if(_capacity < rhs._capacity || !_data){
            if(_data){
                delete[] _data;
            }

            _capacity = rhs._capacity;
            _data = new char[_capacity];
        }

        _size = rhs._size;
        memcopy(_data, rhs._data, _size + 1);
    }

    return *this;
}

string::string(string&& rhs){
    *this = rhs;
}

string& string::operator=(string&& rhs){
    if(_data){
        delete[] _data;
    }

    _size = rhs._size;
    _capacity = rhs._capacity;
    _data = rhs._data;

    rhs._size = 0;
    rhs._capacity = 0;
    rhs._data = nullptr;

    return *this;
}

string::~string(){
    if(_data){
        delete[] _data;
    }
}

void string::clear(){
    _size = 0;
    _data[0] = '\0';
}

size_t string::size() const {
    return _size;
}

size_t string::capacity() const {
    return _capacity;
}

bool string::empty() const {
    return !_size;
}

const char* string::c_str() const {
    return _data;
}

char& string::operator[](size_t i){
    return _data[i];
}

const char& string::operator[](size_t i) const {
    return _data[i];
}

string string::operator+(char c) const {
    string copy = *this;

    copy += c;

    return move(copy);
}

string& string::operator+=(char c){
    if(!_data || _capacity <= _size + 1){
        if(_data){
            delete[] _data;
        }

        _capacity = _size + 3;
        _data = new char[_capacity];
    }

    _data[_size] = c;
    _data[++_size] = '\0';

    return *this;
}

bool string::operator==(const char* s) const {
    if(size() != str_len(s)){
        return false;
    }

    for(size_t i = 0; i < size(); ++i){
        if(_data[i] != s[i]){
            return false;
        }
    }

    return true;
}

string::iterator string::begin(){
    return iterator(&_data[0]);
}

string::iterator string::end(){
    return iterator(&_data[_size]);
}

string::const_iterator string::begin() const {
    return const_iterator(&_data[0]);
}

string::const_iterator string::end() const {
    return const_iterator(&_data[_size]);
}