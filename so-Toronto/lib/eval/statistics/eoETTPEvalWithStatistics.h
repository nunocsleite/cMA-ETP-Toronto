#ifndef EOETTPEVALWITHSTATISTICS_H
#define EOETTPEVALWITHSTATISTICS_H


#include "eval/eoETTPEval.h"
#include "eval/eoNumberEvalsCounter.h"



/**
 * Evaluation of objective function and # evals statistics computation
 */
template <class EOT>
class eoETTPEvalWithStatistics : public eoETTPEval<EOT> {
public:

    eoETTPEvalWithStatistics(eoNumberEvalsCounter &_numberEvalsCounter)
        : numberEvalsCounter(_numberEvalsCounter) { }

    void operator()(EOT& _chrom) {
        // Invoke base class method to perform chromosome evaluation
        eoETTPEval<EOT>::operator ()(_chrom);
        // # evals statistics computation. Add 1 to # evals
        numberEvalsCounter.addNumEvalsToGenerationTotal(1);
    }


protected:

    eoNumberEvalsCounter &numberEvalsCounter;
};



#endif // EOETTPEVALWITHSTATISTICS_H
