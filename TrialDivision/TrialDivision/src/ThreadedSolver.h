#pragma once
#include <vector>
#include <thread>
#include "Solver.h"

class ThreadedSolver : public Solver {
private:
    int m_NumThreads;
    std::vector<std::thread> m_Threads;

public:
    ThreadedSolver(int num_threads);
    ~ThreadedSolver() override;
    TDivResult solve(mpz_class& n) override;
};