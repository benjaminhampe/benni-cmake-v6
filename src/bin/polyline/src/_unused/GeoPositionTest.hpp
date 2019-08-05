#ifndef GEOPOSITIONTEST_HPP
#define GEOPOSITIONTEST_HPP

#include <de/hampe/math/GeoPosition.hpp>
#include <de/hampe/logging/Logger.hpp>

class GeoPositionTest
{
    CREATE_STATIC_CLASS_LOGGER("de.hampe.math.GeoPositionTest")
public:
    GeoPositionTest() = default;
    ~GeoPositionTest() = default;

    static void test();
};

#endif // GEOPOSITIONTEST_HPP
