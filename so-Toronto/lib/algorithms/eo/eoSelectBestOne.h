#ifndef EOSELECTBESTONE_H
#define EOSELECTBESTONE_H

#include <eoPop.h>


template <class EOT>
class eoSelectBestOne : public eoSelectOne<EOT> {

public :
    const EOT & operator () (const eoPop<EOT> & _pop) {
        size_t bestIdx = 0;
        for (unsigned i = 1; i < _pop.size(); ++i) {
            if (_pop[i].fitness() < _pop[bestIdx].fitness()) {
                bestIdx = i;
            }
        }
        return _pop[bestIdx];
    }
};

#endif // EOSELECTBESTONE_H
