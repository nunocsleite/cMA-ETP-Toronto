#ifndef EOCHROMOSOME_H
#define EOCHROMOSOME_H


#include <EO.h>

#include "containers/Matrix.h"
#include "containers/TimetableContainer.h"
#include "containers/TimetableContainerMatrix.h"
#include "data/TimetableProblemData.hpp"
#include "utils/Common.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <vector>
#include <string>
#include <ostream>
#include "kempeChain/ETTPKempeChain.h"



// For debugging purposes
//#define EOCHROMOSOME_DEBUG


#define EOCHROMOSOME_DEBUG_COPY_CTOR


// EO (Evolving Object) chromosome implementation.
// A chromosome encodes a complete and feasible timetable.
// The chromosome holds a matrix where the columns represent the periods,
// holding, by its turn, the period exams.
// For fast access, insertion, removal, and random selection of exams,
// the timetable matrix has dimensions of (# exams x # periods) where the
// values 0/1 represent, respectively, absence/presence of exam in the period.
////
class eoChromosome : public EO<double> {

public:
    /**
     * @brief Chromosome Default chromosome constructor
     */
    eoChromosome()
        :
          /// Implementation #1 - TimetableContainerMatrix
          ///
          timetableContainer(boost::make_shared<TimetableContainerMatrix>()),
          timetableProblemData(nullptr),
          feasible(false),
          examProximityConflicts(0) { }

    /**
     * @brief eoChromosome Copy constructor
     * @param _chrom
     */
    eoChromosome(const eoChromosome &_chrom)
        : timetableContainer(boost::make_shared<TimetableContainerMatrix>(_chrom.getNumExams(), _chrom.getNumPeriods())),
          timetableProblemData(_chrom.getTimetableProblemData()),
          feasible(_chrom.isFeasible()),
          examProximityConflicts(_chrom.examProximityConflicts) {

        // Copy timetable data
        copyTimetableData(_chrom);

        // Set fitness
        fitness(_chrom.fitness());

#ifdef EOCHROMOSOME_DEBUG_COPY_CTOR
//            std::cout << "eoChromosome::Copy ctor" << std::endl;
#endif
    }

    /**
     * @brief operator =
     * @param _chrom
     * @return
     */
    eoChromosome& operator=(const eoChromosome &_chrom) {
#ifdef EOCHROMOSOME_DEBUG_COPY_CTOR
        std::cout << "eoChromosome::operator=" << std::endl;
        std::cin.get();
#endif

        if (&_chrom != this) {
            timetableContainer = boost::make_shared<TimetableContainerMatrix>(_chrom.getNumExams(), _chrom.getNumPeriods());
            timetableProblemData = _chrom.getTimetableProblemData();
            feasible = _chrom.isFeasible();
            examProximityConflicts = _chrom.examProximityConflicts;
            // Copy timetable data
            copyTimetableData(_chrom);
            // Set fitness
            fitness(_chrom.fitness());
        }
        return *this;
    }

protected:
    // Copy timetable data
    void copyTimetableData(const eoChromosome &_chrom) {
        // Copy timetable data
        auto &timetableContThis = getTimetableContainer();
        auto const &timetableContOther = _chrom.getTimetableContainer();

        // For each period do
        for (int pi = 0; pi < timetableContOther.getNumPeriods(); ++pi) {
            // Get period pi exams and size
            auto& periods = timetableContOther.getPeriodExams(pi);
            int periodSize = timetableContOther.getPeriodSize(pi);
            // Copy to this timetable
            timetableContThis.replacePeriod(pi, periods, periodSize);
        }
    }

public:

    /**
     * @brief init Initialise the chromosome
     * @param _timetableProblemData Timetable problem data
     */
    void init(TimetableProblemData const* _timetableProblemData);

    ////////// TimetableProblemData methods ////////////////////////////////////////////////
    /**
     * @brief getNumPeriods
     * @return The number of periods
     */
    inline int getNumPeriods() const { return timetableProblemData->getNumPeriods(); }
    /**
     * @brief getNumStudents
     * @return The nmber of students
     */
    int getNumStudents() const { return timetableProblemData->getNumStudents(); }
    /**
     * @brief getNumExams
     * @return The number of exams
     */
    int getNumExams() const { return timetableProblemData->getNumExams(); }
    /**
     * @brief getNumEnrolments
     * @return The number of exams
     */
    int getNumEnrolments() const { return timetableProblemData->getNumEnrolments(); }
    /**
     * @brief getConflictMatrixDensity
     * @return The conflict matrix density
     */
    double getConflictMatrixDensity() const { return timetableProblemData->getConflictMatrixDensity(); }
    /**
     * @brief getConflictMatrix
     * @return The conflict matrix
     */
    Matrix<int> const &getConflictMatrix() const { return timetableProblemData->getConflictMatrix(); }
    /**
     * @brief getGraph
     * @return The exam graph
     */
    AdjacencyList const &getExamGraph() { return timetableProblemData->getExamGraph(); }
    /**
     * @brief getCourseStudentCounts Obtain course student counts
     * @return
     */
    std::vector<int> const& getCourseStudentCounts() const {
        return *timetableProblemData->getCourseStudentCounts().get();
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief getTimetableContainer Return the timetable container
     * @return
     */
    TimetableContainer &getTimetableContainer() { return *timetableContainer.get(); }
    TimetableContainer const &getTimetableContainer() const { return *timetableContainer.get(); }

    ////////// Chromosome cost and feasibility manipulation methods //////////////////////////////
    /**
     * @brief setFeasible
     * @param feasible
     */
    void setFeasible(bool _feasible) { feasible = _feasible; }
    /**
     * @brief isFeasible
     * @return true if chromosome is feasible and false otherwise
     */
    bool isFeasible() const { return feasible; }
    /**
     * @brief getProximityCost Return chromosome's proximity cost
     * @return
     */
    double getProximityCost() const { return (double)examProximityConflicts / getNumStudents(); }
    /**
     * @brief computeProximityCost Compute chromosome's proximity cost
     */
    void computeExamProximityConflicts();

    /**
     * @brief computeExamProximityConflictsIncremental Compute chromosome's proximity cost
     * performing an incremental evaluation based on the Kempe chain information.
     *
     * @param _kempeChain
     */
    void computeExamProximityConflictsIncremental(const ETTPKempeChain<eoChromosome> &_kempeChain);

    void countConflictsAllExams();

    /**
     * @brief getExamProximityConflicts
     * @param _ei
     * @param _period
     * @return
     */
    long getExamProximityConflicts(int _ei, int _pi);

    /**
     * @brief getExamProximityConflicts Return exam proximity conflicts
     * @return
     */
    long getProximityConflicts() { return examProximityConflicts; }
    /**
     * @brief setExamProximityConflicts Set exam proximity conflicts
     * @param _examProximityConflicts
     */
    void setExamProximityConflicts(long _examProximityConflicts) { examProximityConflicts = _examProximityConflicts; }
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ////////// Debug methods /////////////////////////////////////////////////////////////////////
    /**
     * @brief validate Validate a chromosome solution
     */
    void validate() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief className +Overriden method+
     * @return The class name
     */
    std::string className() const override { return "EO Chromosome"; }
    /**
     * @brief operator << Print chromosome contents
     * @param os
     * @param timetable
     * @return
     */
    friend std::ostream& operator<<(std::ostream& os, const eoChromosome& timetable);

    void printToFile(ostream &_os);

    inline const TimetableProblemData *getTimetableProblemData() const;
    inline void setTimetableProblemData(const TimetableProblemData *value);

    bool isExamFeasibleInPeriod(int _exam, int _period) const;

protected:
    /**
     * @brief timetableMatrix The timetable container
     */
    boost::shared_ptr<TimetableContainer> timetableContainer;
    /**
     * @brief timetableProblemData The problem data
     */
    TimetableProblemData const* timetableProblemData;
    /**
     * @brief isFeasible This variable is used to determine if the chromosome is feasible or not
     */
    bool feasible;
    /**
     * @brief examProximityConflicts Sum of exam proximity conflicts
     */
    long examProximityConflicts;
};


const TimetableProblemData *eoChromosome::getTimetableProblemData() const
{
    return timetableProblemData;
}

void eoChromosome::setTimetableProblemData(const TimetableProblemData *value)
{
    timetableProblemData = value;
}




#endif // EOCHROMOSOME_H
