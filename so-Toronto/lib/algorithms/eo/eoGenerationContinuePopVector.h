#ifndef EOGENERATIONCONTINUEPOPVECTOR_H
#define EOGENERATIONCONTINUEPOPVECTOR_H

#include <eoFunctor.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

extern volatile bool keepRunning;


//#define EOGENERATIONCONTINUEPOPVECTOR_DEBUG


/**
  Code based on ParadisEO eoGenContinue class

  Generational or keyboard continuator:
    continues until a number of generations is reached or a key is pressed

  @ingroup Continuators
*/
template <class EOT>
class eoGenerationContinuePopVector: public eoUF<const std::vector<boost::shared_ptr<EOT> >&, bool>
{
public:

    /// Ctor for setting a
    eoGenerationContinuePopVector( unsigned long _totalGens)
          : repTotalGenerations(_totalGens),
            thisGeneration(0)
    { }

    virtual std::string className(void) const { return "eoGenerationContinuePopVector"; }

    /** Returns false when a certain number of generations is
     *  reached
     */
    virtual bool operator() (const std::vector<boost::shared_ptr<EOT> >&_pop) override {

#ifdef EOGENERATIONCONTINUEPOPVECTOR_DEBUG
        cout << "eoGenerationContinuePopVector::operator()(vector<boost::shared_ptr<EOT> >& pop)" << endl;
        cin.get();
#endif

        (void)_pop;
        thisGeneration++;

        if (thisGeneration >= repTotalGenerations)
        {
            std::cout << "STOP in eoGenerationContinuePopVector: Reached maximum number of generations ["
                    << thisGeneration << "/" << repTotalGenerations << "]\n" << std::endl;
            return false;
        }

        if (!keepRunning) {
            std::cout << std::endl << "Ctrl+C pressed! Do you really want to quit? [y/n] " << std::endl;
            char c;
            std::cin >> c;
            if (c == 'y' || c == 'Y') {
                std::cout << "Quitting application now..." << std::endl;
                return false;
            }
            else {
                keepRunning = true;
                std::cout << "Resuming application now..." << std::endl;
                return true;
            }
        }
        return true;
    }

    /** Sets the number of generations to reach
          and sets the current generation to 0 (the begin)

      @todo replace this by an "init" method
     */
    virtual void totalGenerations( unsigned long _tg ) {
        repTotalGenerations = _tg;
        thisGeneration = 0;
    }

    /** Returns the number of generations to reach */
    virtual unsigned long totalGenerations()
    {
        return repTotalGenerations;
    }

private:
    unsigned long repTotalGenerations;
    unsigned long thisGeneration;
};



#endif // EOGENERATIONCONTINUE_H
