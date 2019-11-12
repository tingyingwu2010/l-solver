//
// Created by hlefebvr on 04/11/19.
//

#include "Exception.h"

using namespace L;

Exception::Exception(const std::string& message) : _message(message) {}

const std::string &Exception::message() const {
    return _message;
}

const char *Exception::what() const throw() {
    return _message.c_str();
}
