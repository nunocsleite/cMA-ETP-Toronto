
#include "data/TimetableProblemData.hpp"


using namespace std;


ostream& operator<<(ostream& _os, const TimetableProblemData& _timetableProblemData) {

    _os <<  "TimetableProblemData Info:" << endl
         << "NumExams: " << _timetableProblemData.getNumExams() << endl
         << "NumPeriods: " << _timetableProblemData.getNumPeriods() << endl
         << "NumStudents: " << _timetableProblemData.getNumStudents() << endl
         << "NumEnrolments: " << _timetableProblemData.getNumEnrolments() << endl
         << "ConflictMatrixDensity: " << _timetableProblemData.getConflictMatrixDensity() << endl
         << "CourseStudentCounts: " << endl;

    auto const& courseStudentCounts = *_timetableProblemData.getCourseStudentCounts().get();
    _os << "CourseStudentCounts.size() = " << courseStudentCounts.size() << endl;
//    std::copy(courseStudentCounts.begin(), courseStudentCounts.end(),
//              ostream_iterator<int>(_os, "\n"));

    return _os;
}
