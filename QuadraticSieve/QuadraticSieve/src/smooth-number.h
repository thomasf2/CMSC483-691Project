/*
 * smooth-number.h
 *
 *  Created on: Jan 13, 2013
 *      Author: sky
 */

#ifndef SMOOTH_NUMBER_H_
#define SMOOTH_NUMBER_H_

#include "gmp.h"
#include "gmpxx.h"

class SmoothNumber {
public:
	mpz_class X;

	SmoothNumber ()
	{
		mpz_init(this->x_squared);
		mpz_init(this->exponent_vector);
//		mpz_init(this->non_zeor_exponents_vector);

		this->_is_initialized = true;
		this->smooth_base_size = 0;
	}

	SmoothNumber (const SmoothNumber& other)
	{
		this->X = other.X;
		mpz_init_set(this->x_squared, other.x_squared);
		mpz_init_set(this->exponent_vector, other.exponent_vector);
//		mpz_init(this->non_zeor_exponents_vector);

		this->_is_initialized = other._is_initialized;
		this->smooth_base_size = other.smooth_base_size;
	}

	SmoothNumber& operator=(const SmoothNumber& other)
	{
		this->X = other.X;
		mpz_init_set(this->x_squared, other.x_squared);
		mpz_init_set(this->exponent_vector, other.exponent_vector);
//		mpz_init(this->non_zeor_exponents_vector);

		this->_is_initialized = other._is_initialized;
		this->smooth_base_size = other.smooth_base_size;

		return *this;
	}

	~SmoothNumber ()
	{
		mpz_clear(this->x_squared);
		mpz_clear(this->exponent_vector);
//		mpz_clear(this->non_zeor_exponents_vector);
	}


	//Initialize the smooth number with the value of init_x
	//The exponent vector of this smooth number have nb_smooth_primes bits
	//one bit for each prime in the smooth base
	void Init (mpz_class init_x, unsigned long int nb_smooth_primes, mpz_class modulus);


	//Initialize the smooth number without the exponent vector (no smooth base
	//information is saved)
	void InitWithoutExponentVector (mpz_class init_x, mpz_class modulus);

	//Removes all powers of f from x_squared. Same logic as mpz_remove.
	unsigned long int RemovePowerOfFactor(mpz_t f);

	//Returns true if x_squared is a smooth number (=1 after calls to RemovePowerOfFactor)
	bool IsFullyFactoredOnSmoothBase();

	//Sets to 1 the bit corresponding to the i'th prime in the smooth base.
	void SetExponentVectorBit(unsigned long i);

	inline void SetNonZeroExponentsVectorBit(unsigned long i);

	const mpz_t& GetExponentVector ();
//	const mpz_t& GetNonZeroExponentsVector ();

	mpz_class GetXSquared () const;


private:
	mpz_class modulus_N;
	mpz_t x_squared;
	mpz_t exponent_vector;
	mpz_t non_zeor_exponents_vector;	//bit is set to one if corresponding prime is <> 0

	bool _is_initialized;
	unsigned long int smooth_base_size;
};



#endif /* SMOOTH_NUMBER_H_ */
