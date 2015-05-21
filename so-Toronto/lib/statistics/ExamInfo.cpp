
#include "ExamInfo.h"


// Ctor
ExamInfo::ExamInfo(int _examIndex, int _thresholdArraySize, boost::shared_array<double> _thresholdArray)
    : examIndex(_examIndex),                      // Exam index
      thresholdArraySize(_thresholdArraySize),    // Number of thresholds
      thresholdArray(_thresholdArray),            // Threshold array
      examColorDegree(0),                         // Exam color degree
      moveCounts(new long[_thresholdArraySize]()) // Move counts array
{


}




// Print exam information in  the format:
//
// Threshold 0     this exam index      move count
// Threshold 1     this exam index      move count
// ...
// Threshold N-1   this exam index      move count
//
// E.g.:
// 0.1    1   1000
// 0.01   1   500
// 0.001  1   600
//
ostream& operator<<(ostream& _os, const ExamInfo& _examInfo) {
//    _os << _examInfo.getExamColorDegree();

    int threshIdx = 0;
    double threshold;
    auto* ptr = _examInfo.moveCounts.get();

/// ADDED - SEQUENTIAL EXAMS INDEXS
///

    ++ExamInfo::_examIndex;

    for (long count : boost::make_iterator_range(ptr, ptr + _examInfo.thresholdArraySize)) {
        // Get threshold value
        threshold = _examInfo.getThreshold(threshIdx);
        // Print <threshold>, <exam index>, and <move count>
//        _os << threshold << '\t' << _examInfo.getExamIndex() << '\t' << count << endl;
        _os << threshold << '\t' << ExamInfo::_examIndex << '\t' << count << endl;
        ++threshIdx;


    }
    return _os;
}




int ExamInfo::_examIndex = 0;







