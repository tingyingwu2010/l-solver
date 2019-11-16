//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_EXCEPTION_H
#define ED_SOLVER_EXCEPTION_H

#include <string>

namespace L {
    class Exception;
}

class L::Exception : public std::exception {
    std::string _message;
public:
    explicit Exception(const std::string& message);
    const std::string& message() const;
    const char* what() const throw() override;
};



#endif //ED_SOLVER_EXCEPTION_H
