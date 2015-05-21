#ifndef TESTSETDESCRIPTION_H
#define TESTSETDESCRIPTION_H

#include <string>
#include <ostream>

//using namespace std;


/// TODO: REFACTORING
/// - Remove numPeriods fiels and ctor #1. For Toronto, also use TimetableProblemData
///


class TestSetDescription {
    std::string name;
    std::string description;
    int numPeriods;  /// USED IN TORONTO BENCHMARKS
public:
    TestSetDescription(std::string name, std::string description, int numPeriods) :   /// USED IN TORONTO BENCHMARKS
        name(name), description(description), numPeriods(numPeriods) { }

    TestSetDescription(std::string _name, std::string _description) :
        name(_name), description(_description) { }

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

    int getPeriods() const { return numPeriods; }      /// USED IN TORONTO BENCHMARKS

    friend std::ostream& operator<<(std::ostream& os, const TestSetDescription& t);
};


#endif // TESTSETDESCRIPTION_H
