//
// Created by hlefebvr on 04/11/19.
//

#include "Exception.h"

ED::Exception::Exception(const std::string& message) : _message(message) {}

const std::string &ED::Exception::message() const {
    return _message;
}

const char *ED::Exception::what() const throw() {
    return _message.c_str();
}
