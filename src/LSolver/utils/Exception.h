//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_EXCEPTION_H
#define ED_SOLVER_EXCEPTION_H

#include <string>

namespace L {
    class Exception;
    class StopIteration;
    class NotFound;
}

class L::Exception : public std::exception {
    std::string _message;
public:
    explicit Exception(std::string  message);
    const std::string& message() const;
    const char* what() const throw() override;
};

class L::StopIteration : public std::exception {
public:
    const char* what() const throw() override;
};

class L::NotFound : public std::exception {
public:
    const char* what() const throw() override;
};



#endif //ED_SOLVER_EXCEPTION_H
