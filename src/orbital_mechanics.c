#ifndef UNITY_BUILD
#include "global_typedefs.c"
#include "orbit.c"
#endif

#include <stdio.h>
#include <math.h>

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

    f64_t epsilon = 1e-6; //Fix for floating point errors
    if (fabs(orbitBase->Periapsis(orbitBase) - orbitTarget->Periapsis(orbitTarget)) > epsilon &&
         fabs(orbitBase->Periapsis(orbitBase) - orbitTarget->Apoapsis(orbitTarget)) > epsilon)
    {
        fprintf(stderr, "Base and target orbits must have the same periapsis\n");
        return -1.0;
    }

    f64_t mu = orbitBase->PrimaryBody->GravParam;
    f64_t r_peri = orbitBase->SMajorAxisM * (1.0 - orbitBase->Eccentricity);

    f64_t vBase = sqrt(mu * (2.0 / r_peri - 1.0 / orbitBase->SMajorAxisM));
    f64_t vTarget = sqrt(mu * (2.0 / r_peri - 1.0 / orbitTarget->SMajorAxisM));

    return fabs(vTarget - vBase);
}

Orbit CalcResonantOrbitProg(const Orbit *orbit, uint32_t satteliteCount)
{
    f64_t period = orbit->OPeriod(orbit);
    f64_t mu =  orbit->PrimaryBody->GravParam;
    f64_t periap =orbit->Periapsis(orbit);
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

    return resonantOrbit;
}

Orbit CalcResonantOrbitRetr(const Orbit *orbit, uint32_t satteliteCount)
{
    f64_t period = orbit->OPeriod(orbit);
    f64_t mu =  orbit->PrimaryBody->GravParam;
    f64_t apoap =orbit->Apoapsis(orbit);

    f64_t res_period = period * ((satteliteCount - 1.0) / satteliteCount) ;
    f64_t res_sma = cbrt((pow(res_period, 2) * mu) / (4.0 * pow(M_PI, 2)));
    f64_t res_ecc =  apoap / res_sma - 1.0;

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

    return resonantOrbit;
}