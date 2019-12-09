//
// Created by hlefebvr on 09/12/19.
//

#include <sstream>
#include <LSolver/utils/Exception.h>
#include "Surface.h"

std::string L::Surface::latex() const {
    if (_mesh_points.points().empty()) return "";
    std::stringstream s;

    unsigned int dim = 0, i = 1;
    float current_x = _mesh_points.points().begin()->first.first;
    for (const auto& m : _mesh_points.points()) {
        float x = m.first.first;
        float y = m.first.second;
        float z = m.second;
        if (current_x < x) {
            current_x = x;
            if (dim == 0) dim = i;
            else if (i != dim) throw Exception("Given points do not form a grid");
            i = 1;
            s << "\n";
        }
        s << "(" << x << ", " << y << ", " << z << ") ";
        i += 1;
    }

    std::stringstream output;
    output << "\\begin{tikzpicture}\n"
              << "\\begin{axis}[opacity=.5, view={" << _xy_rotation << "}{" << _z_rotation << "},"
              << "xlabel=" << xlabel() << ","
              << "ylabel=" << ylabel() << ","
              << (colorbar() ? "colorbar," : "")
              << "mesh, mesh/rows=" << dim - 1 << ", "
              << "mesh/ordering=x varies,"
              << "title={" << title() << "}]\n"
              << "\\addplot3[surf, ] coordinates {\n"
              << s.str()
              << "\n};\n"
              << "\\end{axis}\n"
              << "\\end{tikzpicture}";

    return output.str();
}

void L::Surface::colobar(bool colorbar) {
    _colorbar = colorbar;
}

bool L::Surface::colorbar() const {
    return _colorbar;
}

void L::Surface::rotation(float xy_rotation, float z_rotation) {
    _xy_rotation = xy_rotation;
    _z_rotation = z_rotation;
}

float L::Surface::xy_rotation() const {
    return _xy_rotation;
}

float L::Surface::z_rotation() const {
    return _z_rotation;
}

L::Surface::Surface(L::MeshPoints &mesh_points) : _mesh_points(mesh_points) {}

L::SubPlot *L::Surface::copy() const {
    return new Surface(*this);
}
