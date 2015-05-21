

#include "eoChromosome.h"
#include <iostream>

#include <boost/container/set.hpp>

using namespace std;

#define NUM_PROXIMITY_PERIODS 5




//#define EOCHROMOSOME_INCREMENTAL




void eoChromosome::init(TimetableProblemData const* _timetableProblemData) {

    /// Implementation #1 - TimetableContainerMatrix
    ///
    // Instantiate timetable matrix with dimensions (# exams x # periods)
    timetableContainer = boost::make_shared<TimetableContainerMatrix>(
                _timetableProblemData->getNumExams(), _timetableProblemData->getNumPeriods());

    // Set timetable problem data
    timetableProblemData = _timetableProblemData;
    // Chromosome is not feasible initially
    feasible = false;

}



/**
 * @brief computeProximityCost Compute chromosome's proximity cost
 */
void eoChromosome::computeExamProximityConflicts() {
    //
    // The problem has one hard constraint where conflicting exams cannot be assigned to the same time slot.
    // In addition, a soft constraint is present where conflicting exams should be spread throughout the timetable
    // as far as possible from each other. The objective here is to minimise the sum of proximity costs given as:
    //    sum_{i=0}^{4} (wi x n) / S
    // where
    // - wi = 2^{4-i} is the cost of assigning two exams with i time slots apart. Only exams with
    //   common students and are four or less time slots apart are considered as violations
    // - n is the number of students involved in the conflict
    // - S is the total number of students in the problem
    //
    // Set exam proximity conflicts to zero
    examProximityConflicts = 0;
    // Get exam graph reference
    AdjacencyList const &examGraph = this->getExamGraph();
    // Timetable container
    TimetableContainer const &timetableCont = (*this->timetableContainer.get());
    // Conflict matrix
    Matrix<int> const &conflictMatrix = this->getConflictMatrix();
    // Weights array
    int wi[] = { 16, 8, 4, 2, 1 };
    int pj, n;
    // For each period (except the last period, which don't have conflicts) do
    for (int pi = 0; pi < timetableCont.getNumPeriods()-1; ++pi) {
        // For each exam of period pi do
        for (int ei = 0; ei < getNumExams(); ++ei) {
            // If exam ei is scheduled in period pi
            if (timetableCont.isExamScheduled(ei, pi)) {
///
/// OPTIMIZATION ASPECT: using if (matrix.getVal(ei, pi) == SCHED_EXAM)
/// Cons: With this solution, we have to iterate over the all exams, scheduled and not scheduled, in the current period,
///       but we have the advantage that we expect to reduce the cache misses because we use vectors.
///       Compared with using vec<unordered_set<int> > and the matrix implementation is faster (3x)
/// Pros: Insert, Removal, and Lookup exams in O(1) time
///
                // Get ei adjacent vertices
                property_map<AdjacencyList, vertex_index_t>::type index_map = get(vertex_index, examGraph);
                graph_traits<AdjacencyList>::adjacency_iterator ai, a_end;
                boost::tie(ai, a_end) = adjacent_vertices(ei, examGraph);
                for (; ai != a_end; ++ai) {
                    // Get adjacent exam
                    int ej = get(index_map, *ai);
                    // If exam ej is scheduled five periods apart, sum the corresponding weighted proximity cost
                    for (int i = 0; i <= NUM_PROXIMITY_PERIODS-1; ++i) {
                        pj = pi+i+1;
                        if (pj < timetableCont.getNumPeriods()) {
                            // If exam ej is scheduled in period pj
                            if (timetableCont.isExamScheduled(ej, pj)) {
                                n = conflictMatrix.getVal(ei, ej);
                                examProximityConflicts += wi[i]*n;
                                break; // Go to the next adjacent exam
                            }
                        }
                    }
                }
            }
        }
    }
}



void eoChromosome::countConflictsAllExams() {
    long solProximityConflicts = getProximityConflicts();
    std::cout << std::endl << "solution proximity cost full eval = "
              << solProximityConflicts << std::endl;
    std::cout << std::endl << "solution fitness full eval = "
              << getProximityCost() << std::endl;

    // The timetable container
    auto &timetableCont = getTimetableContainer();
    // Aux timetable that is a copy of the original timetable. This is used in the insertion test.
    TimetableContainerMatrix auxTimetable(getNumExams(), getNumPeriods());

    //////////////////////////////////////// TEST - REMOVE ALL EXAMS //////////////////////////////////////////////////
    /// Accumulated costs should be equal to original cost
    ///
    long examProximityConflictsOfRemovedExams = 0;
    // For each period do
    for (int pi = 0; pi < timetableCont.getNumPeriods(); ++pi) {
        // For each exam of period do
        for (int ei = 0; ei < getNumExams(); ++ei) {
            if (timetableCont.isExamScheduled(ei, pi)) {
                // Sum proximity conflicts of this exam
                long sourceExamProximityConflicts = getExamProximityConflicts(ei, pi);
                examProximityConflictsOfRemovedExams += sourceExamProximityConflicts;
                // Remove the exam from the timetable
                timetableCont.removeExam(ei, pi);

                // Insert in the auxiliary timetable
                auxTimetable.insertExam(ei, pi);
            }
        }
    }
    cout << "Accumulated costs after removing = " << examProximityConflictsOfRemovedExams << endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////// TEST - INSERT ALL EXAMS //////////////////////////////////////////////////
    /// After removing all exams, now insert them again.
    /// Accumulated costs should be equal to original cost
    ///
    long examProximityConflictsOfInsertedExams = 0;
    // For each period do
    for (int pi = 0; pi < timetableCont.getNumPeriods(); ++pi) {
        // For each exam of period do
        for (int ei = 0; ei < getNumExams(); ++ei) {
            // Find an exam to schedule from the auxTimetable
            if (auxTimetable.isExamScheduled(ei, pi)) {
                // Insert the exam in the timetable
                timetableCont.insertExam(ei, pi);
                // Sum proximity conflicts of this exam
                long sourceExamProximityConflicts = getExamProximityConflicts(ei, pi);
                examProximityConflictsOfInsertedExams += sourceExamProximityConflicts;
            }
        }
    }
    cout << "Accumulated costs after inserting = " << examProximityConflictsOfInsertedExams << endl;


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cin.get();

}



/**
 * @brief computeExamProximityConflictsIncremental Compute chromosome's proximity cost
 * performing an incremental evaluation based on the Kempe chain information.
 *
 * Pre-condition: The solution passed in the <this> parameter is the one prior to move
 *
 * @param _kempeChain
 */
void eoChromosome::computeExamProximityConflictsIncremental(const ETTPKempeChain<eoChromosome> &_kempeChain) {
    // Source Ti and Tj exams
    auto const& srcTiExams = _kempeChain.getSourceTiPeriodExams();
    auto const& srcTjExams = _kempeChain.getSourceTjPeriodExams();
    // Dest Ti and Tj exams
    auto const& destTiExams = _kempeChain.getDestTiPeriodExams();
    auto const& destTjExams = _kempeChain.getDestTjPeriodExams();
    // The timetable container
    auto &timetableCont = getTimetableContainer();

#ifdef EOCHROMOSOME_INCREMENTAL
//    std::cout << "this == kempeChain.getSolution() ? " << (this == &_kempeChain.getSolution() ? "true" : "false") << std::endl;
//    std::cout << "Ti = " << _kempeChain.getTi() << std::endl;
//    std::cout << "Tj = " << _kempeChain.getTj() << std::endl;

//    /// Source period exams //////////////////////////
//    std::cout << "source Ti period size = " << timetableCont.getPeriodSize(_kempeChain.getTi()) << std::endl;
//    std::cout << "source Ti period exams: ";
//    // For each exam of period do
//    for (int ei = 0; ei < getNumExams(); ++ei) {
//        if (srcTiExams[ei] == SCHED_EXAM)
//            std::cout << ei << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "Actual timetable Ti period exams: ";
//    // For each exam of period do
//    for (int ei = 0; ei < getNumExams(); ++ei) {
//        if (timetableCont.isExamScheduled(ei, _kempeChain.getTi()))
//            std::cout << ei << " ";
//    }
//    std::cout << std::endl;

//    std::cout << "source Tj period size = " << timetableCont.getPeriodSize(_kempeChain.getTj()) << std::endl;
//    std::cout << "source Tj period exams: ";
//    // For each exam of period do
//    for (int ei = 0; ei < getNumExams(); ++ei) {
//        if (srcTjExams[ei] == SCHED_EXAM)
//            std::cout << ei << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "Actual timetable Tj period exams: ";
//    // For each exam of period do
//    for (int ei = 0; ei < getNumExams(); ++ei) {
//        if (timetableCont.isExamScheduled(ei, _kempeChain.getTj()))
//            std::cout << ei << " ";
//    }
//    std::cout << std::endl;

//    /// Dest period exams //////////////////////////
//    std::cout << "dest Ti period exams: ";
//    // For each exam of period do
//    for (int ei = 0; ei < getNumExams(); ++ei) {
//        if (destTiExams[ei] == SCHED_EXAM)
//            std::cout << ei << " ";
//    }
//    std::cout << std::endl;

//    std::cout << "dest Tj period exams: ";
//    // For each exam of period do
//    for (int ei = 0; ei < getNumExams(); ++ei) {
//        if (destTjExams[ei] == SCHED_EXAM)
//            std::cout << ei << " ";
//    }
//    std::cout << std::endl;
#endif



#ifdef EOCHROMOSOME_INCREMENTAL
    std::cout << "///////////////////////////////// Incremental eval method /////////////////////////////////////////" << std::endl;
    std::cout << "Original solution # proximity conflicts = " << getProximityConflicts() << std::endl;

    std::cout << "Computing (again) the solution full eval..." << std::endl;
    computeExamProximityConflicts();
    std::cout << "Original solution # proximity conflicts after full eval = " << getProximityConflicts() << std::endl;

#endif



    ////////////////////// Move Kempe chain exams between Ti and Tj ///////////////////////////////////////////////
    long examProximityConflictsOfRemovedExams = 0;
    long examProximityConflictsOfInsertedExams = 0;

    for (int ei = 0; ei < getNumExams(); ++ei) {
/// TODO - BUILD THE KEMPE CHAIN EXAMS WHEN BUILDING THE KEMPE CHAIN
///
        //
        // Move all *source* kempe chain exams and determine corresponding removal and insertion exam proximity costs
        //
        // Find if ei belongs to the Kempe chain in Ti, that is,
        // if it is to be moved to time slot Tj
        if (srcTiExams[ei] == SCHED_EXAM && destTjExams[ei] == SCHED_EXAM) {

/// TODO - Implement methods
///
///
            long sourceExamProximityConflictsRemoval = getExamProximityConflicts(ei, _kempeChain.getTi());
            // Sum proximity conflicts of this exam
            examProximityConflictsOfRemovedExams += sourceExamProximityConflictsRemoval;
            // Remove the exam from the timetable
            timetableCont.removeExam(ei, _kempeChain.getTi());

            // Now, insert the exam in the Tj period
            timetableCont.insertExam(ei, _kempeChain.getTj());
            // Determine the proximity conflicts
            long sourceExamProximityConflictsInsertion = getExamProximityConflicts(ei, _kempeChain.getTj());
            // Sum proximity conflicts of this exam
            examProximityConflictsOfInsertedExams += sourceExamProximityConflictsInsertion;
        }
        // Move all *dest* kempe chain dest exams and determine corresponding removal and insertion exam proximity costs
        //
        // Find if ei belongs to the Kempe chain in Tj, that is,
        // if it is to be moved to time slot Ti
        else if (srcTjExams[ei] == SCHED_EXAM && destTiExams[ei] == SCHED_EXAM) {
            long sourceExamProximityConflictsRemoval = getExamProximityConflicts(ei, _kempeChain.getTj());
            // Sum proximity conflicts of this exam
            examProximityConflictsOfRemovedExams += sourceExamProximityConflictsRemoval;
            // Remove the exam from the timetable
            timetableCont.removeExam(ei, _kempeChain.getTj());

            // Now, insert the exam in the Ti period
            timetableCont.insertExam(ei, _kempeChain.getTi());
            // Determine the proximity conflicts
            long sourceExamProximityConflictsInsertion = getExamProximityConflicts(ei, _kempeChain.getTi());
            // Sum proximity conflicts of this exam
            examProximityConflictsOfInsertedExams += sourceExamProximityConflictsInsertion;
        }
    }

    // Now subtract to the solution proximity conflicts the conflicts of the removed exams
    this->examProximityConflicts -= examProximityConflictsOfRemovedExams;
    // ... and add the conflicts of the inserted exams
    this->examProximityConflicts += examProximityConflictsOfInsertedExams;


//    validate();

}




/**
 * @brief eoChromosome::getExamProximityConflicts Determine the proximity conflicts of exam _ei in period _period
 * This is done by suming up all the edge costs of conflicting exams five time slots from left and
 * five time slots to right.
 * @param _ei
 * @param _period
 * @return
 */
long eoChromosome::getExamProximityConflicts(int _ei, int _pi) {
    long sourceExamProximityConflicts = 0;
    // Sum up all the edge costs of conflicting exams five time slots from left and
    // five time slots to right

    // Get exam graph reference
    AdjacencyList const &examGraph = this->getExamGraph();
    // Timetable container
    TimetableContainer const &timetableCont = (*this->timetableContainer.get());
    // Conflict matrix
    Matrix<int> const &conflictMatrix = this->getConflictMatrix();
    // Weights array
    int wi[] = { 16, 8, 4, 2, 1 };
    int pj, n;

/// TODO: USE GRAPH OR VECTOR INDEXING??
///
///
    // Get ei adjacent vertices
    property_map<AdjacencyList, vertex_index_t>::type index_map = get(vertex_index, examGraph);
    graph_traits<AdjacencyList>::adjacency_iterator ai, a_end;
    boost::tie(ai, a_end) = adjacent_vertices(_ei, examGraph);
    for (; ai != a_end; ++ai) {
        // Get adjacent exam
        int ej = get(index_map, *ai);
        // If exam ej is scheduled five periods apart (left or right),
        // sum the corresponding weighted proximity cost
        for (int i = 0; i <= NUM_PROXIMITY_PERIODS-1; ++i) {
            // Check to the right
            pj = _pi+i+1;
            if (pj < timetableCont.getNumPeriods()) {
                // If exam ej is scheduled in period pj
                if (timetableCont.isExamScheduled(ej, pj)) {
                    n = conflictMatrix.getVal(_ei, ej);
                    sourceExamProximityConflicts += wi[i]*n;
                    break; // Go to the next adjacent exam
                }
            }
            // If the exam ej was not in the right, check to the left
            pj = _pi-i-1;
            if (pj >= 0) {
                // If exam ej is scheduled in period pj
                if (timetableCont.isExamScheduled(ej, pj)) {
                    n = conflictMatrix.getVal(_ei, ej);
                    sourceExamProximityConflicts += wi[i]*n;
                    break; // Go to the next adjacent exam
                }
            }
        }
    }
    return sourceExamProximityConflicts;
}


bool eoChromosome::isExamFeasibleInPeriod(int _exam, int _period) const {
    //
    // Verify _exam feasibility within _period
    //
    // For each exam of period pi do
    for (int ei = 0; ei < getNumExams(); ++ei) {
        // If exam ei is scheduled in period
        if (timetableContainer->isExamScheduled(ei, _period)) {
            // Verify feasibility between exam ei and exam _exam
            // Obtain conflicts by consulting the conflicting matrix
            int n = getConflictMatrix().getVal(ei, _exam);
            if (n > 0) {
                return false;
            }
        }
    }
    return true;
}


/**
 * @brief operator <<
 * @param _os
 * @param _chrom
 * @return
 */
ostream& operator<<(ostream &_os, const eoChromosome &_chrom) {
    _os << endl << "Timetable" << endl;
    _os << endl;
    _os << " cost = " << _chrom.fitness() << " - periods = " << _chrom.getNumPeriods() << endl;

    _os << endl << ".Sol Data" << endl;

    // Timetable container
    TimetableContainer const &timetableCont = _chrom.getTimetableContainer();
    // For each period do
    for (int pi = 0; pi < timetableCont.getNumPeriods(); ++pi) {
        // For each exam of period pi do
        for (int ei = 0; ei < _chrom.getNumExams(); ++ei) {
            // If exam ei is scheduled in period pi
            if (timetableCont.isExamScheduled(ei, pi)) {
                // Print exams indexes starting at 1 (according to the R. Qu's Toronto validator)
                _os << (ei+1) << " " << pi << endl;
            }
        }
    }
    return _os;
}



void eoChromosome::printToFile(ostream &_os) {
    // Timetable container
    TimetableContainer const &timetableCont = getTimetableContainer();
    // For each period do
    for (int pi = 0; pi < timetableCont.getNumPeriods(); ++pi) {
        // For each exam of period pi do
        for (int ei = 0; ei < getNumExams(); ++ei) {
            // If exam ei is scheduled in period pi
            if (timetableCont.isExamScheduled(ei, pi)) {
                // Print exams indexes starting at 1 (according to the R. Qu's Toronto validator)
                _os << (ei+1) << " " << pi << endl;
            }
        }
    }
}




void eoChromosome::validate() const {

    cout << endl << "[validate() method]" << endl;

    // Count # exams, # enrolments, and verify exams unicity
    int numExams = 0, numEnrolments = 0;
    set<int> examsSet;
    bool unique = true;
    // Timetable container
    TimetableContainer const &timetableCont = this->getTimetableContainer();
    // Conflict matrix
    Matrix<int> const &conflictMatrix = this->getConflictMatrix();

    int numExamsInPeriods = 0;

    // For each period do
    for (int pi = 0; pi < timetableCont.getNumPeriods(); ++pi) {
        // Count # exams in period pi
        int numExamsPeriodPi = timetableCont.getPeriodSize(pi);
        int numExamsPeriodPi2 = 0;
        for (int exam = 0; exam < getNumExams(); ++exam) {
            if (timetableCont.isExamScheduled(exam, pi))
                ++numExamsPeriodPi2;
        }
//        cout << "pi = " << pi << ", # exams in period = " << numExamsPeriodPi << endl;
//        ////// Print period contents /////////////////////
//        cout << "pi period contents = ";
//        // For each exam of period pi do
//        for (int ei = 0; ei < getNumExams(); ++ei) {
//            // If exam ei is scheduled in period pi
//            if (timetableCont.isExamScheduled(ei, pi)) {
//                cout << ei << " ";
//            }
//        }
//        cout << endl;
        ////////////////////
        if (numExamsPeriodPi != numExamsPeriodPi2) {
            cout << "numExamsPeriodPi != numExamsPeriodPi2 " << endl
                 << "numExamsPeriodPi = " << numExamsPeriodPi << ", numExamsPeriodPi2 = "
                 << numExamsPeriodPi2 << endl;
            cin.get();
        }

        numExamsInPeriods += numExamsPeriodPi;
    }
    // Reset examsSet
    examsSet.clear();

    // For each period do
    for (int pi = 0; pi < timetableCont.getNumPeriods(); ++pi) {
        // For each exam of period pi do
        for (int ei = 0; ei < getNumExams(); ++ei) {
            // If exam ei is scheduled in period pi
            if (timetableCont.isExamScheduled(ei, pi)) {
                ++numExams;
                numEnrolments += this->getCourseStudentCounts()[ei];
                // Verify unicity of exams
                if (examsSet.find(ei) != examsSet.end()) {
                    unique = false;
                    cout << "There are duplicated exams" << endl;
                    // Print period contents
                    cout << "Period pi=" << pi << " contents: ";
                    for (int exam : timetableCont.getPeriodExams(pi))
                        cout << exam << " ";
                    cout << endl;

                    cin.get();

                    return;
                }
                // Insert exam in examsSet
                examsSet.insert(ei);
            }
        }
    }

    cout << "numExams = " << numExams << endl;
    cout << "numExamsInPeriods = " << numExamsInPeriods << endl;
    cout << "numEnrolments = " << numEnrolments << endl << endl;
    cout << "Original values" << endl;
    cout << "numExams = " << this->getNumExams() << endl;
    cout << "numEnrolments = " << this->getNumEnrolments() << endl;

    if (numExams != this->getNumExams()) {
        cout << "numExams is different" << endl;
        cout << "counted: " << numExams << "; original: " <<  this->getNumExams() << endl;
        cin.get();
    }
    else if (numExams != numExamsInPeriods) {
        cout << "numExams != numExamsInPeriods" << endl;
        cout << "numExams: " << numExams << "; numExamsInPeriods: " << numExamsInPeriods << endl;
        cin.get();
    }
    else if (numEnrolments != this->getNumEnrolments()) {
        cout << "numEnrolments is different" << endl;
        cin.get();
    }

    //
    // Verify feasibility
    //
    // For each period do
    for (int pi = 0; pi < timetableCont.getNumPeriods(); ++pi) {
        //
        // Verify feasibility within each period
        //
        // For each exam of period pi do
        for (int ei = 0; ei < getNumExams(); ++ei) {
            // If exam ei is scheduled in period pi
            if (timetableCont.isExamScheduled(ei, pi)) {
                // Verify feasibility between exam ei and the others exams of period pi
                for (int ej = ei+1; ej < getNumExams(); ++ej) {
                    // If exam ej is scheduled in period pi
                    if (timetableCont.isExamScheduled(ej, pi)) {
                        // Obtain conflicts by consulting the conflicting matrix
                        int n = conflictMatrix.getVal(ei, ej);
                        if (n > 0) {
                            cout << endl << "Found period with non feasible exams" << endl;
                            cout << "ei = " << ei << " and ej = " << ej << " have " << n << " students in common" << endl;
                            cin.get();
                            cout << "Exiting..." << endl;
                            exit(-1);
                        }
                    }
                }
            }
        }
    }

}







