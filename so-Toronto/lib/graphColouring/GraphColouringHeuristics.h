#ifndef GRAPH_COLOURING_HEURISTIC
#define GRAPH_COLOURING_HEURISTIC

#include <boost/graph/adjacency_list.hpp>
#include "utils/Common.h"
#include "containers/VertexPriorityQueue.h"
#include "containers/TimetableContainer.h"
#include "containers/TimetableContainerMatrix.h"
#include "data/TimetableProblemData.hpp"
#include <utils/eoRNG.h>


using namespace std;


//#define GRAPH_COLOURING_HEURISTIC_DEBUG
//#define GRAPH_COLOURING_HEURISTIC_DEBUG_1


extern RNGType myRandom;


template <typename EOT>
class GCHeuristics {

public:
    static void saturationDegree(TimetableProblemData const& _timetableProblemData, EOT& _chrom);

};




template <typename EOT>
void GCHeuristics<EOT>::saturationDegree(TimetableProblemData const& _timetableProblemData, EOT& _chrom) {
    //===
    // SD (Saturation Degree) graph colouring heuristic
    //
    // 1. Initially, create a Priority queue with all exams inserted randomly.
    //    Set available periods of all unscheduled exams equal to T, where T is the maximum number of periods.
    // 2. If the queue is not empty, pop exam 'ei' from the top of exam's priority queue and schedule it into
    //    a random available period 'tj'.
    //    2.1. Mark period 'tj' of all the exams connected to 'ei' as unvailable, because they have common registered students.
    //         The number of available periods is the priority used to sort the priority queue of unscheduled exams.
    //    2.2 If there are no available periods for the current exam, set the solution as unfeasible and Stop.
    // 3. Goto step 2.
    //===


#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
    cout << "GCHeuristics::saturationDegree method" << endl;
#endif

    ///
    /// We use the ParadisEO random generator eoRng which is reseed on initialization.
    ///
    /// Initialise EO random generator
    rng.reseed(static_cast<uint32_t>(time(0)));

    // Chromosome's timetable
    TimetableContainer& timetableContainer = _chrom.getTimetableContainer();
    //
    // 1. Initially, create a Priority queue with all exams inserted randomly.
    //    Set available periods of all unscheduled exams equal to T, where T is the max. number of periods.
    //
    // N is the number of vertices
    const int NUM_EXAMS = _chrom.getNumExams();
    VertexPriorityQueue pq(NUM_EXAMS);
    // Number of periods
    const int NUM_PERIODS = _chrom.getNumPeriods();
#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
    cout << "NUM_EXAMS = " << NUM_EXAMS << endl;
    cout << "NUM_PERIODS = " << NUM_PERIODS << endl;
#endif
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Insert exams randomly into the priority queue
    //
    // Exam's vector. Exams start at index 0.
    vector<int> exams(NUM_EXAMS);
    for (int ei = 0; ei < NUM_EXAMS; ++ei)
        exams[ei] = ei;
    // Scheduled exams
    vector<bool> scheduledExams(NUM_EXAMS); // Initially, no exam is scheduled
    // Shuffle exams
//    std::srand(std::time(0)); // Seed random generator used in random_shuffle
    std::random_shuffle(exams.begin(), exams.end());

#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
    cout << "Random exams to insert into the priority queue: " << endl;
    copy(exams.begin(), exams.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
#endif

    // Insert them in the priority queue
    for (int i = 0; i < NUM_EXAMS; ++i) {
        pq.push(exams[i], NUM_PERIODS);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Available period's list
    vector<vector<int> > examsAvailablePeriodsList(NUM_EXAMS);
    vector<int> allPeriods(NUM_PERIODS);
    for (int p = 0; p < NUM_PERIODS; ++p)
        allPeriods[p] = p;

#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
    cout << "allPeriods vector: ";
    copy(allPeriods.begin(), allPeriods.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
#endif

    // Set available periods of all unscheduled exams equal to T, where T is the max. number of periods.
    for (int exam_i = 0; exam_i < NUM_EXAMS; ++exam_i)
        examsAvailablePeriodsList[exam_i] = allPeriods;

    while (!pq.empty()) {
        // 2. Pop exam 'ei' from the top of exam's priority queue and schedule it into a random period 'tj'.
        VertexPriorityQueue::heap_data data = pq.top();
        int ei = data.vertex;
        int numAvailablePeriods = data.priority;
        pq.pop();

#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
        cout << "ei = " << ei << ", availablePeriods = " << numAvailablePeriods << endl;
#endif
        int tj = -1; // Invalid value
        // Test if therea any available # periods
        if (numAvailablePeriods > 0) {
            // Schedule it into a random available period 'tj'.
            //
            // Get available period's list
            vector<int> const &availablePeriodsList = examsAvailablePeriodsList[ei];

            // Generate random number in the range [0, numAvailablePeriods)
            int index = rng.uniform(numAvailablePeriods);
            tj = availablePeriodsList[index];

//            // Random generator
//            boost::random::mt19937 myRng;
//            std::srand(std::time(0)); // Seed random generator
//            boost::random::uniform_int_distribution<> periodsDist(1, numAvailablePeriods); // period distribution
//            int index = periodsDist(myRng);
//            tj = availablePeriodsList[index-1];


#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
            cout << "index = " << index << ", tj = " << tj << endl;
#endif
            // Insert exam ei into period tj
            _chrom.getTimetableContainer().insertExam(ei, tj);


#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG_1
    std::cout << "In [GraphColouringHeuristic] method - After insert exam ei=" << ei << " into period tj=" << tj << std::endl;
    std::cout << "////////////////////////////////////////////////////////////////////////////" << std::endl;
    std::cout << "All periods" << std::endl;
    // For each period do
    for (int pi = 0; pi < _chrom.getTimetableContainer().getNumCols(); ++pi) {
        std::cout << "time slot pi = " << pi << ": ";
        // For each exam of period do
        for (int ei = 0; ei < _chrom.getTimetableContainer().getNumLines(); ++ei) {
            if (_chrom.isExamScheduled(ei, pi))
                std::cout << ei << " ";
        }
        std::cout << std::endl;
        cout << "periodsSizes[" << pi << "] = " << _chrom.getPeriodSize(pi) << endl;
    }

//    cin.get();
#endif



            //    2.1. Mark period 'tj' of all the exams connected to 'ei' as unvailable, because they have common registered students.
            //         The number of available periods is the priority used to sort the priority queue of unscheduled exams.

#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
            cout << "////////////////////////////////////////////////////////////////////////////////////////" << endl <<
                    "//" << endl <<
                    "// Get adjacent vertices" << endl <<
                    "////////////////////////////////////////////////////////////////////////////////////////" << endl;
#endif
            // Get adjacent vertices
            property_map<AdjacencyList, vertex_index_t>::type index_map = get(vertex_index, _chrom.getExamGraph());
            graph_traits<AdjacencyList>::adjacency_iterator ai, a_end;
            boost::tie(ai, a_end) = adjacent_vertices(ei, _chrom.getExamGraph());
            for (; ai != a_end; ++ai) {
                // Get adjacent exam
                int ej = get(index_map, *ai);

#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
                cout << "Adjacent vertex ej = " << ej << endl;
#endif
                if (!scheduledExams[ej])  {
#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
                    //cout << "ej = " << ej << ", tj = " << tj << endl;
                    cout << "Adjacent vertex ej = " << ej << " not scheduled yet" << endl;
#endif
                    // If exam ej was not scheduled yet, mark adjacent exam's tj period as unavailable.
                    // This is done by removing from available period's vector the period tj
                    bool periodRemoved = false;
                    // Find and remove period tj from ej's exams periods list
                    vector<int>::iterator it;
                    for (it = examsAvailablePeriodsList[ej].begin(); it != examsAvailablePeriodsList[ej].end(); ++it) {
                        if (*it == tj) {
#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
                            cout << "Size before removing = " << examsAvailablePeriodsList[ej].size() << endl;
#endif
                            // Remove period tj from ej available periods list
                            examsAvailablePeriodsList[ej].erase(it);

#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
                            cout << "Size after removing = " << examsAvailablePeriodsList[ej].size() << endl;
#endif
                            // Update exam ej priority
                            pq.update(ej, examsAvailablePeriodsList[ej].size());
                            periodRemoved = true;
                            break;
                        }
                    }

    //                if (!periodRemoved) {
    //                    cout << "In method [GCHeuristics<EOT>::saturationDegree] Period tj not removed -> ERROR?" << endl;
    ////                    cin.get();
    //                }

                    //copy(examsAvailablePeriodsList[ej].begin(), examsAvailablePeriodsList[ej].end(), ostream_iterator<int>(cout, " "));
                } // if (!scheduled_exams[ej])
            }
            // Set exam ei as scheduled
            scheduledExams[ei] = true;

#ifdef GRAPH_COLOURING_HEURISTIC_DEBUG
            cout << "Vertex ei = " << ei << " was scheduled" << endl;
#endif


        }
        else {
            //
            // 2.2  If there are no available periods for the current exam, set the solution as unfeasible and Stop.
            //
            // Set chromosome as an infeasible one
            _chrom.setFeasible(false);
            return;
        }

        //
        // 3. Goto step 2.
        //
    }
    // Set chromosome as a feasible one
    _chrom.setFeasible(true);
}








#endif
