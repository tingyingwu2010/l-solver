//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_COLUMNITERATOR_H
#define LSOLVERPROJECT_COLUMNITERATOR_H

#include "Column.h"

namespace L {
    class ColumnIterator;
}

class L::ColumnIterator {
public:
    virtual Column get_next_column() = 0;
    virtual bool is_done() = 0;
};


#endif //LSOLVERPROJECT_COLUMNITERATOR_H
