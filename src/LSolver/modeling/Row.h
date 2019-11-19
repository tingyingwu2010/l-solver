//
// Created by hlefebvr on 18/11/19.
//

#ifndef LSOLVERPROJECT_ROW_H
#define LSOLVERPROJECT_ROW_H

namespace L {
    class Row;
    class Expression;
}

class L::Row {
public:
    virtual const Expression& expression() const = 0;
    virtual Expression& expression() = 0;
};

#endif //LSOLVERPROJECT_ROW_H
