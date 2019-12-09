//
// Created by hlefebvr on 08/12/19.
//

#ifndef LSOLVERPROJECT_ROBUSTWNTJINSTANCE_H
#define LSOLVERPROJECT_ROBUSTWNTJINSTANCE_H

#include <string>
#include <iostream>
#include <vector>

/**
 * see also: https://github.com/hlefebvr/cplex-cpp-1riwiUi/blob/master/
 */

/**
 * Structure to store jobs
 */
struct Job {
    const int _release_date;
    const int _deadline;
    const int _processing_time;
    const int _weight;
    const unsigned int _id;

    friend std::ostream& operator<<(std::ostream& os, const Job& x);

    Job(const unsigned int id, const int release_date, const int deadline, const int weight, const int processing_time)
            : _id(id), _release_date(release_date), _deadline(deadline), _processing_time(processing_time), _weight(weight) {}
};

/**
 * Job occurence of job _parent_job from _from to _to
 */
struct JobOccurence {
    const int _release;
    const int _deadline;
    const int  _weight;
    const int _processing_time;
    const unsigned int _parent_job_id;

    friend std::ostream& operator<<(std::ostream& os, const JobOccurence& x);

    explicit JobOccurence(unsigned int parent_id, int release, int deadline, int weight, int processing_time)
            : _parent_job_id(parent_id), _release(release), _deadline(deadline), _weight(weight), _processing_time(processing_time) {}
};


class RobustWNTJInstance {
    const std::string _instance_filename;
    const bool _verbose;
    std::vector<const Job*> _jobs;
    std::vector<const JobOccurence*> _occurences;
    int _max_deadline = 0;

    void load_jobs_from_instance();
    void build_occurences_from_jobs();
    void apply_edf_rule();
    std::vector<std::string> get_next_row(std::ifstream& reader) const;

    RobustWNTJInstance(const RobustWNTJInstance& instance, bool);
public:
    RobustWNTJInstance(std::string  filename, bool verbose);
    [[nodiscard]] const std::vector<const Job*>& jobs() const { return _jobs; }
    [[nodiscard]] const std::vector<const JobOccurence*>& occurences() const { return _occurences; }
    [[nodiscard]] bool verbose() const { return _verbose; }
    [[nodiscard]] int max_deadline() const { return _max_deadline; }
    [[nodiscard]] std::string instance_name() const { return _instance_filename.substr(_instance_filename.find_last_of("/\\") + 1); }

    static RobustWNTJInstance reverse(const RobustWNTJInstance& instance);
};


#endif //LSOLVERPROJECT_ROBUSTWNTJINSTANCE_H
