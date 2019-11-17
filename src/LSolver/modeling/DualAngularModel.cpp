//
// Created by hlefebvr on 17/11/19.
//

#include "DualAngularModel.h"

#include <utility>
#include <iostream>

L::DualAngularModel::DualAngularModel(L::Model &model, const std::map<std::string, VariableIndicator> &indicators)
    : _src_model(model)
{
    // TODO this can be removed as soon as expand() is written and simplifies the expressions !
    auto is_numerical = [](const Expression& expr){ // checks if an expression is an expression
        bool is_numerical = true;
        expr.depth_first_traversal<PostOrder>([&is_numerical](const Expression& node){
            if (node.type() == Var) {
                is_numerical = false;
                throw StopIteration();
            }
        });
        return is_numerical;
    };

    for (Constraint ctr : _src_model.constraints()) {
        std::map<std::string, Expression> splitted_by_block = ctr.expression().split_by_variable(indicators);

        if (splitted_by_block.empty()) continue;

        bool is_linking_constraint = true;
        if (splitted_by_block.size() <= 2) {

            std::string block_name;
            bool others_is_not_numerical = false;
            for (auto& m : splitted_by_block) {
                if (m.first == "others" && is_numerical(m.second)) continue;
                if (m.first == "others") others_is_not_numerical = true;
                else block_name = m.first;
            }

            is_linking_constraint = others_is_not_numerical || splitted_by_block.size() == 1;
            if ( !is_linking_constraint ) {
                auto found = _blocks.find(block_name);
                Model* block;
                if (found != _blocks.end()) {
                    block = found->second;
                } else {
                    block = new Model(block_name);
                    _blocks.insert({block_name, block});
                }
                block->add(ctr);
            }
        }

        if (is_linking_constraint) {
            _linking_constraints.insert({ ctr.user_defined_name(), new LinkingConstraint(ctr, splitted_by_block) });
        }

    }

    // adding variables to sub-problems
    for (auto variable : _src_model.variables()) {
        for (auto& m : indicators) {
            if (m.second(variable)) {
                auto found = _blocks.find(m.first);
                if (found != _blocks.end()) found->second->add(variable);
                break;
            }
        }
    }
}

std::ostream &L::operator<<(std::ostream &os, const L::DualAngularModel &model) {
    os << "Dual Angular Model: " << std::endl;
    for (auto& m : model._blocks) os << "Defined block \" " + m.first + " \": " << std::endl << *m.second << std::endl;
    for (auto& m : model._linking_constraints) os << "Linking constraint \"" << m.first << "\": " << *m.second << std::endl;
    return os;
}

L::LinkingConstraint::LinkingConstraint(const L::Constraint &ctr, std::map<std::string, Expression> block_splitted_expression)
    : _src_constraint(ctr), _splitted_expression(std::move(block_splitted_expression)) {}

std::ostream &L::operator<<(std::ostream &os, const L::LinkingConstraint &model) {
    return (os << model._src_constraint);
}
