#define UNITY_BUILD

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "global_typedefs.c"
#include "celestial_body.c"
#include "planets.c"

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

/**
 * @brief Calculates the delta-v needed to burn from a circular or elliptical orbit to an elliptical orbit around the same body.
 * Assumes they share periapsis
 * @param orbitBase Base orbit, it must same body as the target
 * @param orbitTarget Target orbit, it must be elliptical and around the same body as the base
 * @return Delta-v Needed to burn from base orbit to target. (m/s)
 */
f64_t DeltaVCircToEllip(const Orbit *orbitBase, const Orbit *orbitTarget)
{
    if (!(0.0 < orbitTarget->Eccentricity && orbitTarget->Eccentricity < 1.0))
    {
        fprintf(stderr, "Target orbit must be elliptical\n");
        return -1.0; //TODO: better way to handle these errors, this is shit. Maybe a struct with bool or something
    }

    if (!(0.0 <= orbitBase->Eccentricity && orbitBase->Eccentricity < 1.0))
    {
        fprintf(stderr, "Base orbit must be elliptical or circular\n");
        return -1.0;
    }

    if (orbitBase->PrimaryBody == nullptr)
    {
        fprintf(stderr, "Base orbit must have a primary body\n");
        return -1.0;
    }

    if (orbitBase->Periapsis(orbitBase) != orbitTarget->Periapsis(orbitTarget))
    {
        fprintf(stderr, "Base and target orbits must have the same periapsis\n");
        return -1.0;
    }

    f64_t Mu = orbitBase->PrimaryBody->GravParam;
    f64_t r_peri = orbitBase->SMajorAxisM * (1.0 - orbitBase->Eccentricity);

    f64_t vBase = sqrt(Mu * (2.0 / r_peri - 1.0 / orbitBase->SMajorAxisM));
    f64_t vTarget = sqrt(Mu * (2.0 / r_peri - 1.0 / orbitTarget->SMajorAxisM));

    return vTarget - vBase;;
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

int main(void)
{
    f64_t r1 = 100000;
    f64_t a1 = 150000;
    f64_t r2 = 200000;

    Orbit orbit1 = CreateOrbit(&Planets[MOHO], r1, a1);
    Orbit orbit2 = CreateOrbit(&Planets[MOHO], r1, r2);

    printf("delta v: %lf\n", DeltaVCircToEllip(&orbit1, &orbit2));

    printf("Moho grav surf: %lf\n", Planets[MOHO].GravSurf);
    printf("Eve grav param: %lf\n", Planets[EVE].GravParam);

    return 0;
}