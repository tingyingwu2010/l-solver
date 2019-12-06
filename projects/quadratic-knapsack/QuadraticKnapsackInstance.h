#ifndef LSOLVERPROJECT_QUADRATICKNAPSACKINSTANCE_H
#define LSOLVERPROJECT_QUADRATICKNAPSACKINSTANCE_H

#include <string>
#include <vector>
#include <map>

class QuadraticKnapsackInstance {
    unsigned int _n_items = 0;
    std::map<std::pair<unsigned int, unsigned int>, float> _profits;
    std::map<unsigned int, float> _weigths;
    float _capacity = 0;
    float _sum_weights = 0;

    void add_profit(unsigned int i, unsigned int j, float value);
    void add_weight(unsigned int i, float w);
    std::vector<std::string> split_by_char(const std::string& line, char by = ' ');
public:
    explicit QuadraticKnapsackInstance(const std::string& filename);
    [[nodiscard]] const std::map<std::pair<unsigned int, unsigned int>, float>& profits() const;
    [[nodiscard]] const std::map<unsigned int, float>& weights() const;
    [[nodiscard]] float capacity() const;
    [[nodiscard]] float sum_of_weights() const;
    [[nodiscard]] unsigned int n_items() const;
};


#endif //LSOLVERPROJECT_QUADRATICKNAPSACKINSTANCE_H
