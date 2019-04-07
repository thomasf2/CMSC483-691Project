#pragma once 
#include "Solver.h"


class SerialSolver : public Solver {
public:
    SerialSolver();
    ~SerialSolver();
    TDivResult solve(mpz_class& n) override;
};