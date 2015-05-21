#ifndef TIMETABLEPROBLEMDATA_H
#define TIMETABLEPROBLEMDATA_H

#include <vector>
#include <iostream>
#include "containers/IntMatrix.h"
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "utils/Common.h"
#include "Exam.h"


using namespace std;
using namespace boost;


class TimetableProblemData {

public:
    // Constructors
    TimetableProblemData() { }

    TimetableProblemData(int _numPeriods, int _numStudents, int _numExams, int _numEnrolments,
                         boost::shared_ptr<IntMatrix > _conflictMatrix,
                         boost::shared_ptr<AdjacencyList> _examGraph)

        : numPeriods(_numPeriods), numStudents(_numStudents), numExams(_numExams),
          numEnrolments(_numEnrolments),
          conflictMatrix(_conflictMatrix),
          examGraph(_examGraph)
    {
        // Compute conflict matrix density
        computeConflictMatrixDensity();

    }

    // Public interface
    //
    // Getters & setters
    int getNumPeriods() const;
    void setNumPeriods(int value);

    int getNumStudents() const;
    void setNumStudents(int value);

    int getNumExams() const;
    void setNumExams(int value);

    int getNumEnrolments() const;
    void setNumEnrolments(int value);

    double getConflictMatrixDensity() const;
    void setConflictMatrixDensity(double value);

    const IntMatrix &getConflictMatrix() const;
    void setConflictMatrix(const boost::shared_ptr<IntMatrix > &value);

    const AdjacencyList &getExamGraph() const;
    void setExamGraph(const boost::shared_ptr<AdjacencyList> &value);

    const boost::shared_ptr<vector<int> > &getCourseStudentCounts() const;
    void setCourseStudentCounts(const boost::shared_ptr<vector<int> > &_courseStudentCounts);

    friend ostream& operator<<(ostream& _os, const TimetableProblemData& _timetableProblemData);

private:

    //--
    // Auxiliary methods
    //--
    void computeConflictMatrixDensity() {
        // Compute conflict matrix density

        // Count the number of non-zero elements
        int nonZeroElements = 0;
        for (int i = 0; i < conflictMatrix->getNumLines(); ++i) {
            for (int j = 0; j < conflictMatrix->getNumCols(); ++j) {
                if (conflictMatrix->getVal(i,j) != 0)
                    ++nonZeroElements;
            }
        }

        // The ‘conflict’ density is the ratio of the number of non-zero elements
        // in the conflict matrix to the total number of conflict matrix elements.
        double numMatrixElements = conflictMatrix->getNumCols() * conflictMatrix->getNumCols();
        // Do not consider the matrix diagonal
        conflictMatrixDensity = nonZeroElements / (numMatrixElements - conflictMatrix->getNumCols());
    }


    //--
    // Fields
    //--
    // Number of Periods
    int numPeriods;
    // Number of Rooms
    int numRooms;
    // Number of students
    int numStudents;
    // Number of exams
    int numExams;
    // Number of enrolments
    int numEnrolments;
    // Conflict matrix density
    double conflictMatrixDensity;
    // Vector to keep course total students. Exams indexed from [0..numExams-1].
    boost::shared_ptr<vector<int> > courseStudentCounts;
    // Conflict matrix
    boost::shared_ptr<IntMatrix > conflictMatrix;
    // Graph
    boost::shared_ptr<AdjacencyList> examGraph;
};


inline int TimetableProblemData::getNumPeriods() const
{
    return numPeriods;
}
inline void TimetableProblemData::setNumPeriods(int value)
{
    numPeriods = value;
}

inline int TimetableProblemData::getNumStudents() const
{
    return numStudents;
}
inline void TimetableProblemData::setNumStudents(int value)
{
    numStudents = value;
}

inline int TimetableProblemData::getNumExams() const
{
    return numExams;
}
inline void TimetableProblemData::setNumExams(int value)
{
    numExams = value;
}

inline int TimetableProblemData::getNumEnrolments() const
{
    return numEnrolments;
}
inline void TimetableProblemData::setNumEnrolments(int value)
{
    numEnrolments = value;
}

inline double TimetableProblemData::getConflictMatrixDensity() const
{
    return conflictMatrixDensity;
}
inline void TimetableProblemData::setConflictMatrixDensity(double value) {
    conflictMatrixDensity = value;
}

inline const IntMatrix &TimetableProblemData::getConflictMatrix() const
{
    return *conflictMatrix.get();
}
inline void TimetableProblemData::setConflictMatrix(const boost::shared_ptr<IntMatrix > &value)
{
    // Set conflict matrix
    conflictMatrix = value;
    // Compute conflict matrix density
    computeConflictMatrixDensity();
}

inline AdjacencyList const& TimetableProblemData::getExamGraph() const
{
    return *examGraph.get();
}
inline void TimetableProblemData::setExamGraph(const boost::shared_ptr<AdjacencyList> &value)
{
    examGraph = value;
}

inline boost::shared_ptr<vector<int> > const& TimetableProblemData::getCourseStudentCounts() const
{
    return courseStudentCounts;
}
inline void TimetableProblemData::setCourseStudentCounts(const boost::shared_ptr<vector<int> > &_courseStudentCounts)
{
    courseStudentCounts = _courseStudentCounts;
}





#endif // TIMETABLEPROBLEMDATA_H











