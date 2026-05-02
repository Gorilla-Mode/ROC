#ifndef UNITY_BUILD
#include "global_typedefs.c"
#include "orbit.c"
#endif

#include "error.h"

const char* ResonantErrorToString(ResonantError err)
{
    static const char* errorStrings[] = {
        [RES_SUCCESS]                          = "Success",
        [RES_ERR_PERIAPSIS_INTERSECTS_SURFACE] = "Periapsis intersects surface",
        [RES_ERR_INVALID_SATELLITE_COUNT]      = "Satellite count must be at least 3",
        [RES_ERR_APOAPSIS_OUTSIDE_SOI]         = "Outside SOI, lower apoapsis",
        [RES_ERR_MISSING_PRIMARY]              = "Missing primary body",
        [RES_ERR_NOT_ELLIPTICAL]               = "Target orbit is not elliptical",
        [RES_ERR_NOT_ELLIPTICAL_OR_CIRCULAR]   = "Base orbit is not elliptical or circular",
        [RES_ERR_NOT_INTERSECTING]             = "Periapsis or apoapsis don't intersect base",
        [RES_ERR_PERIAPSIS_BELOW_ATMOSPHERE]   = "Below atmosphere, raise periapsis",
    };

    if (err >= (sizeof(errorStrings) / sizeof(errorStrings[0]))) {
        return "Unknown error";
    }
    return errorStrings[err];
}

const char* LosErrorToString(LosError err)
{
    static const char* errorStrings[] = {
        [LOS_SUCCESS]                      = "Success",
        [LOS_ERR_INVALID_SATELLITE_COUNT]  = "Satellite count must be at least 3",
        [LOS_ERR_MISSING_PRIMARY]          = "Missing primary body",
        [LOS_ERR_ORBIT_NOT_CIRCULAR]       = "Orbit is not circular",

    };

    if (err >= (sizeof(errorStrings) / sizeof(errorStrings[0])))
        return "Unknown error";

    return errorStrings[err];
}

const char* OrbitErrorToString(OrbitError err)
{
    static const char* errorStrings[] = {
        [ORBIT_SUCCESS]                  = "Success",
        [ORBIT_ERR_MISSING_PRIMARY]      = "Missing primary body",
        [ORBIT_ERR_BELOW_ATMOSPHERE]     = "Below atmosphere, raise periapsis",
        [ORBIT_ERR_OUTSIDE_SOI]          = "Outside SOI, lower apoapsis",
        [ORBIT_ERR_INTERSECTING_SURFACE] = "Intersects surface, raise periapsis",
    };

    if (err >= (sizeof(errorStrings) / sizeof(errorStrings[0])))
        return "Unknown error";

    return errorStrings[err];
}

/**
 * Validates parameters for Line of Sight (LOS) calculations.
 * Validates:
 * - LOS_ERR_MISSING_PRIMARY
 * - LOS_ERR_ORBIT_NOT_CIRCULAR
 * - LOS_ERR_INVALID_SATELLITE_COUNT
 * @param orbit Pointer to the orbit to validate
 * @param satelliteCount Number of satellites in the constellation
 * @param err Pointer to LosError enum to store validation result
 * @return true if all parameters are valid, false otherwise
 */
static bool ValidateLosParams(const Orbit *orbit, uint32_t satelliteCount, LosError *err)
{
    if (orbit->PrimaryBody == nullptr)
        *err = LOS_ERR_MISSING_PRIMARY;

    if (orbit->Eccentricity != 0.0)
        *err = LOS_ERR_ORBIT_NOT_CIRCULAR;

    if (satelliteCount < 3)
        *err = LOS_ERR_INVALID_SATELLITE_COUNT;

    if (*err != LOS_SUCCESS)
        return false;
    return true;
}

/**
 * Validates orbital parameters for general orbit validity.
 * Validates:
 * - ORBIT_ERR_MISSING_PRIMARY
 * - ORBIT_ERR_INTERSECTING_SURFACE
 * - ORBIT_ERR_OUTSIDE_SOI
 * - ORBIT_ERR_BELOW_ATMOSPHERE
 * @param orbit Pointer to the orbit to validate
 * @param err Pointer to OrbitError enum to store validation result
 * @return true if the orbit is valid, false otherwise
 */
static bool ValidateOrbit(const Orbit *orbit, OrbitError *err)
{
    if (orbit->PrimaryBody == nullptr)
    {
        *err = ORBIT_ERR_MISSING_PRIMARY;
        return false; //Must return, otherwise primary is dereferenced
    }

    if (calcPeriapsis(orbit) <= (f64_t)orbit->PrimaryBody->EqRadiusM)
        *err = ORBIT_ERR_INTERSECTING_SURFACE;

    if (calcApoapsis(orbit) > orbit->PrimaryBody->SOI)
        *err = ORBIT_ERR_OUTSIDE_SOI;

    if (calcPeriapsis(orbit) < (f64_t)orbit->PrimaryBody->EqRadiusM + (f64_t)orbit->PrimaryBody->AtmHeightM)
        *err = ORBIT_ERR_BELOW_ATMOSPHERE;

    if (*err != ORBIT_SUCCESS)
        return false;
    return true;
}

/**
 * Validates parameters for resonant orbit calculations.
 * Validates:
 * - RES_ERR_APOAPSIS_OUTSIDE_SOI
 * - RES_ERR_INVALID_SATELLITE_COUNT
 * @param orbit Pointer to the orbit to validate
 * @param satelliteCount Number of satellites in the constellation
 * @param err Pointer to ResonantError enum to store validation result
 * @return true if all parameters are valid, false otherwise
 */
static bool ValidateResOrbParams(const Orbit *orbit, uint32_t satelliteCount, ResonantError *err)
{
    if (orbit->Apoapsis(orbit) > orbit->PrimaryBody->SOI)
        *err = RES_ERR_APOAPSIS_OUTSIDE_SOI;

    if (satelliteCount < 3)
        *err = RES_ERR_INVALID_SATELLITE_COUNT;

    if (*err != RES_SUCCESS)
        return false;
    return true;
}

#define ErrToStr(err) _Generic(err, \
    OrbitError:    OrbitErrorToString(err), \
    ResonantError: ResonantErrorToString(err), \
    LosError:      LosErrorToString(err))