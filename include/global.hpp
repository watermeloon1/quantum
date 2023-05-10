#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <math.h>
#include <iostream>

namespace global
{
    inline const double PI = 3.14159265;
    inline const double EARTH_RADIUS = 6378.0;
    inline const double ATMOSPHERE_HEIGHT = (EARTH_RADIUS + 200.0);
    inline const double A = (1.7 * pow(10.0, -14));

    template<typename T>
    inline void LOG(T log) {
        std::cout << log << std::endl;
    }

    enum Focus
    {
        parallel,
        optimal
    };

    enum Direction
    {
        uplink,
        downlink,
        space
    };

    inline double toRadian(double number)
    {
        return number * PI / 180.0;
    }

    inline double toDegree(double number)
    {
        return number * 180.0 / PI;
    }

    inline double toMeter(double kilometer)
    {
        return kilometer * 1000.0;
    }

    inline double toWaveNumber(double waveLength)
    {
        return (2.0 * PI) / (waveLength * pow(10.0, -9));
    }
}
#endif
