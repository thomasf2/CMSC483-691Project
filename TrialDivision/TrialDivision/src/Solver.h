#pragma once
#include <gmpxx.h>
#include "result.h"

class Solver {

public:
    Solver() = default;
    virtual ~Solver() = default;
    virtual TDivResult solve(mpz_class& n) = 0;
};