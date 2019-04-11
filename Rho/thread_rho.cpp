#include "stdio.h"
#include "stdlib.h"
#include <gmpxx.h>
//#include "pthread.h"
#include <thread>
#include <atomic>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <iostream>
/*
bool isprime(mpz_class n){
  if (n == 2){return true; }
  if (n == 3){return true; }
  if (n % 2 == 0){return false;}
  if (n % 3 == 0){return false;}

  mpz_class i = 5;
  mpz_class w = 2;

  while (i * i <= n){
    if (n % i == 0){printf("IS NOT THE FATHER\n");return false;}
    i += w;
    w = 6 - w;
  }
  printf("IS THE FATHER\n");
  return true;

}
*/

mpz_class abso(mpz_class a)     //fn to return absolute value
{
    return a>0?a:-a;
}
int recur_depth = 0;

mpz_class gcd(mpz_class a,mpz_class b)    //Euclidean GCD recursive fn
{
    if(b==0)
        return a;
    else{
      return gcd(b,a%b);
    }
}



int main(int argc, char *argv[])  //Driver Program
{
    srand(time(NULL));
    if(argc != 3){
      std::cout << "Two arguments plz" << std::endl;
      std::cout << "1: the number to factor, 2: the number of threads" << std::endl;
      return -1;
    }
    mpz_class n(argv[1]);
    int num_threads = atoi(argv[2]);
    mpz_class p = 0;
    mpz_class q;

    std::mutex m;
    std::condition_variable cond;
    std::atomic<bool> running(true);

    if(n%2==0){
      printf("EVEN\n");
      return -1;
    }
    
    std::thread threads[num_threads];
    std::cout << "Starting threads for number " << n << std::endl;
    
    //cl = clock();
    for(int i = 0; i < num_threads; i++){

       threads[i] = std::thread([i, &n, &p, &running, &cond]{
	   std::cout << "Thread " << i << " starting" << std::endl;
	   mpz_class x = rand()%n + 1;
	   mpz_class y = x;
	   mpz_class c = rand()&n + 1; 
	   mpz_class g = 1;

	   //std::cout << "x: " << x << " y: " << y << " c: " << c << std::endl;

	   while(g==1){
	     if (!running.load()){
	       return;
	     }
	     
	     x = ( (x*x)%n + c)%n;    
	     y = ( (y*y)%n + c)%n;
	     y = ( (y*y)%n + c)%n;
	     g = gcd(abso(x-y),  n);
	     
	   }
	   std::cout << "THE LOOP FINISHED thread " << i << " found " << g << std::endl;
	   running = false;
	   cond.notify_all();
	   p = g;
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

    std::cout << "THE END I GUESS"<< std::endl;
    std::cout << "We Found " << p << " and " << n/p <<  std::endl;
    
      
    return 0;
}
