#ifndef UNITY_BUILD
#include "celestial_body.c"
#endif

CelestBody Kerbol[] = {
    {
        //Moho
        .GravSurf = 2.70,
        .GravParam = 1.6860938e11,
        .EqRadiusM = 250000,
        .AtmHeightM = -1,
        .SOI = 9646663.0
    },
    {
        //Eve
        .GravSurf = 16.70,
        .GravParam = 8.1717302e12,
        .EqRadiusM = 700000,
        .AtmHeightM = 90000,
        .SOI = 85109365
    },
    {
        //Gilly
        .GravSurf = 0.049,
        .GravParam = 8289449.80,
        .EqRadiusM = 13000,
        .AtmHeightM = -1,
        .SOI = 126123.27
    },
    {
        //Kerbin
        .GravSurf = 9.81,
        .GravParam = 3.5316000e12,
        .EqRadiusM = 600000,
        .AtmHeightM = 70000,
        .SOI = 84159286.0
    },
    {
        //Mun
        .GravSurf = 1.63,
        .GravParam = 6.5138398e10,
        .EqRadiusM = 200000,
        .AtmHeightM = -1,
        .SOI = 2429559.1
    },
    {
        //Minmus
        .GravSurf = 0.491,
        .GravParam = 1.7658000e9,
        .EqRadiusM = 60000,
        .AtmHeightM = -1,
        .SOI = 2247428.4
    },
    {
        //Duna
        .GravSurf = 2.94,
        .GravParam = 3.0136321e11,
        .EqRadiusM = 320000,
        .AtmHeightM = 50000,
        .SOI = 47821949.0
    },
    {
        //Ike
        .GravSurf = 1.00,
        .GravParam = 1.8568369e10,
        .EqRadiusM = 130000,
        .AtmHeightM = -1,
        .SOI = 1049598.9
    },
    {
        //Dres
        .GravSurf = 1.13,
        .GravParam = 2.1484489e10,
        .EqRadiusM = 138000,
        .AtmHeightM = -1,
        .SOI = 32832840.0
    },
    {
        //Jool
        .GravSurf = 7.85,
        .GravParam = 2.8252800e14,
        .EqRadiusM = 6000000,
        .AtmHeightM = 200000,
        .SOI = 2.4559852e9
    }
};