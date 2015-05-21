#ifndef TESTSET_H
#define TESTSET_H

#include <iostream>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include "data/TimetableProblemData.hpp"



std::string read_from_file(char const* infile);



///////////////////////////////////////////////////////////////////////////////
//  Abstract class representing a test set
///////////////////////////////////////////////////////////////////////////////
class TestSet {

public:
    std::string getName()          const { return name;        }
    std::string getDescription()   const { return description; }
    std::string getRootDirectory() const { return rootDir;     }
    // Get timetable problem data
    boost::shared_ptr<TimetableProblemData> const& getTimetableProblemData() const {
        return timetableProblemData;
    }

    friend std::ostream& operator<<(std::ostream& _os, const TestSet& _t);

protected:
    // Protected constructor
    TestSet(std::string _name, std::string _description, std::string _rootDir,
            boost::shared_ptr<TimetableProblemData> _timetableProblemData)
        : name(_name), description(_description), rootDir(_rootDir), timetableProblemData(_timetableProblemData)
    { }

    // Pure virtual method
    virtual void load() = 0;

    // Set timetable problem data
    void setTimetableProblemData(boost::shared_ptr<TimetableProblemData> const&  _timetableProblemData) {
        timetableProblemData = _timetableProblemData;
    }

private:
    std::string name;
    std::string description;
    std::string rootDir;

protected:
    // Structure containing the timetable problem data
    boost::shared_ptr<TimetableProblemData> timetableProblemData;
};



#endif // TESTSET_H
