#ifndef ATMOSPHERE_HPP
#define ATMOSPHERE_HPP

#include <string>
#include <vector>

class Atmosphere
{
public:
    Atmosphere(const std::string &climate, const std::string &season, const std::string &weather);
    ~Atmosphere();

    double windSpeed = 21.0;

    std::vector<double> layers;
    std::vector<double> molecularScattering;
    std::vector<double> molecularAbsorption;
    std::vector<double> aerosolScattering;
    std::vector<double> aerosolAbsorption;

private:
    const std::string m_climate;
    const std::string m_season;
    const std::string m_weather;

    void readFile(const std::string &directory);
};

#endif
