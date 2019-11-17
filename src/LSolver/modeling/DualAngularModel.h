//
// Created by hlefebvr on 17/11/19.
//

#ifndef LSOLVERPROJECT_DUALANGULARMODEL_H
#define LSOLVERPROJECT_DUALANGULARMODEL_H

#include "../application/enums.h"
#include "Model.h"

namespace L {
    class DualAngularModel;
    class LinkingConstraint;

    std::ostream& operator<<(std::ostream& os, const LinkingConstraint& model);
    std::ostream& operator<<(std::ostream& os, const DualAngularModel& model);
}

class L::LinkingConstraint {
    Constraint _src_constraint;
    std::map<std::string, Expression> _splitted_expression;
public:
    LinkingConstraint(const Constraint& ctr, std::map<std::string, Expression>  block_splitted_expression);
    friend std::ostream& operator<<(std::ostream& os, const LinkingConstraint& model);
};

class L::DualAngularModel {
    Model& _src_model;
    std::map<std::string, Model*> _blocks;
    std::map<std::string, LinkingConstraint*> _linking_constraints;
public:
    DualAngularModel(Model& model, const std::map<std::string, VariableIndicator>& indicators);
    friend std::ostream& operator<<(std::ostream& os, const DualAngularModel& model);
};


#endif //LSOLVERPROJECT_DUALANGULARMODEL_H
