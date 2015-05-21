#ifndef EXAM_H
#define EXAM_H

#include <vector>
#include <boost/shared_ptr.hpp>


using namespace std;


class Constraint;


class Exam {

public:
    // Constructors
//    Exam(int _id) : id(_id), scheduled(false), period(0), room(0), numStudents(0), duration(0),
//                    periodRelatedHardConstraints(0), roomRelatedHardConstraints(0) { }
//    Exam(int _id, int _numStudents, int _duration) : id(_id), scheduled(false), period(0), room(0),
//        numStudents(_numStudents), duration(_duration),
//        periodRelatedHardConstraints(0), roomRelatedHardConstraints(0) { }

    Exam(int _id) : id(_id), scheduled(false), period(0), room(0), numStudents(0), duration(0) { }

    Exam(int _id, int _numStudents, int _duration) : id(_id), scheduled(false), period(0), room(0),
        numStudents(_numStudents), duration(_duration) { }

    // Public interface

    inline int getId() const;

    inline bool isScheduled() const;
    inline void schedule(bool value);

    template <typename EOT>
    void schedule(int _tj, int _rk, EOT &_chrom);

    inline int getPeriod() const;
    inline void setPeriod(int value);

    inline int getRoom() const;
    inline void setRoom(int value);

    inline int getNumStudents() const;
    inline void setNumStudents(int value);

    inline int getDuration() const;
    inline void setDuration(int value);


//    inline void insertPeriodRelatedHardConstraint(boost::shared_ptr<Constraint> hardConstraint);
//    inline vector<boost::shared_ptr<Constraint> > const& getPeriodRelatedHardConstraints() const;

//    inline void insertRoomRelatedHardConstraint(boost::shared_ptr<Constraint> hardConstraint);
//    inline vector<boost::shared_ptr<Constraint> > const& getRoomRelatedHardConstraints() const;

private:
    int id;
    bool scheduled;
    int period;
    int room;
    int numStudents;
    int duration;
//    vector<boost::shared_ptr<Constraint> > periodRelatedHardConstraints;
//    vector<boost::shared_ptr<Constraint> > roomRelatedHardConstraints;
};


inline int Exam::getId() const
{
    return id;
}

bool Exam::isScheduled() const
{
    return scheduled;
}
inline void Exam::schedule(bool value)
{
    scheduled = value;
}

template <typename EOT>
void Exam::schedule(int _tj, int _rk, EOT &_chrom) {
    //
    // Schedule exam this in time slot 'tj' and room 'rk'
    //
    setPeriod(_tj);
    setRoom(_rk);
    // Get room vector
    auto const& roomVector = _chrom.getRoomVector();
    // Get number of currently occupied seats for room _rk
    int numOccupiedSeats = roomVector[_rk]->getNumOccupiedSeats();
    // Get number of students for exam _ei
    int thisExamNumStudents = getNumStudents();
    roomVector[_rk]->setNumOccupiedSeats(numOccupiedSeats + thisExamNumStudents);
    // Insert exam in random feasible room-period
    auto& timetable = _chrom.getTimetable();
    timetable(_rk, _tj).insert(getId());
}



inline int Exam::getPeriod() const
{
    return period;
}
inline void Exam::setPeriod(int value)
{
    period = value;
    // Mark exam as scheduled
    schedule(true);
}

inline int Exam::getRoom() const
{
    return room;
}
inline void Exam::setRoom(int value)
{
    room = value;
}

inline int Exam::getNumStudents() const
{
    return numStudents;
}
inline void Exam::setNumStudents(int value)
{
    numStudents = value;
}

inline int Exam::getDuration() const
{
    return duration;
}
inline void Exam::setDuration(int value)
{
    duration = value;
}

//inline void Exam::insertPeriodRelatedHardConstraint(boost::shared_ptr<Constraint> hardConstraint)
//{
//    // Insert Period-related hard constraint into internal vector
//    periodRelatedHardConstraints.push_back(hardConstraint);
//}

//inline void Exam::insertRoomRelatedHardConstraint(boost::shared_ptr<Constraint> hardConstraint)
//{
//    // Insert Room-related hard constraint into internal vector
//    roomRelatedHardConstraints.push_back(hardConstraint);
//}
//inline const vector<boost::shared_ptr<Constraint> > &Exam::getPeriodRelatedHardConstraints() const
//{
//    return periodRelatedHardConstraints;
//}
//inline const vector<boost::shared_ptr<Constraint> > &Exam::getRoomRelatedHardConstraints() const
//{
//    return roomRelatedHardConstraints;
//}








#endif // EXAM_H
