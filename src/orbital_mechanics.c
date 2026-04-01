#ifndef UNITY_BUILD
#include "global_typedefs.c"
#include "error.c"
#include "orbit.c"
#endif

#include <math.h>
#include <stdio.h>

/**
* @brief Calculates the delta-v needed to burn from a circular or elliptical orbit to an elliptical orbit around the same body.
 * Assumes they share periapsis
 * @param orbitBase Base orbit, it must same body as the target
 * @param orbitTarget Target orbit, it must be elliptical and around the same body as the base
 * @param err Pointer to an OrbitError variable to store any error that occurs during the calculation
 * @return Delta-v Needed to burn from base orbit to target. (m/s)
 */
f64_t DeltaVCircToEllip(const Orbit *orbitBase, const Orbit *orbitTarget, ResonantError* err)
{
    if (!(0.0 < orbitTarget->Eccentricity && orbitTarget->Eccentricity < 1.0))
    {
        *err = RES_ERR_NOT_ELLIPTICAL;
        return -1.0;
    }

    if (!(0.0 <= orbitBase->Eccentricity && orbitBase->Eccentricity < 1.0))
    {
        *err = RES_ERR_NOT_ELLIPTICAL_OR_CIRCULAR;
        return -1.0;
    }

    if (orbitBase->PrimaryBody == nullptr)
    {
        *err = RES_ERR_MISSING_PRIMARY;
        return -1.0;
    }

    f64_t epsilon = 1e-6; //Fix for floating point errors
    if (fabs(orbitBase->Periapsis(orbitBase) - orbitTarget->Periapsis(orbitTarget)) > epsilon &&
         fabs(orbitBase->Periapsis(orbitBase) - orbitTarget->Apoapsis(orbitTarget)) > epsilon)
    {
        *err = RES_ERR_NOT_INTERSECTING;
        return -1.0;
    }

    f64_t mu = orbitBase->PrimaryBody->GravParam;
    f64_t r_peri = orbitBase->SMajorAxisM * (1.0 - orbitBase->Eccentricity);

    f64_t vBase = sqrt(mu * (2.0 / r_peri - 1.0 / orbitBase->SMajorAxisM));
    f64_t vTarget = sqrt(mu * (2.0 / r_peri - 1.0 / orbitTarget->SMajorAxisM));

    return fabs(vTarget - vBase);
}

Orbit CalcResonantOrbitProg(const Orbit *orbit, uint32_t satteliteCount, ResonantError* err)
{
    if (orbit->Apoapsis(orbit) > orbit->PrimaryBody->SOI)
    {
        *err = RES_ERR_APOAPSIS_OUTSIDE_SOI;
        return (Orbit){0};
    }

    if (satteliteCount < 3)
    {
        *err = RES_ERR_INVALID_SATELLITE_COUNT;
        return (Orbit){0};
    }

    const f64_t period = orbit->OPeriod(orbit);
    const f64_t mu =  orbit->PrimaryBody->GravParam;
    const f64_t periap =orbit->Periapsis(orbit);
    satteliteCount++;

    f64_t res_period = period * ((f64_t)satteliteCount / (satteliteCount - 1.0)) ;
    f64_t res_sma = cbrt((pow(res_period, 2) * mu) / (4.0 * pow(M_PI, 2)));
    f64_t res_ecc = 1.0 - (periap / res_sma);

    Orbit resonantOrbit = {
        .PrimaryBody = orbit->PrimaryBody,
        .SMajorAxisM = res_sma,
        .Eccentricity = res_ecc,
        .OPeriod = orbit->OPeriod,
        .OAltitude = orbit->OAltitude,
        .Periapsis = orbit->Periapsis,
        .Apoapsis = orbit->Apoapsis,
        .PeriapsisHeight = orbit->PeriapsisHeight,
        .ApoapsisHeight = orbit->ApoapsisHeight,
    };

    if (resonantOrbit.Apoapsis(&resonantOrbit) > resonantOrbit.PrimaryBody->SOI)
    {
        *err = RES_ERR_APOAPSIS_OUTSIDE_SOI;
        return (Orbit){0};
    }

    return resonantOrbit;
}

Orbit CalcResonantOrbitRetr(const Orbit *orbit, uint32_t satteliteCount, ResonantError* err)
{
    if (orbit->Apoapsis(orbit) > orbit->PrimaryBody->SOI)
    {
        *err = RES_ERR_APOAPSIS_OUTSIDE_SOI;
        return (Orbit){0};
    }

    if (satteliteCount < 3)
    {
        *err = RES_ERR_INVALID_SATELLITE_COUNT;
        return (Orbit){0};
    }

    const f64_t period = orbit->OPeriod(orbit);
    const f64_t mu =  orbit->PrimaryBody->GravParam;
    const f64_t apoap =orbit->Apoapsis(orbit);

    f64_t res_period = period * ((satteliteCount - 1.0) / satteliteCount) ;
    f64_t res_sma = cbrt((pow(res_period, 2) * mu) / (4.0 * pow(M_PI, 2)));
    f64_t res_ecc =  (apoap / res_sma) - 1.0;

    Orbit resonantOrbit = {
        .PrimaryBody = orbit->PrimaryBody,
        .SMajorAxisM = res_sma,
        .Eccentricity = res_ecc,
        .OPeriod = orbit->OPeriod,
        .OAltitude = orbit->OAltitude,
        .Periapsis = orbit->Periapsis,
        .Apoapsis = orbit->Apoapsis,
        .PeriapsisHeight = orbit->PeriapsisHeight,
        .ApoapsisHeight = orbit->ApoapsisHeight,
    };

    if (resonantOrbit.Periapsis(&resonantOrbit) <= (f64_t)resonantOrbit.PrimaryBody->EqRadiusM)
    {
        *err = RES_ERR_PERIAPSIS_INTERSECTS_SURFACE;
        return (Orbit){0};
    }

    if (resonantOrbit.Periapsis(&resonantOrbit) <= (f64_t)resonantOrbit.PrimaryBody->AtmHeightM + (f64_t)resonantOrbit.PrimaryBody->EqRadiusM)
    {
        *err = RES_ERR_PERIAPSIS_BELOW_ATMOSPHERE;
        return (Orbit){0};
    }

    return resonantOrbit;
}

bool LineofSight(const Orbit *orbit, uint32_t satelliteCount, LosError *err)
{
    if (!ValidateLos(orbit, satelliteCount, err))
        return false;

    const f64_t SmajA = orbit->SMajorAxisM;
    const f64_t EqRad = orbit->PrimaryBody->EqRadiusM;

    const f64_t Theta = 2.0 * M_PI / (f64_t)satelliteCount;
    const f64_t MinDist = SmajA * cos(Theta / 2.0);

    return MinDist > EqRad;
}

bool AtmosphericOccusion(const Orbit *orbit, uint32_t satelliteCount, LosError *err)
{
    if (!ValidateLos(orbit, satelliteCount, err))
        return false;

    const f64_t SmajA = orbit->SMajorAxisM;
    const f64_t AtmH = orbit->PrimaryBody->EqRadiusM + orbit->PrimaryBody->AtmHeightM;

    const f64_t Theta = 2.0 * M_PI / (f64_t)satelliteCount;
    const f64_t MinDist = SmajA * cos(Theta / 2.0);

    return MinDist > AtmH;
}