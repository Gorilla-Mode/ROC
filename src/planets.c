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
        .SOI = 9646663.0,
        .Name = "Moho"
    },
    {
        //Eve
        .GravSurf = 16.70,
        .GravParam = 8.1717302e12,
        .EqRadiusM = 700000,
        .AtmHeightM = 90000,
        .SOI = 85109365,
        .Name = "Eve"
    },
    {
        //Gilly
        .GravSurf = 0.049,
        .GravParam = 8289449.80,
        .EqRadiusM = 13000,
        .AtmHeightM = -1,
        .SOI = 126123.27,
        .Name = "Gilly"
    },
    {
        //Kerbin
        .GravSurf = 9.81,
        .GravParam = 3.5316000e12,
        .EqRadiusM = 600000,
        .AtmHeightM = 70000,
        .SOI = 84159286.0,
        .Name = "Kerbin"
    },
    {
        //Mun
        .GravSurf = 1.63,
        .GravParam = 6.5138398e10,
        .EqRadiusM = 200000,
        .AtmHeightM = -1,
        .SOI = 2429559.1,
        .Name = "Mun"
    },
    {
        //Minmus
        .GravSurf = 0.491,
        .GravParam = 1.7658000e9,
        .EqRadiusM = 60000,
        .AtmHeightM = -1,
        .SOI = 2247428.4,
        .Name = "Minmus"
    },
    {
        //Duna
        .GravSurf = 2.94,
        .GravParam = 3.0136321e11,
        .EqRadiusM = 320000,
        .AtmHeightM = 50000,
        .SOI = 47821949.0,
        .Name = "Duna"
    },
    {
        //Ike
        .GravSurf = 1.00,
        .GravParam = 1.8568369e10,
        .EqRadiusM = 130000,
        .AtmHeightM = -1,
        .SOI = 1049598.9,
        .Name = "Ike"
    },
    {
        //Dres
        .GravSurf = 1.13,
        .GravParam = 2.1484489e10,
        .EqRadiusM = 138000,
        .AtmHeightM = -1,
        .SOI = 32832840.0,
        .Name = "Dres"
    },
    {
        //Jool
        .GravSurf = 7.85,
        .GravParam = 2.8252800e14,
        .EqRadiusM = 6000000,
        .AtmHeightM = 200000,
        .SOI = 2.4559852e9,
        .Name = "Jool"
    },
    {
        //Laythe
        .GravSurf = 7.85,
        .GravParam = 1.9620000e12,
        .EqRadiusM = 500000,
        .AtmHeightM = 50000,
        .SOI = 3723645.8,
        .Name = "Laythe"
    },
    {
        //Vall
        .GravSurf = 2.31,
        .GravParam = 2.0748150e11,
        .EqRadiusM = 300000,
        .AtmHeightM = -1,
        .SOI = 2406401.4,
        .Name = "Vall"
    },
    {
        //Tylo
        .GravSurf = 7.85,
        .GravParam = 2.8252800e12,
        .EqRadiusM = 6000000,
        .AtmHeightM = -1,
        .SOI = 10856518,
        .Name = "Tylo"
    },
    {
        //Bop
        .GravSurf = 0.589,
        .GravParam = 2.4868349e9,
        .EqRadiusM = 65000,
        .AtmHeightM = -1,
        .SOI = 1221060.9,
        .Name = "Bop"
    },
    {
        //Pol
        .GravSurf = 0.373,
        .GravParam = 7.2170208e8,
        .EqRadiusM = 44000,
        .AtmHeightM = -1,
        .SOI = 1042138.9,
        .Name = "Pol"
    },
    {
        //Eeloo
        .GravSurf = 1.69,
        .GravParam = 5.5417694e11,
        .EqRadiusM = 2100000,
        .AtmHeightM = -1,
        .SOI = 1.1908294e8,
        .Name = "Eeloo"
    }
};