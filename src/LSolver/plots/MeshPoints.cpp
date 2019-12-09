//
// Created by hlefebvr on 09/12/19.
//

#include "MeshPoints.h"

void L::MeshPoints::add_point(float x, float y, float z) {
    _points.insert({ {x,y}, z });
}

const std::map<std::pair<float, float>, float> &L::MeshPoints::points() const {
    return _points;
}

Point3D::Point3D(float x, float y, float z) : x(x), y(y), z(z) {}
