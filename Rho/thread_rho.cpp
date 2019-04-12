#include "stdio.h"
#include "stdlib.h"
#include <gmpxx.h>

#include <thread>
#include <atomic>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include <vector>
mpz_class abso(mpz_class a) //fn to return absolute value
{
  return a > 0 ? a : -a;
}
int recur_depth = 0;

mpz_class gcd(mpz_class a, mpz_class b) //Euclidean GCD recursive fn
{
  if (b == 0)
    return a;
  else
  {
    return gcd(b, a % b);
  }
}

int main(int argc, char *argv[]) //Driver Program
{
    srand(time(NULL));
    if (argc < 2 || argc > 3) {
      std::cout << "WRONG" << std::endl;
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
            std::cerr << "Invalid threads option: " << arg << ". Assuming " << num_threads << " thrads" << std::endl;
        }
        catch (std::out_of_range const &ex) {
            std::cerr << "Invalid threads option: " << arg << ". Assuming " << num_threads << " threads" << std::endl;
        }
    }


    mpz_class n(argv[1], 10);

    mpz_class p = 0;
    mpz_class q;

    std::mutex m;
    std::condition_variable cond;
    std::atomic<bool> running(true);

    if(n%2==0){
      printf("EVEN\n");
      return -1;
    }
    
    std::vector<std::thread> threads;

    threads.reserve(num_threads);
    
    std::cout << "Starting " << num_threads  << "  threads for number " << n << std::endl;
    
    //cl = clock();
    auto start = std::chrono::steady_clock::now();
    for(int i = 0; i < num_threads; i++){

       threads[i] = std::thread([i, &n, &p, &running, &cond]{
	   //std::cout << "Thread " << i << " starting" << std::endl;
	   mpz_class x;// = rand()%n + 1;
	   mpz_class y;// = x;
	   mpz_class c;// = rand()&n + 1; 
	   mpz_class g = n;

	   //std::cout << "x: " << x << " y: " << y << " c: " << c << std::endl;

	   while( g == n){
	     x = rand()%n + 1;
	     y = x;
	     c = rand()&n + 1; 
	     g = 1;
	     std::cout << "Thread " << i << " LOOPED" << std::endl;
	     while(g==1){
	       if (!running.load()){
		 return;
	       }
	       
	       x = ( (x*x)%n + c)%n;    
	       y = ( (y*y)%n + c)%n;
	       y = ( (y*y)%n + c)%n;
	       g = gcd(abs(x-y),  n);
	     }
	   }
	   p = g;
	   std::cout << "THE LOOP FINISHED thread " << i << " found " << g << std::endl;
	   running = false;
	   cond.notify_all();

	   return;
	 });
       
       threads[i].detach();
    }

    std::unique_lock<std::mutex> lock{ m };
    // Wait until any of the threads signals us. p MUST be captured as a reference, since the value  in the lambda                                                                                        
    // is not updated every time we evaluate the wait                                                
    cond.wait(lock, [&p] {
        return p != 0;
    });

    
  q = n / p;

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

  std::cout << "Elapsed time in minutes : "
            << std::chrono::duration_cast<std::chrono::minutes>(end - start).count()
            << " mins" << std::endl;

  return 0;
}
