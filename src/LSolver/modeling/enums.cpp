//
// Created by hlefebvr on 20/11/19.
//

#include "enums.h"
#include "../utils/Exception.h"

std::ostream& L::operator<<(std::ostream& os, ConstraintType constraint) {
    switch (constraint) {
        case LessOrEqualTo: return (os << std::string("<="));
        case GreaterOrEqualTo: return (os << std::string(">="));
        case EqualTo: return (os << std::string("=="));
        default: throw Exception("Unknown constraints type: " + std::to_string(constraint));
    }
}

std::ostream& L::operator<<(std::ostream& os, ObjectiveType type) {
    switch (type) {
        case Minimize: return (os << std::string("minimize"));
        case Maximize: return (os << std::string("maximize"));
        default: throw Exception("Unknown objective type: " + std::to_string(type));
    }
}

std::ostream& L::operator<<(std::ostream& os, ObjectiveStatus obj) {
    switch (obj) {

        case Unsolved: return (os << std::string("Unsolved"));
        case Optimal: return (os << std::string("Optimal"));
        case Feasible: return (os << std::string("Feasible"));
        case Infeasible: return (os << std::string("Infeasible"));
        case Unbounded: return (os << std::string("Unbounded"));
        case Error:  return (os << std::string("Error"));
        default: throw Exception("Unknown objective status: " + std::to_string(obj));
    }
}

std::ostream &L::operator<<(std::ostream &os, VariableType type) {
    switch (type) {
        case Positive: return (os << "positive");
        case Negative: return (os << "negative");
        case Free: return (os << "free");
        case Binary: return (os << "binary");
        case Integer: return (os << "integer");
        default: throw Exception("Unknown variable type: " + std::to_string(type));
    }
}
