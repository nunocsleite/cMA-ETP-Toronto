#ifndef EOGENERATIONCONTINUE_H
#define EOGENERATIONCONTINUE_H

#include <eoContinue.h>
#include <utils/eoParam.h>
#include <utils/eoLogger.h>
#include <stdio.h>
#include <stdlib.h>


extern volatile bool keepRunning;


/**
  Code based on ParadisEO eoGenContinue class

  Generational or keyboard continuator:
    continues until a number of generations is reached or a key is pressed

  @ingroup Continuators
*/
template< class EOT>
class eoGenerationContinue: public eoContinue<EOT>, public eoValueParam<unsigned>
{
public:

  /// Ctor for setting a
  eoGenerationContinue( unsigned long _totalGens)
          : eoValueParam<unsigned>(0, "Generations", "Generations"),
            repTotalGenerations( _totalGens ),
            thisGenerationPlaceHolder(0),
            thisGeneration(thisGenerationPlaceHolder)
    { }

  /// Ctor for enabling the save/load the no. of generations counted
  eoGenerationContinue( unsigned long _totalGens, unsigned long& _currentGen)
          : eoValueParam<unsigned>(0, "Generations", "Generations"),
            repTotalGenerations( _totalGens ),
            thisGenerationPlaceHolder(0),
            thisGeneration(_currentGen)
    { }


    /** Returns false when a certain number of generations is
     *  reached
     */
    virtual bool operator() ( const eoPop<EOT>& _vEO ) override {
        (void)_vEO;
        thisGeneration++;
        value() = thisGeneration;

        if (thisGeneration >= repTotalGenerations)
        {
            eo::log << eo::logging << "STOP in eoGenerationContinue: Reached maximum number of generations ["
                    << thisGeneration << "/" << repTotalGenerations << "]\n";
            return false;
        }

        if (!keepRunning) {
            cout << endl << "Ctrl+C pressed! Do you really want to quit? [y/n] " << endl;
            char c;
            cin >> c;
            if (c == 'y' || c == 'Y') {
                cout << "Quitting application now..." << endl;
                return false;
            }
            else {
                keepRunning = true;
                cout << "Resuming application now..." << endl;
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
    virtual unsigned long totalGenerations( )
    {
        return repTotalGenerations;
    }


    virtual std::string className(void) const override { return "eoGenerationContinue"; }

    /** Read from a stream
     * @param __is the istream to read from
     */
    void readFrom (std :: istream & __is) {

        __is >> thisGeneration; /* Loading the number of generations counted */
    }

    /** Print on a stream
     * @param __os the ostream to print on
     */
    void printOn (std :: ostream & __os) const {

        __os << thisGeneration << std :: endl; /* Saving the number of generations counted */
    }

private:
    unsigned long repTotalGenerations;
    unsigned long thisGenerationPlaceHolder;
    unsigned long& thisGeneration;
};



#endif // EOGENERATIONCONTINUE_H
