//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_EXCEPTION_H
#define ED_SOLVER_EXCEPTION_H

#include <string>

namespace ED {

    class Exception : public std::exception {
        std::string _message;
    public:
        explicit Exception(const std::string& message);
        const std::string& message() const;
    };

}


#endif //ED_SOLVER_EXCEPTION_H
