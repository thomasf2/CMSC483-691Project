/*
    Algo:   Pollar Rho
    Task:   Prime Factorization of an Integer
    Author: Chandan Mittal
*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"


const int MAX = 100000000;
const int LMT = 10000;
bool isprime(long long int num){
   bool flag = true;

   for(long long int i = 2; i <= num / 2; i++) {
      if(num % i == 0) {
         flag = false;
         break;
      }
   }
   if (flag==true)
     return true;

   return false;
}


int abso(long int a)     //fn to return absolute value
{
    return a>0?a:-a;
}
long long int gcd(long long int a,long long int b)    //Euclidean GCD recursive fn
{
  printf("a: %lld | b: %lld\n", a, b);
    if(b==0)
        return a;
    else
        return gcd(b,a%b);
}

long long int pollard_rho(long long int n)  //pollard rho implementation
{
    if(n%2==0)
        return 2;

    long long int x = rand()%n+1;
    long long int c = rand()%n+1;
    long long int y = x;
    long long int g = 1;
    printf("POLLARD RHO\n");
    printf("INITIAL: x: %lld | y: %lld | c: %lld | g %lld\n", x,y,c,g);
    //fn is f(x) = x*x + c
    while(g==1)
    {
      printf("x: %lld | y: %lld | c: %lld | g %lld\n", x,y,c,g);
      x = ((x*x)%n + c)%n;    
      y = ((y*y)%n + c)%n;
      y = ((y*y)%n + c)%n;
      printf("CALLING GCD\n");
      g = gcd(abso(x-y),n);
      printf("\n");
    }
    return g;
}
`
long long int factors[MAX/100], total;

void factorize(long long int n)   //fn to factorize the number
{
  printf("FACTORIZING: %lld\n", n);

    if(n == 1)
        return;

    if(isprime(n))      //if n is prime,store it
    {
        factors[total++] = n;
        return;
    }
    long int divisor = pollard_rho(n);   //get a divisor of n
    printf("divisor: %ld\n\n", divisor);
    factorize(divisor);
    factorize(n/divisor);
}

int main()  //Driver Program
{
    srand(time(NULL));

    long int n;
    scanf("%ld",&n);

    total = 0;
    //printf("FACTORIZING\n");
    factorize(n);

    printf("THE FACTORS ARE:\n");
    for (int i = 0; i < total; ++i)
    {
        printf("%lld ",factors[i] );
    }
    printf("\n");
	return 0;
}
