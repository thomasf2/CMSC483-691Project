#include <iostream>
#include <chrono>
#include <thread>
#include "src/Solver.h"
#include "src/SerialSolver.h"
#include "src/ThreadedSolver.h"

void print_usage(void) {
    std::cout << "Usage: tdiv n <n_threads>\n" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 3) {
        print_usage();
        exit(0);
    }

    int num_threads = std::thread::hardware_concurrency();

    if (argc == 3) {
        std::string arg = argv[2];
        try {
            std::size_t pos;
            int x = std::stoi(arg, &pos);
            num_threads = x;
        }
        catch (std::invalid_argument const &ex) {
            std::cerr << "Invalid threads option: " << arg << ". Assuming " << num_threads << " threads" << std::endl;
        }
        catch (std::out_of_range const &ex) {
            std::cerr << "Invalid threads option: " << arg << ". Assuming " << num_threads << " threads" << std::endl;
        }
    }
    
    mpz_class n(argv[1], 10);

    auto start = std::chrono::steady_clock::now();


    //Solver* the_solver = new SerialSolver();
    Solver* the_solver = new ThreadedSolver(num_threads);

    TDivResult result = the_solver->solve(n);

    auto end = std::chrono::steady_clock::now();

    std::cout << "n: " << result.n << std::endl;
    std::cout << "p: " << result.p << std::endl;
    std::cout << "q: " << result.q << std::endl;
    std::cout << "Elapsed time in milliseconds : "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;

    std::cout << "Elapsed time in seconds : "
        << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
        << " sec" << std::endl;

    std::cout << "Elapsed time in minutes : "
        << std::chrono::duration_cast<std::chrono::minutes>(end - start).count()
        << " mins" << std::endl;

    delete the_solver;
	return 0;
}
