#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef float f32_t;
typedef double f64_t; //The fuck even is double, GIVE THE BITS
typedef __float128 f128_t;

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
 * @brief Represents an orbit
 *
 * @param PrimaryBody pointer to the body the orbit is around
 * @param SMajorAxisM Semi-Major Axis (m), defines size of orbit
 * @param Eccentricity Defines shape of orbit (0 <= e < 1)
 */
typedef struct Orbit Orbit;

/**
 * @brief Keys of planets in the kerbol system, ordered.
 */
typedef enum
{
    MOHO,
    EVE,
    GILLY,
    KERBIN,
    MUN,
    MINMUS,
    DUNA,
    IKE,
    DRES,
    JOOL,
    LAYTHE,
    VALL,
    TYLO,
    BOP,
    POL,
    EELOO
}CelestBodyKey;

struct CelestBody
{
    f64_t GravSurf;
    f64_t GravParam;
    uint64_t EqRadiusM;
    int32_t AtmHeightM;
};

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

f64_t DeltaV(const Orbit *orbitBase, const Orbit *orbitTarget)
{
    f64_t Mu = orbitBase->PrimaryBody->GravParam;
    f64_t r1 = orbitBase->SMajorAxisM;
    f64_t r2 = orbitTarget->SMajorAxisM * (1.0 + orbitTarget->Eccentricity);

    f64_t dv1 =  sqrt(Mu / r1) * (sqrt(2.0 * r2 / (r1 + r2)) - 1.0);
    f64_t dv2 = sqrt(Mu / r2) * (1.0 - sqrt(2.0 * r1 / (r1 + r2)));

    return dv1 + dv2;
}

Orbit CreateOrbit(const CelestBody *primary, f64_t periapsis, f64_t apoapsis)
{
    Orbit orbit = {
        .PrimaryBody = primary,
        .SMajorAxisM = (periapsis + apoapsis) / 2.0,
        .Eccentricity = (apoapsis - periapsis) / (apoapsis + periapsis),
        .OPeriod = CalcOrbitalPeriod,
        .OAltitude = nullptr,
        .Periapsis = nullptr,
        .Apoapsis = nullptr,
        .ResonantOrbit = nullptr
    };

    return orbit;
}

CelestBody Planets[] = {
    {
        .GravSurf = 2.70,
        .GravParam = 1.6860938e11,
        .EqRadiusM = 250000,
        .AtmHeightM = -1
    },
    {
        .GravSurf = 16.70,
        .GravParam = 8.1717302e12,
        .EqRadiusM = 700000,
        .AtmHeightM = 90000
    }
};

int main(void)
{
    f64_t mohoRadius = (f64_t)Planets[MOHO].EqRadiusM;
    f64_t r1 = 100000 + mohoRadius;
    f64_t r2 = 200000 + mohoRadius;

    Orbit orbit1 = CreateOrbit(&Planets[MOHO], r1, r1);
    Orbit orbit2 = CreateOrbit(&Planets[MOHO], r1, r2);

    printf("delta v: %lf\n", DeltaV(&orbit1, &orbit2));

    printf("Moho grav surf: %lf\n", Planets[MOHO].GravSurf);
    printf("Eve grav param: %lf\n", Planets[EVE].GravParam);

    return 0;
}