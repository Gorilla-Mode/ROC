#ifndef ERROR_H
#define ERROR_H

typedef enum {
    RES_SUCCESS = 0,
    RES_ERR_PERIAPSIS_INTERSECTS_SURFACE,
    RES_ERR_INVALID_SATELLITE_COUNT,
    RES_ERR_APOAPSIS_OUTSIDE_SOI,
    RES_ERR_MISSING_PRIMARY,
    RES_ERR_NOT_ELLIPTICAL,
    RES_ERR_NOT_ELLIPTICAL_OR_CIRCULAR,
    RES_ERR_NOT_INTERSECTING,
    RES_ERR_PERIAPSIS_BELOW_ATMOSPHERE
} ResonantError;

typedef enum
{
    LOS_SUCCESS = 0,
    LOS_ERR_INVALID_SATELLITE_COUNT,
    LOS_ERR_MISSING_PRIMARY,
    LOS_ERR_ORBIT_NOT_CIRCULAR
}LosError;

typedef enum
{
    ORBIT_SUCCESS = 0,
    ORBIT_ERR_MISSING_PRIMARY,
    ORBIT_ERR_BELOW_ATMOSPHERE,
    ORBIT_ERR_OUTSIDE_SOI,
    ORBIT_ERR_INTERSECTING_SURFACE
}OrbitError;

typedef struct Orbit Orbit;

static bool ValidateLos(const Orbit *orbit, uint32_t satelliteCount, LosError *err);
static bool ValidateOrbit(const Orbit *orbit, OrbitError *err);

#endif