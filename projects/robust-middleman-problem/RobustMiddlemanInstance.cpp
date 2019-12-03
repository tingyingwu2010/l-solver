//
// Created by hlefebvr on 03/12/19.
//

#include <fstream>
#include <iostream>
#include "RobustMiddlemanInstance.h"
#include <LSolver/utils/Exception.h>

HasId::HasId(unsigned int id) : _id(id) {}

unsigned int HasId::id() const {
    return _id;
}

Offer::Offer(unsigned int id, float buy_price, float sell_low_price, float sell_high_price)
    : HasId(id), _buy_price(buy_price), _sell_lowest_price(sell_low_price), _sell_highest_price(sell_high_price) {}

float Offer::buy_price() const {
    return _buy_price;
}

float Offer::sell_lowest_price() const {
    return _sell_lowest_price;
}

float Offer::sell_highest_price() const {
    return _sell_highest_price;
}

Demand::Demand(unsigned int id, float amount) : HasId(id), _amount(amount) {}

float Demand::amount() const {
    return _amount;
}

float RobustMiddlemanInstance::gamma() const {
    return _gamma;
}

float RobustMiddlemanInstance::budget() const {
    return _budget;
}

const std::vector<Offer>& RobustMiddlemanInstance::offers() const {
    return _offers;
}

const std::vector<Demand> &RobustMiddlemanInstance::demands() const {
    return _demands;
}

RobustMiddlemanInstance::RobustMiddlemanInstance(const std::string &filename, float gamma, float budget) : _gamma(gamma), _budget(budget) {
    std::ifstream f(filename);
    if (!f.is_open()) throw L::Exception("Could not open instance file");

    for (std::string line ; std::getline(f, line) ; ) {
        auto row = split_by_char(line);
        if (row[0] == "O") {
            if (row.size() != 4) throw L::Exception("Could not parse offer: " + line);
            _offers.emplace_back(Offer(_n_offer++, std::stof(row[1]), std::stof(row[2]), std::stof(row[3])));
        } else if (row[0] == "D") {
            if (row.size() != 2) throw L::Exception("Could not parse demand: " + line);
            _demands.emplace_back(Demand(_n_demand++, std::stof(row[1])));
        } else {
            throw L::Exception("Unexpected string: " + row[0]);
        }
    }

    f.close();
}

std::vector<std::string> RobustMiddlemanInstance::split_by_char(const std::string& line, char by) {
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

