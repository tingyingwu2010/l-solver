//
// Created by hlefebvr on 04/11/19.
//

#include "Exception.h"

#include <utility>

using namespace L;

Exception::Exception(std::string  message) : std::exception(), _message(std::move(message)) {}

const std::string &Exception::message() const {
    return _message;
}

const char *Exception::what() const throw() {
    return _message.c_str();
}

const char *StopIteration::what() const throw() {
    return "Stopped after throwing StopIteration exception";
}

const char *NotFound::what() const throw() {
    return "Requested component not found";
}
