#ifndef NETWORK_HPP
#define NETWORK_HPP

#define MAX_LONGITUDE 180.0

#include <vector>
#include <string>

class Channel;
class Atmosphere;
struct Device;

class Network
{
public:
    Network(const Device &alice, const Atmosphere *alice_atmosphere, const Device &bob, const Atmosphere *bob_atmosphere, const double heightAboveSeaLevel, const double deviationRangeHeight, const double deviationRangeLateral);
    virtual ~Network();

    double getOpticalDistance() const;
    double getRandom(const double deviation) const;

    void addChannel(Channel *channel);
    void deleteChannel(Channel *channel);

protected:
    static std::string type;

    virtual void simulateSingleSatelliteDefault(double precision) = 0;
    virtual void simulateDoubleSatelliteDefault(double precision) = 0;
    virtual void simulateTripleSatelliteDefault(double precision) = 0;
    virtual double getQBER() = 0;

    bool dataLogger(const std::string &filename, const std::string &directory, const std::vector<double> &data);
    std::string makeFolder(const char *prefix, const double heightAboveSeaLevel, const std::string &technology);

    const Device &m_alice;
    const Atmosphere *m_alice_atmosphere;

    const Device &m_bob;
    const Atmosphere *m_bob_atmosphere;

    double m_heightAboveSeaLevel;
    const double m_deviationRangeHeight;
    const double m_deviationRangeLateral;

    std::vector<Channel *> m_channels;
};

#endif
