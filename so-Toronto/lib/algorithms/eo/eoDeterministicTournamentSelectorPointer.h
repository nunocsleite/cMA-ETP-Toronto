///
/// Code based on heading file <selectors.h> but adapted in
/// order to support boost::shared_ptr
///
///

#ifndef EODETERMINISTICTOURNAMENTSELECTOR_H
#define EODETERMINISTICTOURNAMENTSELECTOR_H


#include <utils/eoRNG.h>
#include <eoPop.h>
#include <boost/shared_ptr.hpp>
#include <vector>




template <class It>
It deterministic_tournament_shared_ptr(It _begin, It _end, unsigned _t_size, eoRng& _gen = rng)
{
    It best = _begin + _gen.random(_end - _begin);

    for (unsigned i = 0; i < _t_size - 1; ++i)
    {
        It competitor = _begin + _gen.random(_end - _begin);

//        if (*best < *competitor)
        // As it's a population of shared_ptr<EOT>, we have to dereference it
        auto & ptrBest = *best;
        auto & ptrCompetitor = *competitor;
        if (*ptrBest.get() < *ptrCompetitor.get())
        {
            best = competitor;
        }
    }

    return best;
}


template <class EOT>
const boost::shared_ptr<EOT>& deterministic_tournament_shared_ptr(
        const std::vector<boost::shared_ptr<EOT> >& _pop, unsigned _t_size, eoRng& _gen = rng)
{
    return *deterministic_tournament_shared_ptr(_pop.begin(), _pop.end(), _t_size, _gen);
}


template <class EOT>
boost::shared_ptr<EOT>& deterministic_tournament_shared_ptr(
        std::vector<boost::shared_ptr<EOT> >& _pop, unsigned _t_size, eoRng& _gen = rng)
{
    return *deterministic_tournament_shared_ptr(_pop.begin(), _pop.end(), _t_size, _gen);
}



/** eoDetTournamentSelect: a selection method that selects ONE individual by
 deterministic tournament

 This uses a population of shared_ptr<EOT>

 @ingroup Selectors
 */
template <class EOT>
class eoDetTournamentSelectSharedPtr: public eoUF<const std::vector<boost::shared_ptr<EOT> >&, const boost::shared_ptr<EOT>&>
{
 public:
  /* (Default) Constructor -
     @param _tSize tournament size
  */
  eoDetTournamentSelectSharedPtr(unsigned _tSize = 2 ) : tSize(_tSize) {
    // consistency check
    if (tSize < 2) {
        eo::log << eo::warnings << "Tournament size should be >= 2, adjusted to 2" << std::endl;
      tSize = 2;
    }
  }

  /* Perform deterministic tournament calling the appropriate fn
     see selectors.h
  */
  virtual const boost::shared_ptr<EOT>& operator()(const std::vector<boost::shared_ptr<EOT> >& _pop)
  {
      return deterministic_tournament_shared_ptr(_pop, tSize);
  }

private:
    unsigned tSize;
};





#endif // EODETERMINISTICTOURNAMENTSELECTOR_H
