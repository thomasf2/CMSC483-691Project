#include "SerialSolver.h"

SerialSolver::SerialSolver() {}

SerialSolver::~SerialSolver() {}

TDivResult SerialSolver::solve(mpz_class& n)
{
    TDivResult result;
    result.n = n;
    mpz_class root = sqrt(n);

    mpz_class p = (root % 2 == 1) ? root : root + 1;

    while ((n % p) != 0) {
        // We only care about the odd numbers
        p -= 2;
    }

    result.p = p;
    result.q = n / p;

    return result;
}
