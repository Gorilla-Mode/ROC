typedef enum {
    ORBIT_SUCCESS = 0,
    ORBIT_ERR_PERIAPSIS_INTERSECTS_SURFACE,
    ORBIT_ERR_INVALID_SATELLITE_COUNT,
    ORBIT_ERR_APOAPSIS_OUTSIDE_SOI,
    ORBIT_ERR_MISSING_PRIMARY,
    ORBIT_ERR_NOT_ELLIPTICAL,
    ORBIT_ERR_NOT_ELLIPTICAL_OR_CIRCULAR,
    ORBIT_ERR_NOT_INTERSECTING,
} OrbitError;

typedef enum
{
    LOS_SUCCESS = 0,
    LOS_ERR_INVALID_SATELLITE_COUNT,
    LOS_ERR_MISSING_PRIMARY,
    LOS_ERR_ORBIT_NOT_CIRCULAR
}LosError;

const char* OrbitErrorToString(OrbitError err)
{
    static const char* errorStrings[] = {
        [ORBIT_SUCCESS]                          = "Success",
        [ORBIT_ERR_PERIAPSIS_INTERSECTS_SURFACE] = "Periapsis intersects surface, consider a prograde burn",
        [ORBIT_ERR_INVALID_SATELLITE_COUNT]      = "Invalid satellite count, must be at least 3",
        [ORBIT_ERR_APOAPSIS_OUTSIDE_SOI]         = "Apoapsis outside sphere of influence, consider a retrograde burn",
        [ORBIT_ERR_MISSING_PRIMARY]              = "Missing primary body",
        [ORBIT_ERR_NOT_ELLIPTICAL]               = "Target orbit is not elliptical",
        [ORBIT_ERR_NOT_ELLIPTICAL_OR_CIRCULAR]   = "Base orbit is not elliptical or circular",
        [ORBIT_ERR_NOT_INTERSECTING]             = "Orbits do not intersect at periapsis or apoapsis",
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