//
// Created by hlefebvr on 04/11/19.
//

#include "Variable.h"
#include "LSolver/environment/Environment.h"
#include <iostream>
#include <utility>
#include <LSolver/application/Application.h>

L::Variable::Variable(CoreVariable& core) : _core(core) {}
L::Variable::Variable(Environment& env, const std::string& name) : Variable(env.variable(name)) {}

L::ConstVariable::ConstVariable(const CoreVariable &core) : _core(core) {}

