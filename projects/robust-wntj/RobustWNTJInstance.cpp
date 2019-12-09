//
// Created by hlefebvr on 08/12/19.
//

#include <algorithm>
#include <fstream>
#include <regex>
#include <utility>
#include "RobustWNTJInstance.h"

#define MAX_LINE_SIZE 30

std::ostream& operator<<(std::ostream& os, const Job& x) {
    return os << "job " << x._id << " [" << x._release_date << ", " << x._deadline << "], p = " << x._processing_time << ", w = " << x._weight;
}

std::ostream &operator<<(std::ostream &os, const JobOccurence&  x) {
    return os << "job occurence from " << x._release << " to " << x._deadline << ", w = " <<  x._weight << " (comming from job " << x._parent_job_id << ")";
}

RobustWNTJInstance::RobustWNTJInstance(std::string filename, bool verbose) : _instance_filename(std::move(filename)), _verbose(verbose) {
    load_jobs_from_instance();
    build_occurences_from_jobs();
    apply_edf_rule();

    if (_verbose) std::cout << std::endl;
}

std::vector<std::string> RobustWNTJInstance::get_next_row(std::ifstream& reader) const {
    char* placeholder = new char[MAX_LINE_SIZE];
    reader.getline(placeholder, MAX_LINE_SIZE);
    std::string line(placeholder);

    std::regex reg = std::regex("[[:alnum:][:punct:]]+");
    std::smatch match;

    std::vector<std::string> cols;
    while( regex_search(line, match, reg) ) {
        cols.push_back(match[0]);
        line = match.suffix().str();
    }

    delete[] placeholder;

    return cols;
}

void RobustWNTJInstance::load_jobs_from_instance() {
    if(_verbose) std::cout << "Loading jobs from instance file :\n---------------------------------" << std::endl;

    enum COLUMN { WEIGHT, RELEASE_DATE, DEADLINE, PROCESSING_TIME };

    std::ifstream reader = std::ifstream(_instance_filename, std::ios::in);
    if (reader.fail()) throw std::runtime_error("Could not open instance file");

    get_next_row(reader); // skip the header

    unsigned int id = 0;
    while (!reader.eof()) {
        auto row = get_next_row(reader);

        if (row.size() == 4) {
            const int weight = stoi(row[COLUMN::WEIGHT]);
            const int release_date = stoi(row[COLUMN::RELEASE_DATE]);
            const int deadline = stoi(row[COLUMN::DEADLINE]);
            const int processing_time = stoi(row[COLUMN::PROCESSING_TIME]);

            const Job *new_job = new Job(id, release_date, deadline, weight, processing_time);
            if (_verbose) std::cout << "Creating " << *new_job << std::endl;
            _jobs.emplace_back(new_job);

            id += 1;
        }
    }

    reader.close();
}

void RobustWNTJInstance::build_occurences_from_jobs() {
    if (_verbose) std::cout << "Sorting jobs by release date" << std::endl;
    std::vector<const Job*> jobs_copied = _jobs;
    sort(jobs_copied.begin(), jobs_copied.end(), [](const Job* A, const Job* B) { return A->_release_date < B->_release_date; });

    for (unsigned long int i = 0, n_jobs = jobs_copied.size() ; i < n_jobs ; i += 1) {
        const Job& job_i = *jobs_copied[i];
        if(_verbose) std::cout << "Creating job occurences for job " << job_i << std::endl;
        if (_max_deadline < job_i._deadline) _max_deadline = job_i._deadline;

        for (unsigned long int j = i + 1 ; j < n_jobs ; j += 1) {
            const Job& job_j = *jobs_copied[j];

            if (job_i._deadline > job_j._deadline && job_i._release_date + job_i._processing_time + job_j._processing_time < job_j._deadline) {
                const int from = job_i._release_date;
                const int to = job_j._deadline;
                auto new_occurence = new JobOccurence(job_i._id, from, to, 0, job_i._processing_time);
                _occurences.emplace_back(new_occurence);

                if (_verbose) std::cout << "\tCreating " << *new_occurence << std::endl;
            } else if (job_i._deadline <= job_j._release_date) {
                break;
            }
        }

        auto new_occurence = new JobOccurence(job_i._id, job_i._release_date, job_i._deadline, 0, job_i._processing_time);
        if (_verbose) std::cout << "\tCreating default " << *new_occurence << std::endl;
        _occurences.emplace_back(new_occurence);

        new_occurence = new JobOccurence(job_i._id, 0, 0, job_i._weight, 0);
        if (_verbose) std::cout << "\tCreating tardy fictious " << *new_occurence << std::endl;
        _occurences.emplace_back(new_occurence);
    }

    if (_verbose) std::cout << "Done. (" + std::to_string(_occurences.size()) + " occurences have been generated)" << std::endl;
}

void RobustWNTJInstance::apply_edf_rule() {
    std::sort(_occurences.begin(), _occurences.end(), [](const JobOccurence* A, const JobOccurence* B){
        return std::pair<int, int>(A->_deadline, A->_release) < std::pair<int, int>(B->_deadline, B->_release);
    });
}

RobustWNTJInstance RobustWNTJInstance::reverse(const RobustWNTJInstance& instance) {
    return RobustWNTJInstance(instance, true);
}

RobustWNTJInstance::RobustWNTJInstance(const RobustWNTJInstance& instance, bool) : _instance_filename(instance._instance_filename), _verbose(instance._verbose) {
    const int dmax = instance._max_deadline;

    for (const Job* job : instance._jobs)
        _jobs.emplace_back(new Job(job->_id, dmax - job->_deadline, dmax - job->_release_date, job->_weight, job->_processing_time));

    for (const JobOccurence* job_occ : instance._occurences)
        _occurences.emplace_back(new JobOccurence(job_occ->_parent_job_id, dmax - job_occ->_deadline, dmax - job_occ->_release, job_occ->_weight, job_occ->_processing_time));

    _max_deadline = dmax - _occurences.front()->_release;
}
