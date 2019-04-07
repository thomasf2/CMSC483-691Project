#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "gmp.h"
#include "gmpxx.h"
//#include "pthread.h"
#include <thread>
#include <atomic>
#include <ctime>

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

/*
mpz_class gcd(mpz_class a,mpz_class b){
  mpz_class t;
  while(b != 0){
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}
*/
std::atomic<bool> exit_thread_flag{false};
clock_t cl;

void*  pollard_rho(mpz_class x, mpz_class y, mpz_class c, mpz_class n )  //pollard rho implementation
{
  if(n%2==0)
    printf("EVEN\n");
  else{
    //mpz_class x = rand()%n+1;
    //mpz_class c = rand()%n+1;
    //mpz_class y = x;
    mpz_class g = 1;
    //printf("POLLARD RHO\n");
    //printf("INITIAL: x: %d | y: %d | c: %d | g: %d\n", x,y,c,g);
    int iteration = 0;
    while(g==1 and not exit_thread_flag){
      x = ( (x*x)%n + c)%n;    
      y = ( (y*y)%n + c)%n;
      y = ( (y*y)%n + c)%n;
      g = gcd(abso(x-y),  n);

    }

    if(exit_thread_flag == false){
      double duration = ( clock() - cl )/(double)CLOCKS_PER_SEC;
      gmp_printf("Hey, I Found it! The factor is %Zd and my id is %d\n", g, std::this_thread::get_id());
      printf("That took %f seconds\n", duration);
    }

    exit_thread_flag = true;
    
    
  }
    //return g;
}


int main(int argc, char *argv[])  //Driver Program
{
    srand(time(NULL));

    mpz_class n(argv[1]);
    mpz_class p;
    mpz_class q;
    mpz_class returnVal[8];

    mpz_class x;// = rand()%n+1;
    mpz_class c;// = rand()%n+1;
    mpz_class y;// = x;
    
    std::thread threads[8];
    gmp_printf("Starting threads for number %Zd\n", n);
    cl = clock();
    for(int i = 0; i < 8; i++){
       x = rand()%n+1;
       c = rand()%n+1;
       y = x;

       threads[i] = std::thread(pollard_rho, x, y, c, n);
      
    }

    for(int i = 0; i < 8; i++){
      //      printf("Joining threads[%d]\n", i);
      threads[i].join();
      
    }
    
    /*
    gmp_printf("Read Input number: %Zd\n", n);
    gmp_printf("srarting pollard rho\n");
    p = pollard_rho(n);
    gmp_printf("done pollard rho, finding q by division\n");
    q = n / p;

    //if( !(isprime(p) or isprime(q))){
    //  printf("SOMETHING WENT WRONG\n");
    // }
    gmp_printf("n: %Zd\n", n);
    gmp_printf("p: %Zd\n", p);
    gmp_printf("q: %Zd\n", q);
    */
      
	return 0;
}
