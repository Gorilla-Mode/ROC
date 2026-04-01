#include "error.h"
#ifndef UNITY_BUILD
#include "global_typedefs.c"
#include "celestial_body.c"
#include "error.c"
#endif
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
    f64_t (*PeriapsisHeight)(const Orbit *orbit);
    f64_t (*ApoapsisHeight)(const Orbit *orbit);
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

f64_t calcPeriapsisHeight(const Orbit *orbit)
{
    return calcPeriapsis(orbit) - orbit->PrimaryBody->EqRadiusM;
}

f64_t calcApoapsisHeight(const Orbit *orbit)
{
    return calcApoapsis(orbit) - orbit->PrimaryBody->EqRadiusM;
}

f64_t calcOrbitalAltitude(const Orbit *orbit)
{
    return orbit->SMajorAxisM - (f64_t)orbit->PrimaryBody->EqRadiusM;
}

Orbit CreateOrbitEllipse(const CelestBody *primary, f64_t periapsis, f64_t apoapsis, OrbitError *err)
{
    periapsis += (f64_t)primary->EqRadiusM;
    apoapsis += (f64_t)primary->EqRadiusM;

    Orbit orbit = {
        .PrimaryBody = primary,
        .SMajorAxisM = (periapsis + apoapsis) / 2.0,
        .Eccentricity = (apoapsis - periapsis) / (apoapsis + periapsis),
        .OPeriod = CalcOrbitalPeriod,
        .OAltitude = calcOrbitalAltitude,
        .Periapsis = calcPeriapsis,
        .Apoapsis = calcApoapsis,
        .PeriapsisHeight = calcPeriapsisHeight,
        .ApoapsisHeight = calcApoapsisHeight,
    };

    if (!ValidateOrbit(&orbit, err))
        return (Orbit){0};

    return orbit;
}

Orbit CreateOrbitCircularSmj(const CelestBody *primary, f64_t SMajorAxis, OrbitError *err)
{
    Orbit orbit = {
        .PrimaryBody = primary,
        .SMajorAxisM = SMajorAxis,
        .Eccentricity = 0.0,
        .OPeriod = CalcOrbitalPeriod,
        .OAltitude = calcOrbitalAltitude,
        .Periapsis = calcPeriapsis,
        .Apoapsis = calcApoapsis,
        .PeriapsisHeight = calcPeriapsisHeight,
        .ApoapsisHeight = calcApoapsisHeight,
    };

    if (!ValidateOrbit(&orbit, err))
        return (Orbit){0};

    return orbit;
}

Orbit CreateOrbitCircularAlt(const CelestBody *primary, f64_t altitude, OrbitError *err)
{
    Orbit orbit = {
        .PrimaryBody = primary,
        .SMajorAxisM = altitude + (f64_t)primary->EqRadiusM,
        .Eccentricity = 0.0,
        .OPeriod = CalcOrbitalPeriod,
        .OAltitude = calcOrbitalAltitude,
        .Periapsis = calcPeriapsis,
        .Apoapsis = calcApoapsis,
        .PeriapsisHeight = calcPeriapsisHeight,
        .ApoapsisHeight = calcApoapsisHeight,
    };

    if (!ValidateOrbit(&orbit, err))
        return (Orbit){0};

    return orbit;
}