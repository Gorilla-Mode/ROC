#ifndef UNITY_BUILD
#include "global_typedefs.c"
#endif

#include <stdint.h>

/**
* @brief Represents a celestial body
 *
 * @param GravSurf Surface gravity of the body (m/s^2)
 * @param EqRadiusM Equatorial radius (m), approx distance from surface to center
 * @param AtmHeightM Height above surface where atmosphere ends (m)
 * @param GravParam Standard gravitational parameter (Mu) (m^3/s^2)
 */
typedef struct CelestBody CelestBody;

/**
 * @brief Keys of planets in the kerbol system, ordered.
 */
typedef enum CelestBodyKey CelestBodyKey;

struct CelestBody
{
    f64_t GravSurf;
    f64_t GravParam;
    uint64_t EqRadiusM;
    int32_t AtmHeightM;
    f64_t SOI;
};

enum CelestBodyKey
{
    MOHO, EVE, GILLY, KERBIN,
    MUN, MINMUS, DUNA, IKE,
    DRES, JOOL, LAYTHE, VALL,
    TYLO, BOP, POL, EELOO
};