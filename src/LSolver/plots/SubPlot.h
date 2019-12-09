//
// Created by hlefebvr on 09/12/19.
//

#ifndef LSOLVERPROJECT_SUBPLOT_H
#define LSOLVERPROJECT_SUBPLOT_H

#include <string>

namespace L {
    class SubPlot;
}

class L::SubPlot {
    std::string _xlabel = "$x$";
    std::string _ylabel = "$y$";
    std::string _title ;
public:
    virtual ~SubPlot() = default;
    [[nodiscard]] virtual std::string latex() const = 0;
    void title(const std::string& title);
    void xlabel(const std::string& xlabel);
    void ylabel(const std::string& ylabel);
    [[nodiscard]] const std::string& xlabel() const;
    [[nodiscard]] const std::string& ylabel() const;
    [[nodiscard]] const std::string& title() const;
    [[nodiscard]] virtual SubPlot* copy() const = 0;
};


#endif //LSOLVERPROJECT_SUBPLOT_H
