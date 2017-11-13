#include <cstdlib>
#include <cmath>
#include "RandomVariable.h"

// Generate number from [0,1]
double universal_rv () {
	return (double) rand () / (double) RAND_MAX;
}

int uniform_rv (int number) {
	return (int) (universal_rv () * number);
}

double exponential_rv (double lambda) {
    double exp;

    exp = ((-1) / lambda) * log (universal_rv ());
    return (exp);
}   	
