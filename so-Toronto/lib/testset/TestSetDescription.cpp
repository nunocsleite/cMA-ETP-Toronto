
#include "TestSetDescription.h"


std::ostream& operator<<(std::ostream& _os, const TestSetDescription& _t) {
//    _os << "Test set\t # students\t # exams\t # enrolments\t conflict density\t # time slots" << endl;

//    _os << _t.getName() << "\t " << _t.getNumStudents() << "\t " << _t.getNumExams() << "\t "
//       << _t.getNumEnrolments() << "\t " << _t.getConflictDensity() << "\t " << _t.getNumPeriods() << endl;

    _os << _t.getName();

    return _os;
}

