

#include "containers/TimetableContainerMatrix.h"

using namespace  std;




/**
 * @brief removeAllPeriodExams Remove all period exams
 * @param _ti
 */
void TimetableContainerMatrix::removeAllPeriodExams(int _ti) {
    for (int ei = 0; ei < timetableContainer.getNumLines(); ++ei) {
        timetableContainer.setVal(ei, _ti, REMOVE_EXAM);
    }
    periodsSizes[_ti] = 0;
}



/**
 * @brief replacePeriod Copy period _tj to period _ti
 * @param _ti
 * @param _tj
 */
void TimetableContainerMatrix::replacePeriod(int _ti, int _tj) {
    auto const& colTj = timetableContainer.getColumn(_tj);
    timetableContainer.setColumn(_ti, colTj);
    periodsSizes[_ti] = periodsSizes[_tj];
}


/**
 * @brief replacePeriod Copy external _periodExams vector, with _size exams, to period _ti
 * @param _ti
 * @param _periodExams
 * @param _size
 */
void TimetableContainerMatrix::replacePeriod(int _ti, const std::vector<int> &_periodExams, int _size) {
    timetableContainer.setColumn(_ti, _periodExams);
    periodsSizes[_ti] = _size;
}










