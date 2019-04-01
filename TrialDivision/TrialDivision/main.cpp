// TrialDivision.cpp : Defines the entry point for the application.
//

#include "TrialDivision.h"
#include <iostream>
#include <chrono>
//#include <gmp.h>
#include <gmpxx.h>
#include <cstdint>
#include <cstring>

void print_usage(void) {
    printf("Usage: tdiv n\n");
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        print_usage();
    }
    
    mpz_class n(argv[1], 10);

    mpz_class root = sqrt(n);

    mpz_class p = (root % 2 == 1) ? root : root + 1;
    auto start = std::chrono::steady_clock::now();
    
    while ((n % p) != 0) {
        // We only care about the odd numbers
        p -= 2;
    }

    mpz_class q = n / p;
    auto end = std::chrono::steady_clock::now();

    std::cout << "n: " << n << std::endl;
    std::cout << "p: " << p << std::endl;
    std::cout << "q: " << q << std::endl;
    std::cout << "Elapsed time in milliseconds : "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;

    std::cout << "Elapsed time in seconds : "
        << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
        << " sec" << std::endl;

	return 0;
}
