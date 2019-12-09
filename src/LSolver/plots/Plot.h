//
// Created by hlefebvr on 09/12/19.
//

#ifndef LSOLVERPROJECT_PLOT_H
#define LSOLVERPROJECT_PLOT_H

#include <map>
#include "SubPlot.h"

namespace L {
    class Plot;
}

class L::Plot {
    std::string _title;
    std::string _plot_temporary_folder = "tmp_plot";
    std::map<std::pair<unsigned int, unsigned int>, SubPlot*> _subplots;

    void get_dimension(unsigned int& max_row, unsigned int& max_col) const;
public:
    explicit Plot(std::string  title = "");
    virtual ~Plot();
    [[nodiscard]] const std::string& title() const;
    void title(const std::string& title);
    void add_subplot(unsigned int i, unsigned int j, const SubPlot& subplot);
    void render(const std::string& filename = "plot.png") const;
};

#endif //LSOLVERPROJECT_PLOT_H
