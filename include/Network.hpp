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

    void deleteChannels();
    void updateChannels();

    std::string getType() const;
    void simulateSingleSatellite(double precision);
    void simulateDoubleSatellite(double precision);
    void simulateTripleSatellite(double precision);

protected:
    std::string m_type = "network";
    virtual double getQBER() const = 0;
    
    virtual void initChannels(Device &satellite) = 0;
    virtual void initChannels(Device &satellite1, Device &satellite2) = 0;
    virtual void initChannels(Device &satellite1, Device &satellite2, Device &satellite3) = 0;
    
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

// TODO:: rethink private and public members visibility
