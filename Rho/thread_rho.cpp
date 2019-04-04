#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "gmp.h"
#include "gmpxx.h"
#include "assert.h"
@include "pthread.h"

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
mpz_class pollard_rho(mpz_class n)  //pollard rho implementation
{
    if(n%2==0)
        return 2;

    mpz_class x = rand()%n+1;
    mpz_class c = rand()%n+1;
    mpz_class y = x;
    mpz_class g = 1;
    //printf("POLLARD RHO\n");
    //printf("INITIAL: x: %d | y: %d | c: %d | g: %d\n", x,y,c,g);
    int iteration = 0;
    while(g==1){
      x = ( (x*x)%n + c)%n;    
      y = ( (y*y)%n + c)%n;
      y = ( (y*y)%n + c)%n;
      //printf("x: %d | y: %d | c: %d | g %d\n", x,y,c,g);
      
      g = gcd(abso(x-y),n);
      //gmp_printf("Recurrence depth %d, g %Zd iteration %d\n", recur_depth, g, iteration);
      //recur_depth = 0;
      //iteration ++;
      //printf("GCD of %d and %d = %d\n", abso(x-y), n, g);
      //printf("\n");
    }
    return g;
}




int main(int argc, char *argv[])  //Driver Program
{
    srand(time(NULL));

    mpz_class n(argv[1]);
    mpz_class p;
    mpz_class q;
    int thread_id[8];
    pthread_t threads[8];
    
    for(int i = 0 i < 8; i++){
      thread_id[i] = pthread_create(&threads[i], NULL, pollard_rho, n); 
    }
    
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
      
      
	return 0;
}
