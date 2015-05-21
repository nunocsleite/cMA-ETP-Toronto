//-----------------------------------------------------------------------------
// Based on eoAlgo.h from ParadisEO framework
// (c) GeNeura Team, 1998
//-----------------------------------------------------------------------------

#ifndef EOALGOPOINTER_H
#define EOALGOPOINTER_H


#include <eoPop.h>                   // for population
#include <eoFunctor.h>

#include <boost/shared_ptr.hpp>

/**
  @defgroup Algorithms Algorithms

  In EO, an algorithm is a functor that takes one or several solutions to an optimization
  problem as arguments, and iteratively modify them with the help of operators.

  Generally, an EO object is built by assembling together @ref Operators in an algorithm instance,
  and then calling the algorithm's operator() on an initial population (an eoPop). The algorithm will then
  manipulate the solutions within the population to search for the problem's optimum.
*/

/**
    This is the base class for population-transforming algorithms. There
    is only one operator defined, which takes a population and does stuff to
    it. It needn't be a complete algorithm, can be also a step of an
    algorithm. This class just gives a common interface to linear
    population-transforming algorithms.

    @ingroup Algorithms
*/
template <class EOT>
class eoAlgoPointer : public eoUF<boost::shared_ptr<std::vector<boost::shared_ptr<EOT> > > &, void>
{
public:


};



#endif // EOALGOPOINTER_H
