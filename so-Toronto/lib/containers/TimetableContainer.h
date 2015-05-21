#ifndef TIMETABLECONTAINER_H
#define TIMETABLECONTAINER_H


#include <boost/unordered_set.hpp>


/**
 * @brief The TimetableContainer class Abstract timetable container class
 */
class TimetableContainer {

public:

    /**
     * @brief getNumPeriods Get # exams
     * @return
     */
    virtual int getNumPeriods() const = 0;
    /**
     * @brief insertExam Insert exam _ei into period _tj
     * @param _ei
     * @param _tj
     */
    virtual void insertExam(int _ei, int _tj) = 0;
    /**
     * @brief removeExam Remove exam _ei from period _tj
     * @param _ei
     * @param _tj
     */
    virtual void removeExam(int _ei, int _tj) = 0;
    /**
     * @brief isExamScheduled
     * @param _ei
     * @param _tj
     * @return true if exam _ei is scheduled in time slot _tj
     */
    virtual bool isExamScheduled(int _ei, int _tj) const = 0;
    /**
     * @brief removeAllPeriodExams Remove all period exams
     * @param _ti
     */
    virtual void removeAllPeriodExams(int _ti) = 0;
    /**
     * @brief getPeriodSize Return period size
     * @param _period
     * @return
     */
    virtual int getPeriodSize(int _ti) const = 0;

    /**
     * @brief getPeriodExams Return _ti period exams vector
     * @param _ti
     * @return
     */
    virtual std::vector<int> const& getPeriodExams(int _ti) const  = 0;

    /**
     * @brief replacePeriod Copy period _tj to period _ti
     * @param _ti
     * @param _tj
     */
    virtual void replacePeriod(int _ti, int _tj) = 0;

    /**
     * @brief replacePeriod Copy external _periodExams vector, with _size exams, to period _ti
     * @param _ti
     * @param _periodExams
     * @param _size
     */
    virtual void replacePeriod(int _ti, const std::vector<int> &_periodExams, int _size) = 0;
};




#endif // TIMETABLECONTAINER_H
