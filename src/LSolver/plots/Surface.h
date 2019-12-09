//
// Created by hlefebvr on 09/12/19.
//

#ifndef LSOLVERPROJECT_SURFACE_H
#define LSOLVERPROJECT_SURFACE_H

#include <map>
#include "SubPlot.h"
#include "MeshPoints.h"

namespace L {
    class Surface;
}

class L::Surface : public SubPlot {
    MeshPoints& _mesh_points;
    bool _colorbar = true;
    float _xy_rotation = 45;
    float _z_rotation = 45;
public:
    explicit Surface(MeshPoints& mesh_points);
    [[nodiscard]] std::string latex() const override;
    [[nodiscard]] SubPlot* copy() const override;
    void colobar(bool colorbar);
    [[nodiscard]] bool colorbar() const;
    void rotation(float xy_rotation, float z_rotation);
    [[nodiscard]] float xy_rotation() const;
    [[nodiscard]] float z_rotation() const;
};

#endif //LSOLVERPROJECT_SURFACE_H
