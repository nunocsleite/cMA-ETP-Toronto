#ifndef DATA
#define DATA

#include <vector>
#include <iostream>
#include "containers/Matrix.h"
#include <boost/unordered_map.hpp>
#include "utils/Common.h"

using namespace std;
using namespace boost;


class ProblemData {
    // Conflict matrix
    Matrix<int> const* conflictMatrix;
    // Graph
    AdjacencyList const* graph;
    // Number of students
    int numStudents;
    // Number of exams
    int numExams;
    // Number of enrolments
    int numEnrolments;
    // Course student counts
    unordered_map<int, int> const* courseStudentCounts;
    // Course names
    vector<string> const* courseNames;

public:
	ProblemData() { }
    ProblemData(Matrix<int> const* conflictMatrix, AdjacencyList const* graph,
                int numStudents, int numExams,
                int numEnrolments, unordered_map<int, int> const* courseStudentCounts,
                vector<string> const* courseNames)
        : conflictMatrix(conflictMatrix), graph(graph), numStudents(numStudents),
          numExams(numExams), numEnrolments(numEnrolments), courseStudentCounts(courseStudentCounts),
          courseNames(courseNames) { }
    Matrix<int> const* getConflictMatrix() const { return conflictMatrix; }
    AdjacencyList const* getGraph() const { return graph; }

    int getNumStudents() const { return numStudents; }
    int getNumExams() const { return numExams; }
    int getNumEnrolments() const { return numEnrolments; }
    unordered_map<int, int> const* getCourseStudentCounts() const { return courseStudentCounts; }
    vector<string> const* getCourseNames() const { return courseNames; }
};

//
// Used also in variable size chromosomes
//
class Data {

/// Refactor

    //===
	// Algorithm data
	//===
	// Min number of periods.
    int minPeriods;
    // Max number of periods.
    int maxPeriods;
    // Period range
	vector<int> range;
    // Original number of Periods
    int originalNumPeriods;

    // Number of Periods
    int numPeriods;
    // Number of Rooms
    int numRooms;

    //===
	// Problem data
	//===
	ProblemData problData;

public:

    Data(int _minPeriods, int _maxPeriods, int _originalNumPeriods, int _numRooms)
        : minPeriods(_minPeriods), maxPeriods(_maxPeriods),
          originalNumPeriods(_originalNumPeriods), numRooms(_numRooms) {
        range.push_back(minPeriods);
        range.push_back(maxPeriods);
    }

    Data(int _minPeriods, int _maxPeriods, int _originalNumPeriods)
        : minPeriods(_minPeriods), maxPeriods(_maxPeriods), originalNumPeriods(_originalNumPeriods) {
		range.push_back(minPeriods);
		range.push_back(maxPeriods);
	}
	int getMinPeriods() const { return minPeriods; }
	int getMaxPeriods() const { return maxPeriods; }
	const vector<int>& getRange() const { return range; }

/// Refactor
    int getOriginalNumPeriods() const { return originalNumPeriods; }

    int getNumPeriods() const { return numPeriods; }
    int getNumRooms() const { return numRooms; }
	void setProblemData(const ProblemData& problData) { this->problData = problData; }
	ProblemData const& getProblemData() const { return problData; }
};


#endif
