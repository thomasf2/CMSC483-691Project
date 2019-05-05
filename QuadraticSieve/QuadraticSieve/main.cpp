#include <iostream>
#include <chrono>
#include <thread>
#include "src/QS-parallel.h"

void print_usage(void) {
    std::cout << "Usage: qs n <n_threads>" << std::endl;
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

    QSParallel solver(n, num_threads, 10);
    solver.setNumThreads(num_threads);

    solver.Factor();
    
    if(solver.GetFactor1 () == 1 || solver.GetFactor1 () == n)
    {
        std::cout << ">>>> Failed to factor " << n << " <<<<\t"
                << "Try using more linear relations" << std::endl;
    }
    auto end = std::chrono::steady_clock::now();

    

    std::cout << "n: " << n << std::endl;
    std::cout << "p: " << solver.GetFactor1() << std::endl;
    std::cout << "q: " << solver.GetFactor2() << std::endl;
    std::cout << "Elapsed time in milliseconds : "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;

    std::cout << "Elapsed time in seconds : "
        << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
        << " sec" << std::endl;

    std::cout << "Elapsed time in minutes : "
        << std::chrono::duration_cast<std::chrono::minutes>(end - start).count()
        << " mins" << std::endl;

	return 0;
}
