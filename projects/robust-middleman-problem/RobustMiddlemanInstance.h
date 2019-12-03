//
// Created by hlefebvr on 03/12/19.
//

#ifndef LSOLVERPROJECT_ROBUSTMIDDLEMANINSTANCE_H
#define LSOLVERPROJECT_ROBUSTMIDDLEMANINSTANCE_H

#include <vector>
#include <string>

class HasId {
    const unsigned int _id;
public:
    explicit HasId(unsigned int id);
    [[nodiscard]] unsigned int id() const;
};

class Offer : public HasId {
    const float _buy_price;
    const float _sell_lowest_price;
    const float _sell_highest_price;
public:
    Offer(unsigned int id, float buy_price, float sell_low_price, float sell_high_price);
    [[nodiscard]] float buy_price() const;
    [[nodiscard]] float sell_lowest_price() const;
    [[nodiscard]] float sell_highest_price() const;
};

class Demand : public HasId {
    const float _amount;
public:
    Demand(unsigned int id, float amount);
    [[nodiscard]] float amount() const;
};

class RobustMiddlemanInstance {
    unsigned int _n_offer = 0;
    unsigned int _n_demand = 0;
    std::vector<Offer> _offers;
    std::vector<Demand> _demands;
    const float _gamma;
    const float _budget;
    std::vector<std::string> split_by_char(const std::string& line, char by = ' ');
public:
    RobustMiddlemanInstance(const std::string& filename, float gamma, float budget);
    [[nodiscard]] float gamma() const;
    [[nodiscard]] float budget() const;
    [[nodiscard]] const std::vector<Offer>& offers() const;
    [[nodiscard]] const std::vector<Demand>& demands() const;
};


#endif //LSOLVERPROJECT_ROBUSTMIDDLEMANINSTANCE_H
