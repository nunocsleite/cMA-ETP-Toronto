#ifndef EOCELLULARGA_H
#define EOCELLULARGA_H

#include <fstream>
//#include <eoContinue.h>
#include "algorithms/eo/eoGenerationContinuePopVector.h"
#include <eoEvalFunc.h>
#include <eoSelectOne.h>
#include "algorithms/eo/eoDeterministicTournamentSelectorPointer.h"
#include <eoPopEvalFunc.h>
//#include <eoAlgo.h>
#include "algorithms/eo/eoAlgoPointer.h"
#include <eoOp.h>

#include "neighbourhood/ETTPneighborhood.h"
//#include "neighbourhood/ETTPneighborEval.h"
#include "neighbourhood/statistics/ETTPneighborEvalNumEvalsCounter.h"

#include "algorithms/mo/moSimpleCoolingSchedule.h"
#include "algorithms/mo/moTA.h"
#include "eval/eoETTPEval.h"
#include "eval/eoNumberEvalsCounter.h"

#include "utils/CurrentDateTime.h"
#include <boost/make_shared.hpp>

// Using boost accumulators framework for computing the variance
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

// I'm importing the accumulators namespace as "a" to make clear which
// elements belong to it
namespace a = boost::accumulators;


#define EOCELLULARGA_DEBUG


/**
   The abstract cellular evolutionary algorithm.

   @ingroup Algorithms
 */
template <class EOT>
//class eoCellularEA : public eoAlgo<EOT> {
class eoCellularEA : public eoAlgoPointer<EOT> {

public :

    /**
     * Constructor
     */
    eoCellularEA(std::ofstream & _outFile, int _nrows, int _ncols, double _cp, double _mp, double _ip,
                 moSimpleCoolingSchedule<EOT> const& _coolSchedule,
//                 eoContinue<EOT> & _cont, // Stop. criterion
                 eoGenerationContinuePopVector<EOT> &_cont, // Stop. criterion
                 eoEvalFunc<EOT> & _eval, // Evaluation function
                 eoDetTournamentSelectSharedPtr<EOT> &_sel_neigh, // To choose a partner. // Work with pointers for efficiency
//                 eoSelectOne<EOT> & _sel_neigh, // To choose a partner
                 eoQuadOp<EOT> & _cross,
                 eoMonOp<EOT> & _mut, // Mutation operator
                 eoSelectOne<EOT> & _sel_child, // To choose one from the both children
                 eoSelectOne<EOT> & _sel_repl,   // Which to keep between the new
                                                 // child and the old individual?
                 eoNumberEvalsCounter &_numEvalCounter // # evaluations counter
                 ) :
        outFile(_outFile),
        nrows(_nrows), ncols(_ncols),
        cp(_cp), mp(_mp), ip(_ip), coolSchedule(_coolSchedule),
        cont (_cont),
        fullEval(_eval),
        popEval(_eval),
        sel_neigh(_sel_neigh),
        cross(_cross),
        mut(_mut),
        sel_child(_sel_child),
        sel_repl(_sel_repl),
        bestSolution(nullptr),
        popVariance(0),
        numEvalsCounter(_numEvalCounter)
    { }

    /**
     *   Evolve a given population
     */
//    void operator() (eoPop<EOT> & _pop) override {
    virtual void operator() (boost::shared_ptr<std::vector<boost::shared_ptr<EOT> > >&_pop) override {
        //
        // We apply a Synchronous cGA.
        // In the synchronous cGA the reproductive cycle is applied to all the
        // individuals simulataneously, that is, the individuals of the population
        // of the next generation are formally created at the same time, in a concurrent way.
        //
        // Create pointer to offspring population of chromosome pointers (empty)
        boost::shared_ptr<std::vector<boost::shared_ptr<EOT> > > offspringPop =
                boost::make_shared<std::vector<boost::shared_ptr<EOT> > >();


        int genNumber = 1;

#ifdef EOCELLULARGA_DEBUG
      std::cout << "Running cGA" << std::endl;

#endif

        do {
            // Clear the offspring population produced in the previous generation
            (*offspringPop.get()).clear();

            // Get reference to original population
            std::vector<boost::shared_ptr<EOT> > &originalPop = *_pop.get();

            // Produce the generation offspring
            for (int i = 0; i < originalPop.size(); ++i) {
                // Who are neighbouring to the current individual?
                //
                // The neighbours method return a vector containing const pointers
                // the neighbour solutions
                std::vector<boost::shared_ptr<EOT> > neighs = neighbours(originalPop, i);

                // cout << "Neighbours: " << endl;
                // cout << "_pop[i].fitness() = " << _pop[i].fitness() << endl;

                // Create, in the heap, object copies of current individual and its neighbour
                boost::shared_ptr<EOT> solCopy(new EOT(*originalPop[i].get())); // Invoke the copy ctor
                boost::shared_ptr<EOT> part(new EOT(*sel_neigh(neighs).get())); // Invoke the copy ctor

                // To perform cross-over
                if (rng.uniform() < cp) {
                    // Change the _pop[i] and part solutions directly
                    cross(*solCopy.get(), *part.get());
                    // # evals statistics computation. Add 2 to # evals
                    numEvalsCounter.addNumEvalsToGenerationTotal(2);
                }
                // To perform mutation
                if (rng.uniform() < mp) {
                    // Change the solutions directly
                    mut(*solCopy.get());
                    mut(*part.get());
                    // # evals statistics computation. Add 2 to # evals
                    numEvalsCounter.addNumEvalsToGenerationTotal(2);
                }

                //
                // Improvement by Local search.
                // Local search used: Threshold Accepting algorithm
                //
                // moTA parameters
                boost::shared_ptr<ETTPKempeChainHeuristic<EOT> > kempeChainHeuristic(new ETTPKempeChainHeuristic<EOT>());
                ETTPneighborhood<EOT> neighborhood(kempeChainHeuristic);
//                ETTPneighborEval<EOT> neighEval;
                // ETTPneighborEvalWithStatistics which receives as argument an
                // eoNumberEvalsCounter for counting neigbour # evaluations
                ETTPneighborEvalNumEvalsCounter<EOT> neighEval(this->numEvalsCounter);
                // Copy of cool schedule to use in TA solver
                auto cool = coolSchedule;
                moTA<ETTPneighbor<EOT> > ta(neighborhood, fullEval, neighEval, cool);

                if (rng.uniform() < ip) {
                    // Change the solutions directly
                    ta(*solCopy.get());
//                    std::cout << "After TA" << std::endl;
//                    std::cout << "# evaluations generation: " << numEvalsCounter.getGenerationNumEvals()
//                    << ", Total # evaluations: " << numEvalsCounter.getTotalNumEvals() << std::endl;

                    ta(*part.get());
//                    std::cout << "After TA" << std::endl;
//                    std::cout << "# evaluations generation: " << numEvalsCounter.getGenerationNumEvals()
//                    << ", Total # evaluations: " << numEvalsCounter.getTotalNumEvals() << std::endl;
#ifdef EOCELLULARGA_DEBUG
//                    std::cout << "After TA" << std::endl;
//                    std::cout << "sol.fitness() = " << (*solCopy.get()).fitness() << std::endl;
//                    std::cout << "part.fitness() = " << (*part.get()).fitness() << std::endl;
#endif

                }

                // To choose the best of the two children
                boost::shared_ptr<EOT> offspringSol;
                if ((*solCopy.get()).fitness() < (*part.get()).fitness())
                    offspringSol = solCopy;
                else
                    offspringSol = part;


                // To choose the best between the new made child and the old individual
                boost::shared_ptr<EOT> bestOffspringSol;
                if ((*originalPop[i].get()).fitness() < (*offspringSol.get()).fitness()) {
                    boost::shared_ptr<EOT> originalSol(new EOT(*originalPop[i].get())); // Invoke the copy ctor
                    bestOffspringSol = originalSol;
                }
                else
                    bestOffspringSol = offspringSol;

                // Insert into the offspring vector
                (*offspringPop.get()).push_back(bestOffspringSol);


            } // End of generation

            // Swap offspring and original populations
            offspringPop.swap(_pop);
            // Add to total evaluations the generation # evals
            numEvalsCounter.addNumEvalsToTotal(numEvalsCounter.getGenerationNumEvals());
            // Get reference to population
            std::vector<boost::shared_ptr<EOT> > &finalPop = *_pop.get();

            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Compute population variance
            // The accumulator set which will calculate the properties for us:
            a::accumulator_set< double, a::stats<a::tag::mean, a::tag::variance> > acc_variance;
            for (unsigned i = 0; i < finalPop.size(); ++i)
                acc_variance((*finalPop[i].get()).fitness());
            // Set variance value
            popVariance = a::variance(acc_variance);

            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Determine the best solution
            for (unsigned i = 0; i < finalPop.size(); ++i) {
                if (bestSolution.get() == nullptr || (*finalPop[i].get()).fitness() < bestSolution->fitness())
                    bestSolution = finalPop[i];
            }

            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Print population information to output and
            // save population information into file
            std::cout << std::endl << "==============================================================" << std::endl;
            std::cout << "Generation # " << genNumber << ", Date/Time = " << currentDateTime() << std::endl;
            outFile << std::endl << "==============================================================" << std::endl;
            outFile << "Generation # " << genNumber << ", Date/Time = " << currentDateTime() << std::endl;
            int k = 0;
            for (int i = 0; i < nrows; ++i) {
                for (int j = 0; j < ncols; ++j, ++k) {
                    std::cout << (*finalPop[k].get()).fitness() << "\t";
                    outFile << (*finalPop[k].get()).fitness() << "\t";
                }
                std::cout << std::endl;
                outFile << std::endl;
            }
// Just print population vector
//            for (int i = 0; i < nrows*ncols; ++i) {
//                outFile << (*finalPop[i].get()).fitness() << "\t";
//            }
//            outFile << std::endl;

            std::cout << "popVariance = " << popVariance << ", best sol = " << bestSolution->fitness() << std::endl
                      << "# evaluations generation: " << numEvalsCounter.getGenerationNumEvals()
                      << ", Total # evaluations: " << numEvalsCounter.getTotalNumEvals() << std::endl;
            outFile << "popVariance = " << popVariance << ", best sol = " << bestSolution->fitness() << std::endl
                      << "# evaluations generation: " << numEvalsCounter.getGenerationNumEvals()
                      << ", Total # evaluations: " << numEvalsCounter.getTotalNumEvals() << std::endl;
            // Save best solution to file
            outFile << *getBestSolution() << std::endl;
            cout << "==============================================================" << std::endl;
            outFile << "==============================================================" << std::endl;
            ///////////////////////////////////////////////////////////////////////////////////////////////////

            // Increment # generations
            ++genNumber;

            // Reset # evaluations generation counter
            numEvalsCounter.setGenerationNumEvals(0);

        } while (cont(*_pop.get()));

        std::cout << std::endl << "End of evolution cycle" << std::endl
             << "Writing best solution to file..." << std::endl;
        // Write best solution to file
        outFile << std::endl << "End of evolution cycle" << std::endl
                << "Best solution: " << std::endl;
        outFile << *getBestSolution() << std::endl;
    }


    EOT* getBestSolution() { return bestSolution.get(); }

protected :

//    virtual eoPop<const EOT*> neighbours (const eoPop <EOT> & pop, int rank) const = 0;
    virtual std::vector<boost::shared_ptr<EOT> > neighbours (
            const std::vector<boost::shared_ptr<EOT> > &_pop, int _rank) const = 0;

    std::ofstream & outFile;
    int nrows, ncols;
    double cp, mp, ip;
    moSimpleCoolingSchedule<EOT> const& coolSchedule;
//    eoContinue<EOT> & cont;
    eoGenerationContinuePopVector<EOT> &cont; // Stop. criterion
    eoEvalFunc<EOT> & fullEval;
    eoPopLoopEval<EOT> popEval;
//    eoSelectOne<EOT> & sel_neigh;
    eoDetTournamentSelectSharedPtr<EOT> &sel_neigh; // To choose a partner. // Work with pointers for efficiency
    eoBF<EOT &, EOT &, bool> & cross;
    eoMonOp<EOT> & mut;
    eoSelectOne<EOT> & sel_child;
    eoSelectOne<EOT> & sel_repl;
//    EOT* bestSolution; // Reference to the best solution
    boost::shared_ptr<EOT> bestSolution; // Reference to the best solution
    double popVariance; // Population variance
    eoNumberEvalsCounter &numEvalsCounter;
};




#endif // EOCELLULARGA_H
