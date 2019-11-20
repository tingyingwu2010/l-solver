//
// Created by hlefebvr on 17/11/19.
//

#ifndef LSOLVERPROJECT_DUALANGULARMODEL_H
#define LSOLVERPROJECT_DUALANGULARMODEL_H

#include "LSolver/application/enums.h"
#include "LSolver/modeling/models/Model.h"
#include "LSolver/environment/Environment.h"

namespace L {
    class DualAngularModel;
    class LinkingConstraint;

    std::ostream& operator<<(std::ostream& os, const DualAngularModel& model);
}

class L::LinkingConstraint : public Constraint {
    std::map<std::string, Expression> _splitted_expression;
public:
    LinkingConstraint(const Constraint& ctr, std::map<std::string, Expression> block_splitted_expression);
    Expression& block(const std::string& block_name);
};

class L::DualAngularModel {
    Model& _src_model;
    Environment _dw_env;
    std::map<std::string, Model*> _blocks;
    std::map<std::string, LinkingConstraint*> _linking_constraints;
    std::map<std::string, VariableIndicator> _indicators;

    void dispatch_in_blocks(Variable &row, const std::map<std::string, VariableIndicator> &indicators);
    void dispatch_in_blocks(Constraint &row, const std::map<std::string, VariableIndicator> &indicators);
    void dispatch_in_blocks(const Objective &row, const std::map<std::string, VariableIndicator> &indicators);
public:
    typedef MapIterator<std::string, Model, Model&> BlockIterator;
    typedef MapIterator<std::string, LinkingConstraint, LinkingConstraint&> LinkingConstraintIterator;
    explicit DualAngularModel(Model& model);
    DualAngularModel(Model& model, std::map<std::string, VariableIndicator>  indicators);
    Model& block(const std::string& name);

    BlockIterator blocks();
    LinkingConstraintIterator linking_constraints();

    void decompose();
    void add_block_indicator(const std::string& name, const VariableIndicator& indicator);

    Model& source_model() { return _src_model; }

    friend std::ostream& operator<<(std::ostream& os, const DualAngularModel& model);
};


#endif //LSOLVERPROJECT_DUALANGULARMODEL_H
