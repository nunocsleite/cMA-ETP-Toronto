#ifndef PERIOD_H
#define PERIOD_H

#include <boost/shared_ptr.hpp>
#include <vector>
#include "Exam.h"
#include <ostream>





class Period {

public:
    Period() : periodExams(0) { }
    vector<boost::shared_ptr<Exam> > const& getExams() const { return periodExams; }

    int getCapacity() const { return periodExams.size(); }

    void addExam(boost::shared_ptr<Exam> const& exam) {
        periodExams.push_back(exam);
    }

//    friend ostream& operator<<(ostream& out, Period const& period) {
//        out << "Id = " << period.getId() << endl;
//        out << "capacity = " << period.getCapacity() << endl;
//        out << "Exams = ";
//        out << "Exams size = " << period.getExams().size() << endl;
//        for (vector<boost::shared_ptr<Exam> >::const_iterator it = period.getExams().begin(); it != period.getExams().end(); ++it) {
//            out << (*it).get()->getId() << endl;
//        }
//        return out;
//    }

private:

    std::vector<int> periodExams;

};




#endif // PERIOD_H
