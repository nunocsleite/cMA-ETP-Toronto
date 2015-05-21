#ifndef TIMETABLECONTAINERMATRIX_H
#define TIMETABLECONTAINERMATRIX_H

#include "containers/TimetableContainer.h"
#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include "containers/ColumnMatrix.h"
#include "containers/Matrix.h"
#include <vector>


#define SCHED_EXAM  1
#define REMOVE_EXAM 0


//#define TIMETABLECONTAINERMATRIX_DEBUG


class TimetableContainerMatrix : public TimetableContainer {
public:

    /**
     * @brief TimetableContainerMatrix Default ctor
     */
    inline TimetableContainerMatrix();

    /**
     * @brief TimetableContainerMatrix::TimetableContainerMatrix Ctor which receives the #exams and #periods
     * @param _numExams
     * @param _numPeriods
     */
    inline TimetableContainerMatrix(int _numExams, int _numPeriods);

    /**
     * @brief getNumPeriods Get # exams
     * @return
     */
    inline virtual int getNumPeriods() const override;
    /**
     * @brief insertExam Insert exam _ei into period _tj
     *        Complexity: O(1)
     * @param _ei
     * @param _tj
     */
    inline virtual void insertExam(int _ei, int _tj) override;
    /**
     * @brief removeExam Remove exam _ei from period _tj
     *        Complexity: O(1)
     * @param _ei
     * @param _tj
     */
    inline virtual void removeExam(int _ei, int _tj) override;
    /**
     * @brief isExamScheduled
     *        Complexity: O(1)
     * @param _ei
     * @param _tj
     * @return true if exam _ei is scheduled in time slot _tj
     */
    inline virtual bool isExamScheduled(int _ei, int _tj) const override;
    /**
     * @brief removeAllPeriodExams Remove all period exams
     *        Complexity: O(NumExams)
     * @param _ti
     */
    virtual void removeAllPeriodExams(int _ti) override;
    /**
     * @brief getPeriodSize Return period size
     *        Complexity: O(1)
     * @param _period
     * @return
     */
    inline virtual int getPeriodSize(int _ti) const override;
    /**
     * @brief getPeriodExams Return _ti period exams vector
     * @param _ti
     * @return
     */
    inline virtual const std::vector<int>& getPeriodExams(int _ti) const override;
    /**
     * @brief replacePeriod Copy period _tj to period _ti
     * @param _ti
     * @param _tj
     */
    virtual void replacePeriod(int _ti, int _tj) override;

    /**
     * @brief replacePeriod Copy external _periodExams vector, with _size exams, to period _ti
     * @param _ti
     * @param _periodExams
     * @param _size
     */
    virtual void replacePeriod(int _ti, const std::vector<int> &_periodExams, int _size) override;

protected:
    /**
     * @brief timetableContainer The timetable container based on a matrix
     */
    ColumnMatrix<int> timetableContainer;

    /**
     * @brief periodExams Vector containing a hash set for each period exams,
     * in order to access in O(1) (ideally)
     */
    std::vector<int> periodsSizes;

};


TimetableContainerMatrix::TimetableContainerMatrix()
    : timetableContainer(),
      periodsSizes(0)
{ }



TimetableContainerMatrix::TimetableContainerMatrix(int _numExams, int _numPeriods)
    :
      // Initialise timetable matrix to have numExams x numCols size
      timetableContainer(_numExams, _numPeriods),
      // Initialize the periods sizes vector to have numCols size and zero value
      periodsSizes(_numPeriods)
{ }


/**
 * @brief getNumPeriods Get # exams
 * @return
 */
int TimetableContainerMatrix::getNumPeriods() const {
    return timetableContainer.getNumCols();
}


/**
 * @brief insertExam Insert exam _ei into period _tj
 * @param _ei
 * @param _tj
 */
void TimetableContainerMatrix::insertExam(int _ei, int _tj) {
    // Insert exam _ei into period _tj
    timetableContainer.setVal(_ei, _tj, SCHED_EXAM);
    // Increment period size
    ++periodsSizes[_tj];


//    std::cout << std::endl << "////////////////////////////////////// INSERT EXAM METHOD ///////////////" << std::endl;

//    int numExams = 0;
//    for (int ei = 0; ei < timetableContainer.getNumLines(); ++ei) {
//        if (isExamScheduled(ei, _tj))
//            ++numExams;
//    }
//    std::cout << "numExams = " << numExams << ", periodsSizes[_tj] = " << periodsSizes[_tj] << std::endl;

//    if (numExams != periodsSizes[_tj]) {
//        std::cout << "numExams != periodsSizes[_tj]" << std::endl;
//        cin.get();
//    }
}


/**
 * @brief removeExam Remove exam _ei from period _tj
 * @param _ei
 * @param _tj
 */
void TimetableContainerMatrix::removeExam(int _ei, int _tj) {
    // Remove exam _ei from period _tj
    timetableContainer.setVal(_ei, _tj, REMOVE_EXAM);
    // Decrement period size
    --periodsSizes[_tj];

//    std::cout << std::endl << "////////////////////////////////////// REMOVE EXAM METHOD ///////////////" << std::endl;

//    int numExams = 0;
//    for (int ei = 0; ei < timetableContainer.getNumLines(); ++ei) {
//        if (isExamScheduled(ei, _tj))
//            ++numExams;
//    }
//    std::cout << "numExams = " << numExams << ", periodsSizes[_tj] = " << periodsSizes[_tj] << std::endl;

//    if (numExams != periodsSizes[_tj]) {
//        std::cout << "numExams != periodsSizes[_tj]" << std::endl;
//        cin.get();
//    }
}


/**
 * @brief isExamScheduled
 * @param _ei
 * @param _tj
 * @return true if exam _ei is scheduled in time slot _tj
 */
bool TimetableContainerMatrix::isExamScheduled(int _ei, int _tj) const {
    return timetableContainer.getVal(_ei, _tj) == SCHED_EXAM;
}


/**
 * @brief getPeriodSize Return the period size (# scheduled exams) of period _period
 * @param _period
 * @return
 */
int TimetableContainerMatrix::getPeriodSize(int _ti) const {
    return periodsSizes[_ti];
}

/**
 * @brief getPeriodExams Return _ti period exams vector
 * @param _ti
 * @return
 */
const std::vector<int> &TimetableContainerMatrix::getPeriodExams(int _ti) const {
    return timetableContainer.getColumn(_ti);
}



#endif // TIMETABLECONTAINERMATRIX_H
