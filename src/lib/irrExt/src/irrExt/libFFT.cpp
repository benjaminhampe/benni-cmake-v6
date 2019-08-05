#include "libFFT.hpp"
 
namespace irrExt { 
 
// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft( FFT_Array_t & v )
{
   size_t const n = v.size();
   if ( n <= 1 ) return;
 
	// divide
	FFT_Array_t evenButterfly = v[ std::slice( 0, n / 2, 2 ) ];
	FFT_Array_t  oddButterfly = v[ std::slice( 1, n / 2, 2 ) ];

	// conquer
	fft( evenButterfly );
	fft( oddButterfly );

	float64_t const CONST_TWO_PI_OVER_MINUS_N = -2. * CONST_PI / float64_t( n );

	// combine
	for ( size_t k = 0; k < n / 2; ++k )
	{
		FFT_Value_t  t = std::polar( 1.0, CONST_TWO_PI_OVER_MINUS_N * float64_t( k ) ) * oddButterfly[ k ];
		v[ k         ] = evenButterfly[ k ] + t; // symmetric about x=0
		v[ k + n / 2 ] = evenButterfly[ k ] - t; // symmetric about x=0
	}
}
 
// inverse fft (in-place)
void ifft( FFT_Array_t & v )
{
    // conjugate the complex numbers
    v = v.apply( std::conj );
 
    // forward fft
    fft( v );
 
    // conjugate the complex numbers again
    v = v.apply( std::conj );
 
    // scale the numbers
    v /= v.size();
}
 
/*

// Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
// Better optimized but less intuitive
// !!! Warning : in some cases this code make result different from not optimased version above (need to fix bug)
// The bug is now fixed @2017/05/30 
void fft_II( FFT_Array_t & v )
{
	// DFT
	uint32_t const n = v.size();
	uint32_t k = n;
	
	float64_t const thetaT = 3.14159265358979323846264338328L / n;
	FFT_Value_t phiT = FFT_Value_t( ::cos( thetaT ), -::sin( thetaT ) );
	FFT_Value_t s;
	while ( k > 1 )
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		s = 1.0L;
		for ( uint32_t l = 0; l < k; ++l )
		{
			for ( uint32_t a = l; a < N; a += n )
			{
				uint32_t b = a + k;
				FFT_Value_t t = v[ a ] - v[ b ];
				v[ a ] += v[ b ];
				v[ b ] = t * s;
			}
			s *= phiT;
		}
	}
	// Decimate
	uint32_t m = (uint32_t)::log2( n );
	
	for ( uint32_t a = 0; a < n; ++a )
	{
		uint32_t b = a;
		
		// Reverse bits
		b = ( ( ( b & 0xaaaaaaaa ) >> 1 ) | ( ( b & 0x55555555 ) << 1 ) );
		b = ( ( ( b & 0xcccccccc ) >> 2 ) | ( ( b & 0x33333333 ) << 2 ) );
		b = ( ( ( b & 0xf0f0f0f0 ) >> 4 ) | ( ( b & 0x0f0f0f0f ) << 4 ) );
		b = ( ( ( b & 0xff00ff00 ) >> 8 ) | ( ( b & 0x00ff00ff ) << 8 ) );
		b = ( ( b >> 16 ) | ( b << 16 ) ) >> ( 32 - m );
		if ( b > a )
		{
			FFT_Value_t t = v[ a ];
			v[ a ] = v[ b ];
			v[ b ] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//Complex f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
}

void ifft_II( FFT_Array_t & v )
{
    // conjugate the complex numbers
    v = v.apply( std::conj );
 
    // forward fft_II
    fft_II( v );
 
    // conjugate the complex numbers again
    v = v.apply( std::conj );
 
    // scale the numbers
    v /= v.size();
}

*/

} // end namespace irrExt

#ifdef FREE_FFT_TEST

int main()
{
    FFT_Array_t const test[] = { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
    FFT_Array_t data(test, 8);
	 
    for ( int i = 0; i < 8; ++i )
    {
        std::cout << "data[" << i << "] = " << data[i] << std::endl;
    }
	 
    // forward fft
    fft( data );
	 
    for ( int i = 0; i < 8; ++i )
    {
        std::cout << "fft[" << i << "] = " << data[i] << std::endl;
    }
 
    // inverse fft
    ifft( data );
 
    for ( int i = 0; i < 8; ++i )
    {
        std::cout << "ifft[" << i << "] = " << data[i] << std::endl;
    }
    return 0;
}

#endif
