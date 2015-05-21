#ifndef EXAMINFO_H
#define EXAMINFO_H


#include <ostream>
#include <boost/shared_array.hpp>
#include <boost/range/algorithm.hpp>

using namespace std;


class ExamInfo {

public:
    // Ctor
    ExamInfo(int _examIndex, int _numThresholds, boost::shared_array<double> _thresholdArray);
    inline int getExamColorDegree() const;
    inline void setExamColorDegree(int value);
    inline double getThreshold(int _thresholdIndex) const;
    // Update exam move count
    inline void updateMove(int _threshIndex);
    // Get exam index
    inline int getExamIndex() const;

    friend ostream& operator<<(ostream& _os, const ExamInfo& _examInfo);

    inline int getSequentialExamIndex() const;
    inline long getExamMoveCount(int _threshIndex) const;

private:
    int examIndex;
    int thresholdArraySize;
    /// TODO Color? degree

    int examColorDegree; // Exam color degree
    boost::shared_array<double> thresholdArray; // Threshold array sorted in descending order
    boost::shared_array<long> moveCounts; // Array containing this exam move counts.
                                          // The array dimension equals the number of thresholds.

    static int _examIndex;
};




int ExamInfo::getExamColorDegree() const
{
    return examColorDegree;
}

void ExamInfo::setExamColorDegree(int value)
{
    examColorDegree = value;
}

double ExamInfo::getThreshold(int _thresholdIndex) const {
    return thresholdArray[_thresholdIndex];
}

// Update exam move count
void ExamInfo::updateMove(int _threshIndex) {
    ++moveCounts[_threshIndex];
}

int ExamInfo::getExamIndex() const
{
    return examIndex;
}

int ExamInfo::getSequentialExamIndex() const {
    return _examIndex;
}

long ExamInfo::getExamMoveCount(int _threshIndex) const {
    return moveCounts[_threshIndex];
}

#endif // EXAMINFO_H

