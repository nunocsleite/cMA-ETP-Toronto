
#include <cmath>
#include <iostream>

#include "algorithms/mo/moSimpleCoolingSchedule.h"


using namespace std;


// Temperature actualization
double Temp(double t, double Tmax, double R) {
    double newtemp = Tmax*exp(-R*t);
    return newtemp;
}



int getSANumberEvaluations(double tmax, double r, double k, double tmin) {
    cout << "[getSANumberEvaluations] tmax = " << tmax << ", r = "
         << r << ", k = " << k << ", tmin = " << tmin << endl;

    double t = 0, temp = tmax;
    long numberEvaluations = 0;
    do {
        for (int i = 1; i <= k; ++i)
            ++numberEvaluations;
        // Actualize temperature
        ++t;
        temp = Temp(t, tmax, r);
    } while (temp > tmin);

    cout << "last temp (> tmin) = " << temp << endl;

    /////////////////////////////////////////////////////////////
    moSimpleCoolingSchedule<int> coolingSchedule(tmax, r, k, tmin);
    int sol = 10; // dummy
    coolingSchedule.init(sol);
    temp = tmax;
    bool acceptedMove = true;
    long numberEvaluations1 = 0;
    while (coolingSchedule(temp)) {
        coolingSchedule.update(temp, acceptedMove);
        ++numberEvaluations1;
    }
    cout << "numberEvaluations = " << numberEvaluations << ", numberEvaluations1 = " << numberEvaluations1 << endl;
    return numberEvaluations;
}






