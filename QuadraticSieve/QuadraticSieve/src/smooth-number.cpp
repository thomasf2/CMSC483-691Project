#include "smooth-number.h"

void SmoothNumber::Init (mpz_class init_x, unsigned long int nb_smooth_primes,
		mpz_class modulus)
{
	if(!this->_is_initialized)
	{
		mpz_init(this->x_squared);
		mpz_init2(this->exponent_vector, nb_smooth_primes);
		mpz_set_ui(this->exponent_vector, 0);

		this->_is_initialized = true;
	}
	else
	{
		mpz_clear(this->exponent_vector);		//allocate new space for the exponent vector
		mpz_init2(this->exponent_vector, nb_smooth_primes);
		mpz_set_ui(this->exponent_vector, 0);
	}

	this->X = init_x;
	//Set x_squared to X*X - N
	mpz_pow_ui(this->x_squared, this->X.get_mpz_t (), 2);
	mpz_sub(this->x_squared, this->x_squared, modulus.get_mpz_t ());

	this->smooth_base_size = nb_smooth_primes;
	this->modulus_N = modulus;
}

void SmoothNumber::InitWithoutExponentVector (mpz_class init_x, mpz_class modulus)
{
	this->X = init_x;
	if(!this->_is_initialized)
	{
		mpz_init(this->x_squared);
		this->_is_initialized = true;
	}

	//Set x_squared to X*X - N
	mpz_pow_ui(this->x_squared, X.get_mpz_t (), 2);
	mpz_sub(this->x_squared, this->x_squared, modulus.get_mpz_t ());

	this->smooth_base_size = 0;
	this->modulus_N = modulus;
}


unsigned long int SmoothNumber::RemovePowerOfFactor(mpz_t f)
{
	return mpz_remove(this->x_squared, this->x_squared, f);
}


bool SmoothNumber::IsFullyFactoredOnSmoothBase()
{
	return mpz_cmp_ui(this->x_squared, 1) == 0;
}


void SmoothNumber::SetExponentVectorBit(unsigned long i)
{
	mpz_setbit(this->exponent_vector, i);
}

const mpz_t& SmoothNumber::GetExponentVector ()
{
	return this->exponent_vector;
}

mpz_class SmoothNumber::GetXSquared () const
{
	return mpz_class (this->x_squared);
}

void SmoothNumber::SetNonZeroExponentsVectorBit(unsigned long i)
{
	mpz_setbit(this->non_zeor_exponents_vector, i);
}
