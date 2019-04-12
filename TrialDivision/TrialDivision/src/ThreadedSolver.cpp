#include "ThreadedSolver.h"
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

ThreadedSolver::ThreadedSolver(int num_threads)
    : m_NumThreads(num_threads), m_Threads(std::vector<std::thread>(num_threads))
{
}

ThreadedSolver::~ThreadedSolver()
{
}

TDivResult ThreadedSolver::solve(mpz_class & n)
{
    std::mutex m;
    std::condition_variable cond;
    std::atomic<bool> running(true);
    TDivResult the_result;
    the_result.n = n;
    mpz_class root = sqrt(n);
    mpz_class div_start = (root % 2 == 1) ? root : root + 1;
    mpz_class p = 0;
    mpz_class chunk_size = div_start / m_NumThreads;

    if (n % 2 == 0) {
        // We should stop here. The key is bad and it should feel bad.
        #if defined(LOG_OUTPUT)
        std::cout << "The key is a factor of two" << std::endl;
        #endif
        the_result.p = 2;
        the_result.q = n / 2;
        return the_result;
    }

    #if defined(LOG_OUTPUT)
    std::cout << "Trying to find the prime factors of " << n << ". Using " << m_NumThreads << " threads." << std::endl;

    std::cout << "The root is " << div_start << std::endl;
    #endif

    for (int i = 0; i < m_NumThreads; i++)
    {
        // Create the threads
        m_Threads[i] = std::thread([i, chunk_size, div_start, &cond, &n, &p, &running] {
            mpz_class low = i * chunk_size;
            mpz_class high = low + chunk_size;
            low = low == 0 ? 2 : low;
            high = (div_start - high < chunk_size) ? div_start : high;
            high = (high % 2 == 0) ? high + 1 : high;
            
            #if defined(LOG_OUTPUT)
            std::cout << "Thread " << i << " looking for prime factors from " << low << " to " << high << std::endl;
            #endif

            while (true) {

                if (!running.load()) {
                    return;
                }

                if ((n % high) == 0) {
                    #if defined(LOG_OUTPUT)
                    std::cout << "Thread " << i << " has found a prime. high = " << high << " low = " << low << std::endl;
                    #endif
                    p = high;
                    running = false;
                    cond.notify_all();
                    return;
                }
                else if (high > low) {
                    high -= 2;
                }
                else {
                    break;
                }

            }
        });

        // Detach them so we don't have to join them
        m_Threads[i].detach();
    }

    std::unique_lock<std::mutex> lock{ m };
    // Wait until any of the threads signals us. p MUST be captured as a reference, since the value in the lambda
    // is not updated every time we evaluate the wait
    cond.wait(lock, [&p] {
        return p != 0;
    });

    the_result.p = p;
    the_result.q = n / p;

    return the_result;
}
