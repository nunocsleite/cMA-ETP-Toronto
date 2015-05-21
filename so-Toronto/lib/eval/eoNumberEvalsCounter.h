#ifndef EONUMBEREVALSCOUNTER_H
#define EONUMBEREVALSCOUNTER_H


class eoNumberEvalsCounter {

public:
    inline eoNumberEvalsCounter() : totalNumEvals(0), generationNumEvals(0) { }

    inline long getTotalNumEvals() const;
    inline void setTotalNumEvals(long _value);

    inline long getGenerationNumEvals() const;
    inline void setGenerationNumEvals(long _value);

    inline void addNumEvalsToTotal(long _value);
    inline void addNumEvalsToGenerationTotal(long _value);

protected:
    /**
     * @brief totalNumEvals Total # number of evaluations done
     */
    long totalNumEvals;
    /**
     * @brief generationNumEvals # number of evaluations done
     * in one generation of the genetic algorithm
     */
    long generationNumEvals;
};


long eoNumberEvalsCounter::getTotalNumEvals() const
{
    return totalNumEvals;
}

void eoNumberEvalsCounter::setTotalNumEvals(long _value)
{
    totalNumEvals = _value;
}

long eoNumberEvalsCounter::getGenerationNumEvals() const
{
    return generationNumEvals;
}

void eoNumberEvalsCounter::setGenerationNumEvals(long _value)
{
    generationNumEvals = _value;
}

void eoNumberEvalsCounter::addNumEvalsToTotal(long _value) {
    // Add _value to the total # evals
    totalNumEvals += _value;
}

void eoNumberEvalsCounter::addNumEvalsToGenerationTotal(long _value) {
    // Add _value to the generation # evals
    generationNumEvals += _value;
}

#endif // EONUMBEREVALSCOUNTER_H
