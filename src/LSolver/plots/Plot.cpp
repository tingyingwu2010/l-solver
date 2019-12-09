//
// Created by hlefebvr on 09/12/19.
//

#include "Plot.h"
#include <utility>
#include <filesystem>
#include <fstream>
#include <LSolver/utils/Exception.h>
#include <iostream>

L::Plot::Plot(std::string title) : _title(std::move(title)) {}

void L::Plot::title(const std::string &title) {
    _title = title;
}

const std::string &L::Plot::title() const {
    return _title;
}

void L::Plot::add_subplot(unsigned int i, unsigned int j, const L::SubPlot &subplot) {
    if (i == 0 || j == 0) throw Exception("Subplot indices start from 1");
    _subplots.insert({ {i,j}, subplot.copy() });
}

void L::Plot::render(const std::string &filename) const {

    // check extension
    const std::string extension = filename.substr(filename.find_last_of('.') + 1);
    if (extension != "tex" && extension != "pdf" && extension != "png")
        throw Exception("Expected extension among .tex, .pdf and .png");

    // create temporary folder
    if (!std::filesystem::exists(_plot_temporary_folder)) {
        bool created = std::filesystem::create_directories(_plot_temporary_folder);
        if (!created) throw Exception("Could not create temporary folder");
    }

    // create latex file
    std::fstream f(_plot_temporary_folder + "/plot.tex", std::ios::out);
    if (!f.is_open()) throw Exception("Could not open output file");

    unsigned int max_row, max_col;
    get_dimension(max_row, max_col);

    std::string cs;
    for (unsigned int i = 1 ; i <= max_col ; i += 1) cs += 'c';

    f << "\\documentclass{standalone}\n"
         "\n"
         "\\usepackage{tikz}\n"
         "\\usepackage{varwidth}\n"
         "\\usepackage{pgfplots}\n"
         "\\usepgfplotslibrary{patchplots}\n"
         "\n"
         "\\begin{document}\n"
         "\\begin{tabular}{" << cs << "}\n"
         "\\multicolumn{" << max_col << "}{c}{\\huge " << _title << "} \\\\";
    auto it = _subplots.begin(), end = _subplots.end();
    for (unsigned int i = 1 ; i <= max_row ; i += 1) {
        for (unsigned int j = 1 ; j <= max_col ; j += 1) {
            if(j > 1) f << " & ";
            if (it != end && i == it->first.first && j == it->first.second) {
                f << it->second->latex() << "\n";
                it++;
            }
        }
        f << "\\\\\n";
    }
    f << "\\end{tabular}\n"
         "\\end{document}" << std::endl;

    f.close();

    // compile as pdf
    if (extension != "tex") system(std::string("cd " + _plot_temporary_folder + " && pdflatex plot.tex").c_str());

    // mv if pdf
    if (extension == "pdf") system(std::string("mv " + _plot_temporary_folder + "/plot.pdf " + filename).c_str());

    // convert if png
    if (extension == "png") system(std::string("pdftoppm -png -r 90 " + _plot_temporary_folder + "/plot.pdf > " + filename).c_str());

    // clear temporary folder
    std::filesystem::remove_all(_plot_temporary_folder);
}

void L::Plot::get_dimension(unsigned int &max_row, unsigned int &max_col) const {
    max_row = 0;
    max_col = 0;
    for (const auto& m : _subplots) {
        if (m.first.first > max_row) max_row = m.first.first;
        if (m.first.second > max_col) max_col = m.first.second;
    }
}

L::Plot::~Plot() {
    for (auto& m : _subplots) delete m.second;
}
