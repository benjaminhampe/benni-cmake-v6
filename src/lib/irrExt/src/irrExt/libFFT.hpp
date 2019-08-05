#ifndef IRREXT_FREE_FFT_HPP
#define IRREXT_FREE_FFT_HPP

#include <complex>
#include <iostream>
#include <valarray>
 
namespace irrExt { 

	typedef float float32_t;
	typedef double float64_t; 
	typedef std::complex< float64_t > FFT_Value_t;
	typedef std::valarray< FFT_Value_t > FFT_Array_t;
	 
	float64_t const CONST_PI = 3.141592653589793238460;

	// Cooley–Tukey FFT (in-place, divide-and-conquer)
	// Higher memory requirements and redundancy although more intuitive
	void fft( FFT_Array_t & v );
	 
	// inverse fft (in-place)
	void ifft( FFT_Array_t & v );
	
#if 0
	// Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
	// Better optimized but less intuitive
	void fft_II( FFT_Array_t & v );
	
	// inverse fft (in-place)
	void ifft_II( FFT_Array_t & v );
#endif

} // end namespace irrExt

#endif