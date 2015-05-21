#ifndef EOCELLULARRINGEA_H
#define EOCELLULARRINGEA_H


#include "eoCellularEA.h"




///**
// *
// * The ring cellular evolutionary algorithm.
// *
// * @ingroup Algorithms
// */
//template <class EOT>
//class eoCellularEARing : public eoCellularEA<EOT> {

//public :


//    /**
//     * @brief eoCellularEARing
//     * @param _cont
//     * @param _eval
//     * @param _sel_neigh
//     * @param _cross
//     * @param _mut
//     * @param _sel_child
//     * @param _sel_repl
//     */
//    eoCellularEARing(eoContinue <EOT> & _cont, // Stop. criterion
//                     eoEvalFunc <EOT> & _eval, // Evaluation function
//                     eoSelectOne <EOT> & _sel_neigh, // To choose a partner
////                      eoBinOp <EOT> & _cross, // Cross-over operator
//                     eoQuadOp <EOT> & _cross,
//                     eoMonOp <EOT> & _mut, // Mutation operator
//                     // ADDED
//                     eoSelectOne <EOT> & _sel_child, /* To choose one from
//                                                      the both children */
//                     eoSelectOne <EOT> & _sel_repl /* Which to keep between the new
//                                                     child and the old individual ? */
//                    ) :
//      eoCellularEA<EOT>(_cont, _eval, _sel_neigh, _cross, _mut, _sel_child, _sel_repl)
//  { }


//    // Neighbouring of the current individual with rank _rank
//    virtual eoPop<EOT> neighbours (const eoPop <EOT> & _pop, int _rank) override {
//        // Ring configuration only has the left and right neighbours
//        eoPop<EOT> neighs;
//        int leftNeighIdx = (_rank - 1 < 0) ? _pop.size()-1 : _rank-1;
//        EOT leftNeigh = _pop[leftNeighIdx];
//        int rightNeighIdx = (_rank + 1) % _pop.size();
//        EOT rightNeigh = _pop[rightNeighIdx];

//        neighs.push_back(leftNeigh);
//        neighs.push_back(rightNeigh);
//        return neighs;

//     }

//};


#endif // EOCELLULARRINGEA_H
