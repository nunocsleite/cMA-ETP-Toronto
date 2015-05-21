#ifndef KEMPECHAIN_H
#define KEMPECHAIN_H

#include <boost/unordered_set.hpp>


/**
 * @brief The ETTPKempeChain class
 *
 * This class keeps information about a Kempe chain, namely: the selected exam ei to move,
 * the source and destination time slots where the exam is being moved to in a feasible way,
 *
 */
template <typename EOT>
class ETTPKempeChain {

public:

    /**
     * @brief ETTPKempeChain Default constructor
     */
    ETTPKempeChain()
        : chrom(nullptr), ei(-1), ti(-1), tj(-1),
          sourceTiPeriodExams(0), sourceTjPeriodExams(0),
          destTiPeriodExams(0), destTjPeriodExams(0)  { }

    // Get/set the solution where the Kempe chain information is associated
    EOT &getSolution() const;
    void setSolution(EOT *_chrom);
    // Get/set the selected exam ei to move
    int getEi() const;
    void setEi(int _ei);
    // Get/set the Ti time slot
    int getTi() const;
    void setTi(int _ti);
    // Get/set the Tj time slot
    int getTj() const;
    void setTj(int _tj);
    // Get/set Source Ti time slot exams
    const std::vector<int> &getSourceTiPeriodExams() const;
    void setSourceTiPeriodExams(const std::vector<int> &_value);
    // Get/set Source Tj time slot exams
    const std::vector<int> &getSourceTjPeriodExams() const;
    void setSourceTjPeriodExams(const std::vector<int> &_value);
    // Get/set Destination Ti time slot exams
    const std::vector<int> &getDestTiPeriodExams() const;
    void setDestTiPeriodExams(const std::vector<int> &_value);
    // Get/set Destination Tj time slot exams
    const std::vector<int> &getDestTjPeriodExams() const;
    void setDestTjPeriodExams(const std::vector<int> &_value);

private:
    /**
     * @brief chrom The solution where the Kempe chain information is associated
     */
    EOT *chrom;
    /**
     * @brief ei The selected exam ei to move
     */
    int ei;
    /**
     * @brief ti Source time slot
     */
    int ti;
    /**
     * @brief tj Destination time slot
     */
    int tj;
    /**
     * @brief sourceTiPeriodExams Source Ti time slot exams
     * (original exams prior moving the solution)
     */
    std::vector<int> sourceTiPeriodExams;
    /**
     * @brief sourceTjPeriodExams Source Tj time slot exams
     * (original exams prior moving the solution)
     */
    std::vector<int> sourceTjPeriodExams;
    /**
     * @brief destTiPeriodExams Destination Ti time slot exams
     * (period contents after moving the solution)
     */
    std::vector<int> destTiPeriodExams;
    /**
     * @brief destTjPeriodExams Destination Tj time slot exams
     * (period contents after moving the solution)
     */
    std::vector<int> destTjPeriodExams;
};


template <typename EOT>
EOT &ETTPKempeChain<EOT>::getSolution() const
{
    return *chrom;
}

template <typename EOT>
void ETTPKempeChain<EOT>::setSolution(EOT *_chrom)
{
    chrom = _chrom;
}

template <typename EOT>
int ETTPKempeChain<EOT>::getEi() const
{
    return ei;
}
template <typename EOT>
void ETTPKempeChain<EOT>::setEi(int _ei)
{
    ei = _ei;
}

template <typename EOT>
int ETTPKempeChain<EOT>::getTi() const
{
    return ti;
}
template <typename EOT>
void ETTPKempeChain<EOT>::setTi(int _ti)
{
    ti = _ti;
}

template <typename EOT>
int ETTPKempeChain<EOT>::getTj() const
{
    return tj;
}
template <typename EOT>
void ETTPKempeChain<EOT>::setTj(int _tj)
{
    tj = _tj;
}

template <typename EOT>
const std::vector<int> &ETTPKempeChain<EOT>::getSourceTiPeriodExams() const
{
    return sourceTiPeriodExams;
}

template <typename EOT>
void ETTPKempeChain<EOT>::setSourceTiPeriodExams(const std::vector<int> &_value)
{
    sourceTiPeriodExams = _value;
}

template <typename EOT>
const std::vector<int> &ETTPKempeChain<EOT>::getSourceTjPeriodExams() const
{
    return sourceTjPeriodExams;
}

template <typename EOT>
void ETTPKempeChain<EOT>::setSourceTjPeriodExams(const std::vector<int> &_value)
{
    sourceTjPeriodExams = _value;
}

template <typename EOT>
const std::vector<int> &ETTPKempeChain<EOT>::getDestTiPeriodExams() const
{
    return destTiPeriodExams;
}

template <typename EOT>
void ETTPKempeChain<EOT>::setDestTiPeriodExams(const std::vector<int> &_value)
{
    destTiPeriodExams = _value;
}

template <typename EOT>
const std::vector<int> &ETTPKempeChain<EOT>::getDestTjPeriodExams() const
{
    return destTjPeriodExams;
}

template <typename EOT>
void ETTPKempeChain<EOT>::setDestTjPeriodExams(const std::vector<int> &_value)
{
    destTjPeriodExams = _value;
}






#endif // KEMPECHAIN_H
