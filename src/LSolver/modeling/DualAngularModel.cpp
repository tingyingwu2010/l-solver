//
// Created by hlefebvr on 17/11/19.
//

#include "DualAngularModel.h"
#include "Objective.h"

#include <utility>
#include <iostream>

L::DualAngularModel::DualAngularModel(L::Model &model) : _src_model(model) {}

L::DualAngularModel::DualAngularModel(L::Model &model, std::map<std::string, VariableIndicator> indicators)
    : _src_model(model), _indicators(std::move(indicators))
{
    decompose();
}

void L::DualAngularModel::dispatch_in_blocks(L::Variable &variable, const std::map<std::string, VariableIndicator> &indicators) {
    auto add_variable_to_block = [this](const Variable& variable, const std::string& block_name){
        auto found = _blocks.find(block_name);
        Model* model;
        if (found == _blocks.end()) {
            model = new Model(block_name);
            _blocks.insert({block_name, model});
        } else {
            model = found->second;
        }
        model->add(variable);
    };

    bool added_to_a_block = false;
    for (auto& m : indicators) {
        if (m.second(variable)) {
            add_variable_to_block(variable, m.first);
            added_to_a_block = true;
            break;
        }
    }
    if (!added_to_a_block) {
        add_variable_to_block(variable, "_default");
    }
}

void L::DualAngularModel::dispatch_in_blocks(const Objective &row, const std::map<std::string, VariableIndicator> &indicators) {
    Objective copy = row;
    std::map<std::string, Expression> splitted_by_block = copy.expression().split_by_variable(indicators);

    if (splitted_by_block.empty()) return;

    for (auto& m : splitted_by_block) {

        auto found = _blocks.find(m.first);
        if (found == _blocks.end()) {
            throw Exception("An error occurred while decomposing model: " + _src_model.user_defined_name() + ", block: " + m.first);
        } else {
            Objective added_obj = Objective(_dw_env, "obj_" + m.first);
            added_obj.type(_src_model.objective().type());
            added_obj.expression() = m.second;
            found->second->add(added_obj);
        }
    }

}

void L::DualAngularModel::dispatch_in_blocks(Constraint &row, const std::map<std::string, VariableIndicator> &indicators) {

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

    std::map<std::string, Expression> splitted_by_block = row.expression().split_by_variable(indicators);

    if (splitted_by_block.empty()) return;

    bool is_block_defining = false;
    std::string block_name;
    if (splitted_by_block.size() == 1) { // block defining
        is_block_defining = true;
        block_name = splitted_by_block.begin()->first;
    }

    if (splitted_by_block.size() == 2) { // can be block defining
        auto _default = splitted_by_block.end();
        for (auto it = splitted_by_block.begin(), end = splitted_by_block.end() ; it != end ; ++it ) {
            if (it->first == "_default") _default = it;
            else block_name = it->first;
        }

        if (_default != splitted_by_block.end() && is_numerical(_default->second)) { // block defining
            is_block_defining = true;
        }
    }

    if (is_block_defining) {
        auto found = _blocks.find(block_name);
        if (found == _blocks.end()) throw Exception("An error occured while decomposing model:" + _src_model.user_defined_name() + ", on " + block_name);
        found->second->add(row);
    } else {
        _linking_constraints.insert({ row.user_defined_name(), new LinkingConstraint(row, splitted_by_block) });
    }

}

std::ostream &L::operator<<(std::ostream &os, const L::DualAngularModel &model) {
    os << "Dual Angular Model: " << std::endl;
    for (auto& m : model._linking_constraints) os << "Linking constraint \"" << m.first << "\": " << *m.second << std::endl;
    for (auto& m : model._blocks) os << "Defined block \" " + m.first + " \": " << std::endl << *m.second << std::endl;
    return os;
}

L::Model &L::DualAngularModel::block(const std::string &name) {
    if (_blocks.empty()) throw Exception("Dual angular model has not been decomposed.");
    auto found = _blocks.find(name);
    if (found == _blocks.end()) throw Exception("The requested block could not be found: " + name);
    return *found->second;
}

void L::DualAngularModel::decompose() {
    // dispatch variables
    for (Variable variable : _src_model.variables()) dispatch_in_blocks(variable, _indicators);

    // if the "_default" block has no variable, we must create an empty model
    // so that future call can allways rely on "_default" to be available
    Model* default_model = nullptr;
    try {
        default_model = &block("_default");
    } catch (Exception&) {
        default_model = new Model("_default");
        _blocks.insert({ "_default", default_model });
    }

    // dispatch constraints
    for (Constraint ctr : _src_model.constraints()) dispatch_in_blocks(ctr, _indicators);

    // dispatch objective
    dispatch_in_blocks(_src_model.objective(), _indicators);

    // if the "_default" model does not have a defined objective it shall be put to "0"
    try {
        default_model->objective();
    } catch (Exception&) {
        default_model->add(Objective(_dw_env, "obj__default"));
    }
}

L::DualAngularModel::BlockIterator L::DualAngularModel::blocks() {
    if (_blocks.empty()) throw Exception("Dual angular model has not been decomposed.");
    return BlockIterator(_blocks);
}

L::DualAngularModel::LinkingConstraintIterator L::DualAngularModel::linking_constraints() {
    if (_blocks.empty()) throw Exception("Dual angular model has not been decomposed.");
    return LinkingConstraintIterator(_linking_constraints);
}

void L::DualAngularModel::add_block_indicator(const std::string& name, const VariableIndicator& indicator) {
    _indicators.insert({ name, indicator });
}

L::LinkingConstraint::LinkingConstraint(const L::Constraint &ctr, std::map<std::string, Expression> block_splitted_expression)
    : Constraint(ctr), _splitted_expression(std::move(block_splitted_expression)) {}


L::Expression &L::LinkingConstraint::block(const std::string &block_name) {
    auto found = _splitted_expression.find(block_name);
    if (found == _splitted_expression.end()) throw NotFound();
    return found->second;
}

