#ifndef UNITY_BUILD
#include "global_typedefs.c"
#include "orbit.c"
#endif

#include "error.h"

const char* ResonantErrorToString(ResonantError err)
{
    static const char* errorStrings[] = {
        [RES_SUCCESS]                          = "Success",
        [RES_ERR_PERIAPSIS_INTERSECTS_SURFACE] = "Periapsis intersects surface, consider a prograde burn",
        [RES_ERR_INVALID_SATELLITE_COUNT]      = "Invalid satellite count, must be at least 3",
        [RES_ERR_APOAPSIS_OUTSIDE_SOI]         = "Apoapsis outside sphere of influence, consider a retrograde burn",
        [RES_ERR_MISSING_PRIMARY]              = "Missing primary body",
        [RES_ERR_NOT_ELLIPTICAL]               = "Target orbit is not elliptical",
        [RES_ERR_NOT_ELLIPTICAL_OR_CIRCULAR]   = "Base orbit is not elliptical or circular",
        [RES_ERR_NOT_INTERSECTING]             = "Orbits do not intersect at periapsis or apoapsis",
        [RES_ERR_PERIAPSIS_BELOW_ATMOSPHERE]   = "Orbit is below atmosphere, use a higher periapsis",
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
        [LOS_ERR_INVALID_SATELLITE_COUNT]  = "Invalid satellite count, must be at least 3",
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
        [ORBIT_ERR_BELOW_ATMOSPHERE]     = "Orbit is below atmosphere, use a higher periapsis",
        [ORBIT_ERR_OUTSIDE_SOI]          = "Orbit is outside sphere of influence, use a lower apoapsis",
        [ORBIT_ERR_INTERSECTING_SURFACE] = "Orbit intersects surface, use a higher periapsis",
    };

    if (err >= (sizeof(errorStrings) / sizeof(errorStrings[0])))
        return "Unknown error";

    return errorStrings[err];
}

static bool ValidateLos(const Orbit *orbit, uint32_t satelliteCount, LosError *err)
{
    if (orbit->PrimaryBody == nullptr)
    {
        *err = LOS_ERR_MISSING_PRIMARY;
        return false;
    }

    if (orbit->Eccentricity != 0.0)
    {
        *err = LOS_ERR_ORBIT_NOT_CIRCULAR;
        return false;
    }

    if (satelliteCount < 3)
    {
        *err = LOS_ERR_INVALID_SATELLITE_COUNT;
        return false;
    }

    return true;
}

#define ErrToStr(err) _Generic(err, \
    OrbitError:    OrbitErrorToString(err), \
    ResonantError: ResonantErrorToString(err), \
    LosError:      LosErrorToString(err))