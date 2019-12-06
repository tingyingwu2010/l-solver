//
// Created by hlefebvr on 06/12/19.
//

#include <fstream>
#include <LSolver/utils/Exception.h>
#include <iostream>
#include "QuadraticKnapsackInstance.h"

QuadraticKnapsackInstance::QuadraticKnapsackInstance(const std::string &filename) {
    std::ifstream f(filename);
    if (!f.is_open()) throw L::Exception("Could not open instance file");

    unsigned int i = 0;
    unsigned int i_last_quad = 0;
    unsigned int i_capacity = 0;
    for (std::string line ; std::getline(f, line) ; ) {
        i += 1;
        if (i == 1) {
            // skip first line
            continue;
        }

        if (i == 2) {
            _n_items = std::stoi(line);
            std::cout << "Number of items is " << _n_items << std::endl;
            i_last_quad = i + _n_items;
            i_capacity = i_last_quad + 2;
            continue;
        }

        auto row = split_by_char(line);
        if (i == 3) {
            unsigned int item_i = 1;
            for (const auto& v : row) {
                add_profit(item_i, item_i, std::stof(v));
                item_i += 1;
            }
            continue;
        }

        if (i <= i_last_quad) {
            unsigned int item_i = _n_items - (i_last_quad - i + 1);
            unsigned int item_j = item_i + 1;
            for (const auto& v : row) {
                add_profit(item_i, item_j, std::stof(v));
                item_j += 1;
            }
            continue;
        }

        if (i <= i_capacity) continue;

        if (_capacity == 0) {
            _capacity = std::stof(line);
            continue;
        }

        unsigned int item_i = 1;
        for (const auto& v : row) {
            add_weight(item_i, std::stof(v));
            item_i += 1;
        }

    }

    f.close();
}

std::vector<std::string> QuadraticKnapsackInstance::split_by_char(const std::string& line, char by) {
    std::vector<std::string> arr;
    std::string str;
    for (char c : line) {
        if (c == by) {
            arr.emplace_back(str);
            str = "";
        } else {
            str += c;
        }
    }
    if (!str.empty()) arr.emplace_back(str);
    return arr;
}

void QuadraticKnapsackInstance::add_profit(unsigned int i, unsigned int j, float value) {
    if (value == 0) return;
    if (i < j) return add_profit(j, i, value);
    _profits.insert({ {i,j}, value });
}

void QuadraticKnapsackInstance::add_weight(unsigned int i, float w) {
    if (w == 0) return;
    _weigths.insert({ i, w });
    _sum_weights += w;
}

float QuadraticKnapsackInstance::capacity() const {
    return _capacity;
}

const std::map<unsigned int, float> &QuadraticKnapsackInstance::weights() const {
    return _weigths;
}

const std::map<std::pair<unsigned int, unsigned int>, float> &QuadraticKnapsackInstance::profits() const {
    return _profits;
}

float QuadraticKnapsackInstance::sum_of_weights() const {
    return _sum_weights;
}

unsigned int QuadraticKnapsackInstance::n_items() const {
    return _n_items;
}
