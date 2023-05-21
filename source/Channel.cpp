#include "../include/Channel.hpp"
#include "../include/Device.hpp"
#include "../include/Atmosphere.hpp"
#include "../include/global.hpp"
#include "../include/tools.hpp"

// Throws a std::runtime_error if an error occurs during initialization.
Channel::Channel(const Device &initiator, const Device &receiver, const Atmosphere *atmosphere):
    m_initiator(initiator), m_receiver(receiver), m_atmosphere(atmosphere){
    try {
        initDirection();
    } catch (const std::runtime_error &e){
        global::LOG(e.what());
        exit(1);
    }
}

Channel::Channel(const Device &initiator, const Device &receiver):
    m_initiator(initiator), m_receiver(receiver), m_atmosphere(NULL) {

    try {
        initDirection();
    } catch (const std::runtime_error &e){
        global::LOG(e.what());
        exit(1);
    }
}

Channel::~Channel() {
}

void Channel::update() {

    initTheta();
	initDeltaHeight();

    try {
        initOpticalDistance();
    } catch (const std::runtime_error &e){
        throw std::runtime_error("ERROR: unable to initialize channel ( devices are out of sight )");
    }


	initSurfaceDistance();

	if (direction != global::space) {
		try {
		   initZenith();
		} catch (const std::range_error &e){
			throw std::runtime_error("ERROR: communication is insecure ( zenith is too large )");
		}
	}

	initOpticalSectors();

	try {
        initTransmittance();
    } catch (const std::runtime_error &e){
        global::LOG(e.what());
        exit(1);
    }

	initQuantumBitErrorRate();	
}

void Channel::initDirection() {
	if (m_initiator.getType() == "Base") {
		if (m_receiver.getType() == "Base") {
			throw std::runtime_error("ERROR: base to base communication is not valid!");
		} else {
            direction = global::uplink;
        }
	} else {
		if (m_receiver.getType() == "Base") {
			direction = global::downlink;
		} else {
            direction = global::space;
        }
	}
}


void Channel::initTheta() {
	// Converting the latitude and longitude value
	// from degrees to radian
	double latitude1 = global::toRadian(m_initiator.getLatitude());
	double longitude1 = global::toRadian(m_initiator.getLongitude());

	double latitude2 = global::toRadian(m_receiver.getLatitude());
	double longitude2 = global::toRadian(m_receiver.getLongitude());

	double &phi1 = latitude1;
	double &phi2 = latitude2;

	// flattening of the ellipsoid
	double f = 1.0 / 298.257223563;

	double u1 = atan((1.0 - f) * tan(phi1));
	double u2 = atan((1.0 - f) * tan(phi2));

	// The absolute distance between the devices
	double lambda = abs(longitude1 - longitude2);

	// Vincenty's formulae
	double a = pow(cos(u2) * sin(lambda), 2);
	double b = pow(cos(u1) * sin(u2) - (sin(u1) * cos(u2) * cos(lambda)), 2);

	double sinus = sqrt(a + b);
	double cosinus = (sin(u1) * sin(u2)) + (cos(u1) * cos(u2) * cos(lambda));

	// The angle that the origo and the
	// two device coordinates close
	theta = atan2(sinus, cosinus);
}

void Channel::initDeltaHeight() {
	deltaHeight = abs(m_initiator.getHeightAboveSeaLevel() -
                      m_receiver.getHeightAboveSeaLevel());
}

void Channel::initOpticalDistance(){
	double s1 = global::EARTH_RADIUS + m_initiator.getHeightAboveSeaLevel();
	double s2 = global::EARTH_RADIUS + m_receiver.getHeightAboveSeaLevel();

	// Link between satellites
	if(direction == global::space){

		// radius that is used for the calculation
		double atmosphere = global::EARTH_RADIUS + 199.0;

		double enumerator = sin(theta) * s1 * s2;
		double denom1 = pow(sin(theta) * s2, 2);
		double denom2 = pow(cos(theta) * s2 - s1, 2);

		// the distance between the quantum link and the origo
		double perpendicular = enumerator / sqrt(denom1 + denom2);
		//std::cout << perpendicular << std::endl;

		// the x coordinates of the two satellites
		double x1 = s1;
		double x2 = cos(theta) * s2;

		// the line of the link is intercepted
		if (perpendicular < atmosphere){

			double z = sin(theta) * s2 / (cos(theta) * s2 - s1);

			double a = 1.0 + pow(z, 2);
			double b = (- 2.0) * s1 * pow(z, 2);
			double c = pow(s1, 2) * pow(z, 2) - pow(atmosphere, 2);

			double positive = (- b + sqrt(pow(b, 2) - 4.0 * a * c)) / (2.0 * a);
			double negative = (- b - sqrt(pow(b, 2) - 4.0 * a * c)) / (2.0 * a);

			// the interception is between the satellites
			if (( x1 < negative && x1 < positive && negative < x2 && positive < x2) ||
				( x2 < negative && x2 < positive && negative < x1 && positive < x1) ) {
					throw std::runtime_error("ERROR: devices are out of sight");
				}

		// the line of the link touches the arc
		} else if (perpendicular == atmosphere){
			double z = sin(theta) * s2 / (cos(theta) * s2 - s1);

			double a = 1.0 + pow(z, 2);
			double b = (- 2.0) * s1 * pow(z, 2);

			double value = - b / (2.0 * a);

			// the interception is between the satellites
			if ((value < x1 && x2 < value) || (value < x2 && x1 < value)) {
					throw std::runtime_error("ERROR: devices are out of sight");
			}
		}


	// Link between a base and a satellite
	} else {
		// In case the theta is greater than half-pi
		// the base satellite will never be in the
		// sight of the base
    	if (theta > global::PI / 2.0){
        	throw std::runtime_error("ERROR: devices are out of sight");
		}
	}
    opticalDistance = sqrt(pow(s1, 2) + pow(s2, 2) - (2.0 * s1 * s2 * cos(theta)));
}

void Channel::initSurfaceDistance() {
	double circumference = 2.0 * global::PI * global::EARTH_RADIUS;
	double proportion = theta / (2 * global::PI);

	surfaceDistance = circumference * proportion;
}

void Channel::initZenith() {
	// This data only needs to be inited before communicating with base
    const double MAX_ZENITH = 45.0;

	double zenithTemp = tools::zenith(deltaHeight, opticalDistance);
	if (zenithTemp <= MAX_ZENITH){
		zenith = zenithTemp;
	} else {
        throw std::range_error("ERROR: zenith angle is too large for reliable communication");
    }

}

void Channel::initOpticalSectors(){
    double dist = deltaHeight;
    opticalSectors.push_back(0);

    for (size_t i = 0; 0 < dist; ++i) {
        (void)i;
        if (opticalSectors.back() < 1000) {
            opticalSectors.push_back(opticalSectors.back() + 100);
            dist -= 100;
        } else if (opticalSectors.back() < 30000) {
            opticalSectors.push_back(opticalSectors.back() + 500);
            dist -= 500;
        } else if (opticalSectors.back() < 60000) {
            opticalSectors.push_back(opticalSectors.back() + 1000);
            dist -= 1000;
        } else {
            opticalSectors.push_back(opticalSectors.back() + 10000);
            dist -= 5000;
        }

        if (dist <= 0) {
            break;
        }
    }
    opticalSectors.back() += dist;
}


void Channel::initTransmittance(){
    switch (direction){
        case global::uplink:
            staticTransmittance = tools::staticTransmittance(m_atmosphere -> molecularScattering, m_atmosphere -> molecularAbsorption, m_atmosphere -> aerosolScattering, m_atmosphere -> aerosolAbsorption, m_atmosphere -> layers, zenith);
    		coherenceLength = tools::beamWideningEarthSpace(m_initiator.getWaveLength(), opticalSectors, opticalDistance, m_atmosphere -> windSpeed, zenith);
    		beamWidening = tools::beamWideningAtmosphere(opticalDistance, m_receiver.getApertureDiameter(), m_initiator.getWaveLength(), coherenceLength, m_initiator.getFocus());
    		targetingError = tools::targetingError(opticalDistance, m_initiator.getTargetingAngularError());
    		totalScattering = tools::totalScattering(beamWidening, targetingError);
    		dynamicTransmittance = tools::dynamicTransmittance(totalScattering, m_receiver.getMirrorDiameter() / 2.0);
            break;

        case global::downlink:
            staticTransmittance = tools::staticTransmittance(m_atmosphere -> molecularScattering, m_atmosphere -> molecularAbsorption, m_atmosphere -> aerosolScattering, m_atmosphere -> aerosolAbsorption, m_atmosphere -> layers, zenith);
    		coherenceLength = tools::beamWideningSpaceEarth(m_initiator.getWaveLength(), opticalSectors, opticalDistance, m_atmosphere -> windSpeed, zenith);
    		beamWidening = tools::beamWideningAtmosphere(opticalDistance, m_receiver.getApertureDiameter(), m_initiator.getWaveLength(), coherenceLength, m_initiator.getFocus());
    		targetingError = tools::targetingError(opticalDistance, m_initiator.getTargetingAngularError());
    		totalScattering = tools::totalScattering(beamWidening, targetingError);
    		dynamicTransmittance = tools::dynamicTransmittance(totalScattering, m_receiver.getMirrorDiameter() / 2.0);
            break;

        case global::space:
    		staticTransmittance = 1;
    		beamWidening = tools::beamWideningVacuum(opticalDistance, m_receiver.getApertureDiameter(), m_initiator.getWaveLength(), m_initiator.getFocus());
    		targetingError = tools::targetingError(opticalDistance, m_initiator.getTargetingAngularError());
    		totalScattering = tools::totalScattering(beamWidening, targetingError);
    		dynamicTransmittance = tools::dynamicTransmittance(totalScattering, m_receiver.getMirrorDiameter() / 2.0);
    		break;

        default:
            throw std::runtime_error("ERROR: invalid direction of communication");
            break;
    }

    transmittance = staticTransmittance * dynamicTransmittance;
}

void Channel::initQuantumBitErrorRate(){
    const double numerator = m_receiver.getNoise() * m_receiver.getDetectorCount();
    const double denumerator = transmittance * m_initiator.getMeanPhotonNumberOfSignal() * m_receiver.getQuantumEfficiencyOfDetector() * 2.0;

	quantumBitErrorRate = m_receiver.getProbabilityOfPolarizationMeasurementError() + (numerator / denumerator);
}

global::Direction Channel::getDirection() const {
    return direction;
}

double Channel::getQuantumBitErrorRate() const {
    return quantumBitErrorRate;
}

double Channel::getTheta() const {
    return theta;
}

double Channel::getZenith() const {
    return zenith;
}

double Channel::getOpticalDistance() const {
    return opticalDistance;
}

double Channel::getTransmittance() const {
    return transmittance;
}
