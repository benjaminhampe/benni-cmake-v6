/// @author Benjamin Hampe <benjaminhampe@gmx.de>

#ifndef DE_HAMPE_RAINBOWCOLOR_HPP
#define DE_HAMPE_RAINBOWCOLOR_HPP

#include <de/hampe/Types.hpp>

namespace de {
namespace hampe {

struct RainbowColor
{
   /// Rainbow
   /// lambda in range 380 ( blue ) ... 780nm ( red )
   static glm::vec4
   computeFromWaveLength( float64_t lambda, float64_t gamma, float32_t alpha = 1.0f )
   {
       glm::dvec3 color;

      // setze limits ? ? [380, 780]
       // if (lambda < 380.0) lambda = 380.0;
       // if (lambda > 780.0) lambda = 780.0;
       lambda = glm::clamp( lambda, 380.0, 780.0 );

      // Jeder Farbkanal hat seine eigene dem (sichtbaren) Sonnenspektrum angelehnt interpolierte Kurve:
       if ((lambda >= 380.0) && (lambda < 440.0)) { color.r = (440.0-lambda)/(440.0-380.0); color.g = 0.0; color.b = 1.0; }
       else if ((lambda >= 440.0) && (lambda < 490.0)) { color.r = 0.0; color.g = (lambda-440.0)/(490.0-440.0); color.b = 1.0; }
       else if ((lambda >= 490.0) && (lambda < 510.0)) { color.r = 0.0; color.g = 1.0; color.b = (510.0-lambda)/(510.0-490.0); }
       else if ((lambda >= 510.0) && (lambda < 580.0)) { color.r = (lambda-510.0)/(580.0-510.0); color.g = 1.0; color.b = 0.0; }
       else if ((lambda >= 580.0) && (lambda < 645.0)) { color.r = 1.0; color.g = (645.0-lambda)/(645.0-580.0); color.b = 0.0; }
       else /* if ((lambda >= 645.0) && (lambda <= 780.0)) */ { color.r = 1.0; color.g = 0.0; color.b = 0.0; }

       // reduce intesity at the borders
       float64_t f;
       if ((lambda >= 380.0) && (lambda < 420.0)) { f = 0.3 + 0.7*(lambda-380.0)/(420.0-380.0); }
       else if ((lambda >= 420.0) && (lambda <= 700.0)) { f = 1.0; }
       else /* if ((lambda > 700.0) && (lambda <= 780.0)) */ { f = 0.3 + 0.7*(780.0-lambda)/(780.0-700.0); }

       // eigentliche Korrektur
       if ( f != 1.0 )
      {
         color *= f;
      }

       // Gamma Korrektur
       if (gamma!=1.0)
      {
         color.r = ::pow( color.r, gamma );
         color.g = ::pow( color.g, gamma );
         color.b = ::pow( color.b, gamma );
      }

       return glm::clamp( glm::vec4( glm::vec3( color ), alpha ), glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
   }

   /// Rainbow
   /// lambda in range 380 ( blue ) ... 780nm ( red )
   static glm::vec4
   computeFromT( float32_t t, float32_t gamma = 1.0f, float32_t alpha = 1.0f )
   {
      return computeFromWaveLength( 380.0f + 400.0f * glm::clamp( t, 0.0f, 1.0f ), gamma, alpha );
   }

};

} // end namespace hampe
} // end namespace de

#endif // DE_HAMPE_RAINBOWCOLOR_HPP
