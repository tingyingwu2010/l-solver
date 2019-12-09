//
// Created by hlefebvr on 09/12/19.
//

#ifndef LSOLVERPROJECT_MESHPOINTS_H
#define LSOLVERPROJECT_MESHPOINTS_H

#include <map>

namespace L {
    class MeshPoints;
}

struct Point3D {
    float x;
    float y;
    float z;
    Point3D(float x, float y, float z);
};

class L::MeshPoints {
    std::map<std::pair<float, float>, float> _points;
public:
    void add_point(float x, float y, float z);
    [[nodiscard]] const std::map<std::pair<float, float>, float>& points() const;
};


#endif //LSOLVERPROJECT_MESHPOINTS_H
