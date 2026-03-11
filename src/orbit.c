#ifndef UNITY_BUILD
#include "global_typedefs.c"
#include "celestial_body.c"
#endif
#include <stdint.h>
#include <math.h>

/**
* @brief Represents an orbit
 *
 * @param PrimaryBody pointer to the body the orbit is around
 * @param SMajorAxisM Semi-Major Axis (m), defines size of orbit
 * @param Eccentricity Defines shape of orbit (0 <= e < 1)
 */
typedef struct Orbit Orbit;

struct Orbit
{
    const CelestBody *PrimaryBody;

    f64_t SMajorAxisM;
    f64_t Eccentricity;

    f64_t (*OPeriod)       (const Orbit *orbit);
    f64_t (*OAltitude)     (const Orbit *orbit);
    f64_t (*Periapsis)     (const Orbit *orbit);
    f64_t (*Apoapsis)      (const Orbit *orbit);
    Orbit (*ResonantOrbit) (const Orbit *orbit, uint32_t satelliteCount, f64_t desiredAltitude);
};


f64_t CalcOrbitalPeriod(const Orbit *orbit)
{
    return 2.0 * M_PI * sqrt(pow(orbit->SMajorAxisM, 3) / orbit->PrimaryBody->GravParam);
}

f64_t calcPeriapsis(const Orbit *orbit)
{
    return orbit->SMajorAxisM * (1.0 - orbit->Eccentricity);
}

f64_t calcApoapsis(const Orbit *orbit)
{
    return orbit->SMajorAxisM * (1.0 + orbit->Eccentricity);
}

Orbit CreateOrbit(const CelestBody *primary, f64_t periapsis, f64_t apoapsis)
{
    periapsis += (f64_t)primary->EqRadiusM;
    apoapsis += (f64_t)primary->EqRadiusM;

    Orbit orbit = {
        .PrimaryBody = primary,
        .SMajorAxisM = (periapsis + apoapsis) / 2.0,
        .Eccentricity = (apoapsis - periapsis) / (apoapsis + periapsis),
        .OPeriod = CalcOrbitalPeriod,
        .OAltitude = nullptr,
        .Periapsis = calcPeriapsis,
        .Apoapsis = calcApoapsis,
        .ResonantOrbit = nullptr
    };

    return orbit;
}
