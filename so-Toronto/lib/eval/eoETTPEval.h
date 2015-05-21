#ifndef EOETTPEVAL_H
#define EOETTPEVAL_H


#include <eoEvalFunc.h>

// For debugging purposes
//#define EOETTPEVAL_DEBUG



/**
 * Evaluation of objective function
 */
template <class EOT>
class eoETTPEval : public eoEvalFunc<EOT> {
public:
    void operator()(EOT& _chrom) {

#ifdef EOETTPEVAL_DEBUG
        cout << "eoETTPEval::operator()()" << endl;

        cout << "Full chromosome eval" << endl;
#endif

        // Compute chromosome exam proximity conflicts
        _chrom.computeExamProximityConflicts();
        // Set chromosome fitness
        _chrom.fitness(_chrom.getProximityCost());
    }
};






#endif // EOETTPEVAL_H
