typedef enum {
    RES_SUCCESS = 0,
    RES_ERR_PERIAPSIS_INTERSECTS_SURFACE,
    RES_ERR_INVALID_SATELLITE_COUNT,
    RES_ERR_APOAPSIS_OUTSIDE_SOI,
    RES_ERR_MISSING_PRIMARY,
    RES_ERR_NOT_ELLIPTICAL,
    RES_ERR_NOT_ELLIPTICAL_OR_CIRCULAR,
    RES_ERR_NOT_INTERSECTING,
} ResonantError;

typedef enum
{
    LOS_SUCCESS = 0,
    LOS_ERR_INVALID_SATELLITE_COUNT,
    LOS_ERR_MISSING_PRIMARY,
    LOS_ERR_ORBIT_NOT_CIRCULAR
}LosError;

const char* OrbitErrorToString(ResonantError err)
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

    if (err >= (sizeof(errorStrings) / sizeof(errorStrings[0]))) {
        return "Unknown error";
    }
    return errorStrings[err];
}