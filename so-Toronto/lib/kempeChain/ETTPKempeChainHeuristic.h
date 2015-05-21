#ifndef ETTPKEMPECHAINHEURISTIC_H
#define ETTPKEMPECHAINHEURISTIC_H

#include "eoFunctor.h"
#include "kempeChain/ETTPKempeChain.h"
#include <boost/unordered_set.hpp>
#include <utils/eoRNG.h>
#include "utils/Common.h"
#include "containers/Matrix.h"

#include <stdexcept>


/// BECAUSE OF SCH_EXAM
///
#include "containers/TimetableContainerMatrix.h"

// For debugging purposes
//#define ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG

//#define ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG_INCREMENTAL



//////////////////////////////////////////////////////////
//
// (rf. Demeester paper)
// Concerning the uncapacitated Toronto examination time-
// tabling problem, we apply the Kempe chain based heuris-
// tics. These low-level heuristics perturb feasible solutions
// to the uncapacitated examination timetabling problem,
// without making them infeasible. Suppose a partial solu-
// tion that corresponds to the left hand side of Fig. 1. If
// we want to move exam e1 to time slot tj, the solution
// becomes infeasible, since exam e1 has students in com-
// mon with exams e6, e7, and e8 that are assigned to time
// slot tj. To overcome this, exams e6, e7, and e8 should be
// moved to time slot ti. This process is repeated until all the
// exams that have students in common are assigned to dif-
// ferent time slots. The result is depicted at the right hand
// side of Fig. 1. The Kempe Chain heuristic Uncap1 can be
// seen as moving an exam to another time slot whilst main-
// taining feasibility by repairing any infeasibilities that may
// have been introduced.
//
template <typename EOT>
class ETTPKempeChainHeuristic : public eoUF<EOT&, void> {

public:

    /**
     * @brief ETTPKempeChainHeuristic Constructor
     */
    ETTPKempeChainHeuristic() { }


    /**
     * @brief build Create a Kempe chain for a random move
     * @param _sol
     */
    virtual void build(EOT& _sol);

    /**
     * @brief operator () Apply Kempe chain move to solution _sol
     * @param _chrom
     */
    virtual void operator()(EOT& _sol);

    /**
     * @brief getKempeChain Return Kempe chain object
     * @return
     */
    const ETTPKempeChain<EOT> &getKempeChain() const;
    /**
     * @brief evaluateSolutionMove Evaluate move of solution to the neighbour.
     * This envolves doing:
     *   - move, temporarily, the solution to the neighbour
     *   - record ti and tj final time slots
     *   - evaluate neighbour and record neighbour fitness
     *   - undo solution move
     * @param _sol
     */
    void evaluateSolutionMove(EOT &_sol);

    /**
     * @brief undoSolutionMove Undo solution move
     * @param _sol
     */
    void undoSolutionMove(EOT &_sol);

    /**
     * @brief getNeighborFitness Get neighbor fitness
     * @return
     */
    double getNeighborFitness() const;

    /**
     * @brief getNeighborProximityConflicts
     * @return Return neighbor proximity conflicts
     */
    long getNeighborProximityConflicts() const;



protected:

    // Record source Ti and Tj period exams
    void recordSourcePeriodExams(int _ti, int _tj);
    // Record destination Ti and Tj period exams
    void recordDestPeriodExams(int _ti, int _tj);

    void apply();

    void moveExams(int _tSource,
                   std::vector<int> &_conflictingExamsTsource, int _tDest,
                   std::vector<int> &_conflictingExamsTdest);

    void getSourceExamConflictingVertices(int _examSource, AdjacencyList const &_examGraph,
                                          int _tDest, std::vector<int> &_conflictingExamsTdest);

    void getUniqueSourceExamConflictingVertices(int _examSource, AdjacencyList const &_examGraph,
                                                int _tDest, std::vector<int> &_conflictingExamsTdest,
                                                std::vector<bool> &_destTimeslotMarkings);


    /**
     * @brief kempeChain The kempe chain relating the current solution to the neighbour solution
     */
    ETTPKempeChain<EOT> kempeChain;
    /**
     * @brief neigborFitness Neighbor fitness
     */
    double neighborFitness;
    /**
     * @brief neighborExamProximityConflicts Sum of exam proximity conflicts
     */
    long neighborExamProximityConflicts;
};




/**
 * @brief build Create a Kempe chain for a random move
 * @param _sol
 */
template <typename EOT>
void ETTPKempeChainHeuristic<EOT>::build(EOT &_sol)
{

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
    std::cout << "In [ETTPKempeChainHeuristic<EOT>::build()]:" << std::endl;
#endif


    int exami = -1; // Selected exam to move
    int ti, tj; // Source and destination time slots, respectively
    // Select randomly two time slots, ti and tj.
    do {
        ti = rng.random(_sol.getNumPeriods());
        do {
            tj = rng.random(_sol.getNumPeriods());
        }
        while (ti == tj);
    }
    while (_sol.getTimetableContainer().getPeriodSize(ti) == 0); // Repeat until we found a non-empty time slot

    // Initialise Kempe chain object
    kempeChain.setSolution(&_sol);
    kempeChain.setTi(ti);
    kempeChain.setTj(tj);


//#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << "In [ETTPKempeChainHeuristic<EOT>::build()]:" << std::endl;
//    std::cout << "ti = " << ti << ", tj = " << tj << std::endl;
//    std::cout << "////////////////////////////////////////////////////////////////////////////" << std::endl;
//    std::cout << "All periods" << std::endl;
//    // For each period do
//    for (int pi = 0; pi < _sol.getTimetableContainer().getNumPeriods(); ++pi) {
//        std::cout << "time slot pi = " << pi << ": ";
//        // For each exam of period do
//        for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
//            if (_sol.getTimetableContainer().isExamScheduled(ei, pi))
//                std::cout << ei << " ";
//        }
//        std::cout << std::endl;
//    }
//#endif

    // Record source Ti and Tj period exams
    recordSourcePeriodExams(ti, tj);



//#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << "////////////////////////////////////////////////////////////////////////////" << std::endl;
//    std::cout << "Ti period contents" << std::endl;
//    // Print source Ti and Tj exams
//    auto const& srcTiExams = kempeChain.getSourceTiPeriodExams();
//    for (int ei : srcTiExams) {
//        std::cout << ei << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "Tj period contents" << std::endl;
//    auto const& srcTjExams = kempeChain.getSourceTjPeriodExams();
//    for (int ei : srcTjExams) {
//        std::cout << ei << " ";
//    }
//    std::cout << std::endl;
//#endif


    //
    // Select randomly an exam ei from timeslot ti and move it
    // to timeslot tj. If the solution becomes infeasible,
    // because exam ei has students in common with exams ej, ek, ...,
    // that are assigned to time slot tj, one have to move exams
    // ej, ek, ..., to time slot ti. This process is repeated until all the
    // exams that have students in common are assigned to different time slots.
    //

    // Generate random exam ei index
    int randIdx = rng.random(_sol.getTimetableContainer().getPeriodSize(ti));
/////////////////////////////////////////////////////////////////////////////////////////



//#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << "After generating random index in [ETTPKempeChainHeuristic<EOT>::build()]:" << std::endl;
//    std::cout << "ti = " << ti << ", _sol.getTimetableContainer().getPeriodSize(ti) = " << _sol.getTimetableContainer().getPeriodSize(ti) << std::endl;
//    std::cout << "Random index = " << randIdx << std::endl;

//#endif

    //
    // Get exam id
    //
    // Timetable container
    TimetableContainer const &timetableCont = _sol.getTimetableContainer();

///
/// O(NumLines) to find random exam index
///
    int scheduledExamIndex = 0;
    // For each exam of period pi do
    for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
        // If exam ei is scheduled in period ti
        if (timetableCont.isExamScheduled(ei, ti)) {
            if (scheduledExamIndex == randIdx) { // Found random scheduled exam index
                // Set exami index
                exami = ei;
                break;
            }
            ++scheduledExamIndex;
        }
    }

    if (exami == -1) {
        throw std::runtime_error("Selecting random exam in ETTPKempeChainHeuristic::build failed, exami == -1");
    }
///////////////////////////////////////////

    // Initialise Kempe chain object
    kempeChain.setEi(exami);


#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
    std::cout << "In [ETTPKempeChainHeuristic<EOT>::build()]:" << std::endl;
    std::cout << "sol.getProximityConflicts() = " << kempeChain.getSolution().getProximityConflicts() << std::endl <<
    std::cout << "sol.fitness() = " << kempeChain.getSolution().fitness() << std::endl <<
                 "ei = " << kempeChain.getEi() << std::endl <<
                 "ti = " << kempeChain.getTi() << ", tj = " << kempeChain.getTj() << std::endl <<
                 "_chrom.getNumPeriods() = " << kempeChain.getSolution().getNumPeriods() << std::endl;
#endif
}





/**
 * @brief operator() Move current solution _sol to this neighbour using
 * information maintained in the KempeChain structure
 */
template <typename EOT>
void ETTPKempeChainHeuristic<EOT>::operator()(EOT & _sol) {

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
    std::cout << "Entering apply()..." << std::endl;
#endif

    // Apply Kempe chain heuristic to move exami from time slot ti to time slot tj,
    // in a feasible way
    apply();


#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
    std::cout << "After apply()" << std::endl;
#endif

}




// Record source Ti and Tj period exams
template <typename EOT>
void ETTPKempeChainHeuristic<EOT>::recordSourcePeriodExams(int _ti, int _tj) {
    // Timetable container
    auto &timetableContainer = kempeChain.getSolution().getTimetableContainer();
    // Instantiate unordered_sets for recording source ti an tj contents
    std::vector<int> sourceTiPeriodExams = timetableContainer.getPeriodExams(_ti); // Copy
    std::vector<int> sourceTjPeriodExams = timetableContainer.getPeriodExams(_tj); // Copy

    // Record source ti an tj contents in kempe chain data structure
    kempeChain.setSourceTiPeriodExams(sourceTiPeriodExams);
    kempeChain.setSourceTjPeriodExams(sourceTjPeriodExams);


//#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << "sourceTiPeriodExams.size() = " << sourceTiPeriodExams.size() << std::endl;
//    std::cout << "sourceTjPeriodExams.size() = " << sourceTjPeriodExams.size() << std::endl;
//#endif
}


// Record destination Ti and Tj period exams
template <typename EOT>
void ETTPKempeChainHeuristic<EOT>::recordDestPeriodExams(int _ti, int _tj) {
    // Timetable container
    auto &timetableContainer = kempeChain.getSolution().getTimetableContainer();

    // Instantiate unordered_sets for recording source ti an tj contents
    std::vector<int> destTiPeriodExams = timetableContainer.getPeriodExams(_ti); // Copy
    std::vector<int> destTjPeriodExams = timetableContainer.getPeriodExams(_tj); // Copy

    // Record destination ti an tj contents in kempe chain data structure
    kempeChain.setDestTiPeriodExams(destTiPeriodExams);
    kempeChain.setDestTjPeriodExams(destTjPeriodExams);
}




template <typename EOT>
void ETTPKempeChainHeuristic<EOT>::apply() {
    // Queue containing conflicting exams in time slot ti
    vector<int> conflictingExamsTi;
    // Queue containing conflicting exams in time slot tj
    vector<int> conflictingExamsTj;
    // Push exami into the conflictingExamsTi queue
    conflictingExamsTi.push_back(kempeChain.getEi());
    // Get solution reference
    EOT &sol = this->kempeChain.getSolution();
    // Timetable container
    auto &timetableContainer = sol.getTimetableContainer();

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
    std::cout << "Ei = " << kempeChain.getEi() << std::endl;
    std::cout << "Ti = " << kempeChain.getTi() << std::endl;
    std::cout << "Tj = " << kempeChain.getTj() << std::endl;
    std::cout << "////////////////////////////////////////////////////////////////////////////" << std::endl;
    std::cout << "All periods" << std::endl;
    // For each period do
    for (int pi = 0; pi < timetableContainer.getNumPeriods(); ++pi) {
        std::cout << "time slot pi = " << pi << ": ";
        // For each exam of period do
        for (int ei = 0; ei < sol.getNumExams(); ++ei) {
            if (timetableContainer.isExamScheduled(ei, pi))
                std::cout << ei << " ";
        }
        std::cout << std::endl;
    }
#endif

    // Move exams between time slots in order to maintain feasibility
    moveExams(kempeChain.getTi(), conflictingExamsTi,
              kempeChain.getTj(), conflictingExamsTj);

}



template <typename EOT>
void ETTPKempeChainHeuristic<EOT>::moveExams(int _tSource, std::vector<int> &_conflictingExamsTsource,
                                             int _tDest, std::vector<int> &_conflictingExamsTdest) {

    // Get solution reference
    EOT &sol = this->kempeChain.getSolution();
    // Get exam graph reference
    AdjacencyList const &examGraph = sol.getExamGraph();
    // Timetable container
    TimetableContainer const &timetableCont = sol.getTimetableContainer();
    // Boolean vector for marking visited exams
    std::vector<bool> sourceTimeslotMarkings(sol.getNumExams());

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
    std::cout << "timetableCont.getNumExams() = " << sol.getNumExams() << std::endl;
    std::cout << "Initial periods" << std::endl;
    std::cout << "_tSource = " << _tSource << " time slot: ";
    // For each exam of period do
    for (int ei = 0; ei < sol.getNumExams(); ++ei) {
        if (timetableCont.isExamScheduled(ei, _tSource))
            std::cout << ei << " ";
    }

    std::cout << std::endl << "_tDest = " << _tDest << " time slot: ";
    // For each exam of period do
    for (int ei = 0; ei < sol.getNumExams(); ++ei) {
        if (timetableCont.isExamScheduled(ei, _tDest))
            std::cout << ei << " ";
    }
    std::cout << std::endl;
#endif

    while (!_conflictingExamsTsource.empty()) {
        // Get source exam from conflict vector
        int _examSource = _conflictingExamsTsource.back();

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
    std::cout << "_examSource = " << _examSource << std::endl;
#endif

        // Get source exam adjacent vertices that are scheduled in tDest time slot
        getSourceExamConflictingVertices(_examSource, examGraph, _tDest, _conflictingExamsTdest);
        ////////////////////////////////////////////////////////////////////////////////
        //
        // Move sourceExam to time slot tDest
        //
        // Remove exam from source time slot
        this->kempeChain.getSolution().getTimetableContainer().removeExam(_examSource, _tSource);
        // Insert exam examSource in time slot tDest
        this->kempeChain.getSolution().getTimetableContainer().insertExam(_examSource, _tDest);
        // Remove sourceExam from source conflicting exams
        _conflictingExamsTsource.pop_back();
        // Mark source exam as visited
        sourceTimeslotMarkings[_examSource] = true;
        ////////////////////////////////////////////////////////////////////////////////
        // Get exams in the source time slot that conflict with _conflictingExamsTdest exams
        for (int examDest : _conflictingExamsTdest) {
            getUniqueSourceExamConflictingVertices(examDest, examGraph, _tSource,
                                                   _conflictingExamsTsource, sourceTimeslotMarkings);
        }

        // Move conflicting exams from dest time slot to source time slot
        while (_conflictingExamsTdest.size() > 0) {
            int examDest = _conflictingExamsTdest.back();
            // Remove examDest from _conflictingExamsTdest
            _conflictingExamsTdest.pop_back();
            // Remove exam from dest time slot
            this->kempeChain.getSolution().getTimetableContainer().removeExam(examDest, _tDest);
            // Insert exam examDest in time slot tSource
            this->kempeChain.getSolution().getTimetableContainer().insertExam(examDest, _tSource);
            // Mark dest exam as visited
            sourceTimeslotMarkings[examDest] = true;
        }

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
    std::cout << "After moving source exam = " << _examSource << std::endl;
    std::cout << "_tSource time slot: " << std::endl;
    // For each exam of period do
    for (int ei = 0; ei < sol.getNumExams(); ++ei) {
        if (timetableCont.isExamScheduled(ei, _tSource))
            std::cout << ei << " ";
    }

    std::cout << std::endl << "_tDest time slot: " << std::endl;
    // For each exam of period do
    for (int ei = 0; ei < sol.getNumExams(); ++ei) {
        if (timetableCont.isExamScheduled(ei, _tDest))
            std::cout << ei << " ";
    }
    std::cout << std::endl;
#endif

    }
}



template <typename EOT>
void ETTPKempeChainHeuristic<EOT>
    ::getSourceExamConflictingVertices(int _examSource, AdjacencyList const &_examGraph,
                                       int _tDest, std::vector<int> &_conflictingExamsTdest) {


//#ifdef IMPLEMENTATION_GRAPH_BASED
//    property_map<AdjacencyList, vertex_index_t>::type index_map = get(vertex_index, _examGraph);
//    graph_traits<AdjacencyList>::adjacency_iterator ai, a_end;
//    boost::tie(ai, a_end) = adjacent_vertices(_examSource, _examGraph);
//    for (; ai != a_end; ++ai) {
//        // Get adjacent exam
//        int examDest = get(index_map, *ai);
//        // If exam examDest is scheduled in time slot tDest,
//        // add examDest to _conflictingExamsTdest queue
//        if (this->kempeChain.getSolution().getTimetableContainer().isExamScheduled(examDest, _tDest)) {
//            // Exam _examDest in time slot _tDest have conflicts with _examSource in slot _tSource
//            // so add examDest to _conflictingExamsTdest queue
//            _conflictingExamsTdest.push_back(examDest);
//        }
//    }
//#else

    auto const& sol = this->kempeChain.getSolution();
    auto const& conflictMatrix = sol.getConflictMatrix();
    auto const& timetableCont = sol.getTimetableContainer();
    // For each adjacent examDest of _examSource do
    for (int examDest = 0; examDest < sol.getNumExams(); ++examDest) {
        // If exam examDest is scheduled in time slot tDest and has conflicts with _examSource, then it is adjacent
        // add examDest to _conflictingExamsTdest queue
        if (timetableCont.isExamScheduled(examDest, _tDest)
                && conflictMatrix.getVal(_examSource, examDest) > 0) {
            // Exam _examDest in time slot _tDest have conflicts with _examSource in slot _tSource
            // so add examDest to _conflictingExamsTdest queue
            _conflictingExamsTdest.push_back(examDest);
        }
    }
//#endif

}


template <typename EOT>
void ETTPKempeChainHeuristic<EOT>
    ::getUniqueSourceExamConflictingVertices(int _examDest, AdjacencyList const &_examGraph,
                                             int _tSource, std::vector<int> &_conflictingExamsTsource,
                                             std::vector<bool> &_sourceTimeslotMarkings) {

//    property_map<AdjacencyList, vertex_index_t>::type index_map = get(vertex_index, _examGraph);
//    graph_traits<AdjacencyList>::adjacency_iterator ai, a_end;
//    boost::tie(ai, a_end) = adjacent_vertices(_examDest, _examGraph);

//#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << "[getUniqueSourceExamConflictingVertices() method] Obtaining adjacent vertices" << std::endl;
//#endif

//    for (; ai != a_end; ++ai) {
//        // Get adjacent exam
//        int examSource = get(index_map, *ai);

//#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//        std::cout << "adjacent vertex: " << examSource << std::endl;
//#endif

//        // If exam examSource is scheduled in time slot tSource,
//        // add examSource to _conflictingExamsTsource queue
//        if (this->kempeChain.getSolution().getTimetableContainer().isExamScheduled(examSource, _tSource)) {
//            // Verify if examSource isn't inserted already
//            if (_sourceTimeslotMarkings[examSource] == false) {
//                 // Mark source exam as visited
//                _sourceTimeslotMarkings[examSource] = true;
//                _conflictingExamsTsource.push_back(examSource);

//#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//        std::cout << "\tInsert this vertex: " << examSource << std::endl;
//#endif

//            }
//        }
//    }
//#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << "_conflictingExamsTsource contents: " << std::endl;
//    std::copy(_conflictingExamsTsource.begin(),
//              _conflictingExamsTsource.end(), ostream_iterator<int>(std::cout, " "));
//#endif


    auto const& sol = this->kempeChain.getSolution();
    auto const& conflictMatrix = sol.getConflictMatrix();
    auto const& timetableCont = sol.getTimetableContainer();
    // For each adjacent examSource of _examDest do
    for (int examSource = 0; examSource < sol.getNumExams(); ++examSource) {
        // If exam examSource is scheduled in time slot tSource and has conflicts with _examSource, then it is adjacent
        // add examSource to _conflictingExamsTsource queue
        if (timetableCont.isExamScheduled(examSource, _tSource)
                && conflictMatrix.getVal(examSource, _examDest) > 0) {
            // Verify if examSource isn't inserted already
            if (_sourceTimeslotMarkings[examSource] == false) {
                 // Mark source exam as visited
                _sourceTimeslotMarkings[examSource] = true;
                _conflictingExamsTsource.push_back(examSource);
            }
        }
    }

}


template <typename EOT>
ETTPKempeChain<EOT> const &ETTPKempeChainHeuristic<EOT>::getKempeChain() const
{
    return kempeChain;
}


/**
 * @brief evaluateSolutionMove Evaluate move of solution to the neighbour.
 * This envolves doing:
 *   - move, temporarily, the solution to the neighbour
 *   - record ti and tj final time slots
 *   - evaluate neighbour and record neighbour fitness
 *   - undo solution move
 * @param _sol
 */
template <typename EOT>
void ETTPKempeChainHeuristic<EOT>::evaluateSolutionMove(EOT &_sol) {

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG_INCREMENTAL
    long solCostBeforeMove = _sol.getProximityConflicts();
    std::cout << std::endl << "[Before apply()] solution proximity cost full eval = "
              << solCostBeforeMove << std::endl;
    _sol.computeExamProximityConflicts();
    std::cout << std::endl << "solution proximity cost after *computing a full eval* = "
              << _sol.getProximityConflicts() << std::endl;
#endif

    // Get TimetableContainer object
    TimetableContainer & timetableCont = _sol.getTimetableContainer();

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
    std::cout << std::endl;
    std::cout << "Start of evaluateSolutionMove" << std::endl;
    std::cout << "////////////////////////////////////////////////////////////////////////////" << std::endl;
    std::cout << "All periods" << std::endl;
    // For each period do
    for (int pi = 0; pi < _sol.getNumPeriods(); ++pi) {
        std::cout << "time slot pi = " << pi << ": ";
        // For each exam of period do
        for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
            if (timetableCont.isExamScheduled(ei, pi))
                std::cout << ei << " ";
        }
        std::cout << "; time slot pi size = " << timetableCont.getPeriodSize(pi);
        std::cout << std::endl;
    }
#endif

    // Get original solution exam proximity conflicts prior moving the solution
    long solutionOriginalExamProximityConflicts = _sol.getProximityConflicts();
    // Move, temporarily, the solution to the neighbour
    // _sol was previously recorded in the kempe chain, so we don't pass it as argument
    apply();
    // Record ti and tj final time slots
    recordDestPeriodExams(kempeChain.getTi(), kempeChain.getTj());


#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG_INCREMENTAL
    long solCostAfterMove = _sol.getProximityConflicts();
    std::cout << std::endl << "[After apply()] solution proximity cost full eval = "
              << solCostAfterMove << std::endl;
#endif


    //
    // Evaluate neighbour (incremental evaluation) and record neighbour fitness
    //

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG_INCREMENTAL
    // Compute new solution exam proximity conflicts considering the move to the neighbor
    _sol.computeExamProximityConflicts();
    solCostAfterMove = _sol.getProximityConflicts();
    std::cout << std::endl << "solution proximity cost after *computing a full eval* = "
              << solCostAfterMove << std::endl;
#endif

    // Undo solution move before computeExamProximityConflictsIncremental() - pre-condition
    undoSolutionMove(_sol);
    _sol.setExamProximityConflicts(solutionOriginalExamProximityConflicts);

//#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG_INCREMENTAL
//    std::cout << std::endl << "In method [ETTPKempeChainHeuristic::evaluateSolutionMove] -> After calling undoSolutionMove(_sol); " << std::endl
//              << "Now calling _sol.computeExamProximityConflictsIncremental(kempeChain);" << std::endl;
//#endif

    // Compute new solution exam proximity conflicts considering the move to the neighbor
    //    Perform an incremental evaluation
    _sol.computeExamProximityConflictsIncremental(kempeChain);

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG_INCREMENTAL
//    std::cout << std::endl << "solution proximity cost full eval = "
//              << solCostAfterMove << std::endl;

    std::cout << std::endl << "solution proximity cost incremental eval = "
              << _sol.getProximityConflicts() << std::endl;

    cin.get();
#endif

//    _sol.validate();


    // Record neighbour fitness
    neighborFitness = _sol.getProximityCost();
    // Record neighbour exam proximity conflicts
    neighborExamProximityConflicts = _sol.getProximityConflicts();

    // Undo solution move
    undoSolutionMove(_sol);
    // Reset exam proximity conflicts
    _sol.setExamProximityConflicts(solutionOriginalExamProximityConflicts);


#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG_INCREMENTAL
//    std::cout << std::endl << "End of evaluateSolutionMove" << std::endl;
//    std::cout << "////////////////////////////////////////////////////////////////////////////" << std::endl;
//    std::cout << "All periods" << std::endl;
//    // For each period do
//    for (int pi = 0; pi < _sol.getNumPeriods(); ++pi) {
//        std::cout << "time slot pi = " << pi << ": ";
//        // For each exam of period do
//        for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
//            if (timetableCont.isExamScheduled(ei, pi))
//                std::cout << ei << " ";
//        }
//        std::cout << "; time slot pi size = " << timetableCont.getPeriodSize(pi);
//        std::cout << std::endl;
//    }

//    cin.get();
#endif

}



template <typename EOT>
void ETTPKempeChainHeuristic<EOT>::undoSolutionMove(EOT &_sol) {

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << std::endl << "validate solution before undoSolutionMove " << std::endl;
//    _sol.validate();
#endif

    // Get timetable container
    auto & timetableCont = _sol.getTimetableContainer();

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << std::endl;
//    std::cout << "undoSolutionMove " << std::endl;
//    std::cout << "Ti = " << kempeChain.getTi() << std::endl;
//    std::cout << "Tj = " << kempeChain.getTj() << std::endl;
//    std::cout << "////////////////////////////////////////////////////////////////////////////" << std::endl;
//    std::cout << "All periods" << std::endl;

//    // For each period do
//    for (int pi = 0; pi < _sol.getNumPeriods(); ++pi) {
//        std::cout << "time slot pi = " << pi << ": ";
//        // For each exam of period do
//        for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
//            if (timetableCont.isExamScheduled(ei, pi))
//                std::cout << ei << " ";
//        }
//        std::cout << std::endl;
//    }
#endif

    // Undo solution move by fill solution time slots ti and tj to source values
    // Remove Ti actual contents
    timetableCont.removeAllPeriodExams(kempeChain.getTi());
    // Remove Tj actual contents
    timetableCont.removeAllPeriodExams(kempeChain.getTj());

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << std::endl;
//    std::cout << "undoSolutionMove - After removing all Ti and Tj periods exams " << std::endl;
//    std::cout << "////////////////////////////////////////////////////////////////////////////" << std::endl;
//    std::cout << "All periods" << std::endl;
//    // For each period do
//    for (int pi = 0; pi < _sol.getNumPeriods(); ++pi) {
//        std::cout << "time slot pi = " << pi << ": ";
//        // For each exam of period do
//        for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
//            if (timetableCont.isExamScheduled(ei, pi))
//                std::cout << ei << " ";
//        }
//        std::cout << std::endl;
//    }
#endif

    // Copy source Ti and Tj exams
    auto const& srcTiExams = kempeChain.getSourceTiPeriodExams();
    auto const& srcTjExams = kempeChain.getSourceTjPeriodExams();

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << std::endl << "Source Ti exams " << std::endl;
//    // For each exam of period do
//    for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
//        if (srcTiExams[ei] == SCHED_EXAM)
//            std::cout << ei << " ";
//    }
//    std::cout << std::endl;

//    std::cout << std::endl << "Source Tj exams " << std::endl;
//    // For each exam of period do
//    for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
//        if (srcTjExams[ei] == SCHED_EXAM)
//            std::cout << ei << " ";
//    }
//    std::cout << std::endl;
#endif


    for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
        // Copy Ti contents
        //
        // If there's an exam, insert it
        if (srcTiExams[ei] == SCHED_EXAM)
            timetableCont.insertExam(ei, kempeChain.getTi());
        // Copy Tj contents
        //
        // If there's an exam, insert it
        if (srcTjExams[ei] == SCHED_EXAM)
            timetableCont.insertExam(ei, kempeChain.getTj());
    }


#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << std::endl;
//    std::cout << "undoSolutionMove - After inserting source Ti and Tj periods exams " << std::endl;
//    std::cout << "////////////////////////////////////////////////////////////////////////////" << std::endl;
//    std::cout << "All periods" << std::endl;
//    // For each period do
//    for (int pi = 0; pi < _sol.getNumPeriods(); ++pi) {
//        std::cout << "time slot pi = " << pi << ": ";
//        // For each exam of period do
//        for (int ei = 0; ei < _sol.getNumExams(); ++ei) {
//            if (timetableCont.isExamScheduled(ei, pi))
//                std::cout << ei << " ";
//        }
//        std::cout << "; time slot pi size = " << timetableCont.getPeriodSize(pi);
//        std::cout << std::endl;
//    }
#endif

#ifdef ETTP_KEMPE_CHAIN_HEURISTIC_DEBUG
//    std::cout << std::endl << "validate solution after undoSolutionMove " << std::endl;
//    _sol.validate();
#endif

}


template <typename EOT>
double ETTPKempeChainHeuristic<EOT>::getNeighborFitness() const {
    return neighborFitness;
}


/**
 * @brief getNeighborProximityConflicts
 * @return Return neighbor proximity conflicts
 */
template <typename EOT>
long ETTPKempeChainHeuristic<EOT>::getNeighborProximityConflicts() const {
    return neighborExamProximityConflicts;
}


#endif // ETTPKEMPECHAINHEURISTIC_H





