#ifndef EOCELLULAREAMATRIX_H
#define EOCELLULAREAMATRIX_H

#include "eoCellularEA.h"
#include <fstream>
#include "eval/eoNumberEvalsCounter.h"
#include "algorithms/eo/eoGenerationContinuePopVector.h"

/**
 *  The cellular evolutionary algorithm - matrix grid.
 *
 *  @ingroup Algorithms
 */
template <class EOT>
class eoCellularEAMatrix : public eoCellularEA<EOT> {

public :

  /**
   *  Two constructors
   */

    eoCellularEAMatrix(std::ofstream & _outFile, int _nrows, int _ncols,
                     double _cp, double _mp, double _ip, moSimpleCoolingSchedule<EOT> const& _coolSchedule,
//                     eoContinue<EOT> & _cont, // Stop. criterion
                     eoGenerationContinuePopVector<EOT> &_cont, // Stop. criterion
                     eoEvalFunc<EOT> & _eval, // Evaluation function
//                     eoSelectOne<EOT> & _sel_neigh, // To choose a partner
                     eoDetTournamentSelectSharedPtr<EOT> & _sel_neigh, // To choose a partner. // Work with pointers for efficiency
                     eoQuadOp<EOT> & _cross, // Cross-over operator
                     eoMonOp<EOT> & _mut, // Mutation operator
                     eoSelectOne<EOT> & _sel_child, // To choose one from the both children
                     eoSelectOne<EOT> & _sel_repl,  // Which to keep between the new child and the old individual?
                     eoNumberEvalsCounter &_numEvalCounter // # evaluations counter
                    )
        : eoCellularEA<EOT>(_outFile, _nrows, _ncols, _cp, _mp, _ip, _coolSchedule,
                            _cont, _eval, _sel_neigh, _cross, _mut, _sel_child, _sel_repl, _numEvalCounter)
  { }

    // Neighbouring of the current individual with rank _rank
    // The individual rank is individual position index in the population vector
//    virtual eoPop<const EOT*> neighbours(const eoPop<EOT> & _pop, int _rank) const override {
    virtual std::vector<boost::shared_ptr<EOT> > neighbours(
            const std::vector<boost::shared_ptr<EOT> > &_pop, int _rank) const override {
        // Matrix cGA
        int row, col;
        row = _rank / this->ncols; // Integer division
        col = _rank % this->ncols;

/// TODO
/// GENERIC NEIGHBOURHOOD
///
///
        // 4-Neighbourhood
        int leftCoords[2]  = { row, col-1 < 0 ? this->ncols-1 : col-1 };
        int rightCoords[2] = { row, col+1 > this->ncols-1 ? 0 : col+1 };
        int northCoords[2] = { row-1 < 0 ? this->nrows-1 : row-1, col };
        int southCoords[2] = { row+1 > this->nrows-1 ? 0 : row+1, col };

//        cout << "rank = " << _rank << ", row = " << row << ", col = " << col << endl;
//        cout << "nrows = " << nrows << ", ncols = " << ncols << endl;
//        cout << "leftCoords[0]*ncols  + leftCoords[1] = " << (leftCoords[0]*ncols  + leftCoords[1]) << endl;

//        cin.get();

        boost::shared_ptr<EOT> const& leftNeigh  = _pop[leftCoords[0]*this->ncols  + leftCoords[1]];
        boost::shared_ptr<EOT> const& rightNeigh = _pop[rightCoords[0]*this->ncols + rightCoords[1]];
        boost::shared_ptr<EOT> const& northNeigh = _pop[northCoords[0]*this->ncols + northCoords[1]];
        boost::shared_ptr<EOT> const& southNeigh = _pop[southCoords[0]*this->ncols + southCoords[1]];

        // Neighbours pointers vector
        std::vector<boost::shared_ptr<EOT> > neighs;
        neighs.push_back(leftNeigh);
        neighs.push_back(rightNeigh);
        neighs.push_back(northNeigh);
        neighs.push_back(southNeigh);

        return neighs;
    }

};



#endif // EOCELLULAREAMATRIX_H
