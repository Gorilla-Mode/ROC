#ifndef UNITY_BUILD
#include "celestial_body.c"
#endif

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