//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_STATIC_H
#define ED_SOLVER_STATIC_H

#include <string>
#include <array>

namespace ED {

    static std::string concat_string_array(const std::string& str, const std::array<unsigned int, 4>& arr) {
        std::string output = str + "(";
        for (auto& i : arr) output += std::to_string(i) + ",";
        output[output.size() - 1] = ')';
        return output;
    }

}

#endif //ED_SOLVER_STATIC_H
