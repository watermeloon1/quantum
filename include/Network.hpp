#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>
#include <string>

class Channel;
struct Device;

class Network
{
public:
    Network(Device &alice, Device &bob, const double heightAboveSeaLevel, const double deviationRangeHeight, const double deviationRangeLateral);
    virtual ~Network();

    double getOpticalDistance() const;
    double getRandom(const double deviation) const;

    void addChannel(Channel &channel);
    void deleteChannel(Channel &channel);

protected:
    static std::string type;

    virtual void simulateSingleSatelliteDefault(double precision) = 0;
    virtual void simulateDoubleSatelliteDefault(double precision) = 0;
    virtual void simulateTripleSatelliteDefault(double precision) = 0;
    virtual double getQBER() = 0;

    bool dataLogger(const std::string &filename, const std::string &directory, const std::vector<double> &data);
    std::string makeFolder(const char *prefix, const double heightAboveSeaLevel, const std::string &technology);

    Device &m_alice;
    Device &m_bob;

    double m_heightAboveSeaLevel;
    double m_deviationRangeHeight;
    double m_deviationRangeLateral;

    std::vector<Channel *> m_channels;
};

#endif
