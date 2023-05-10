#include "../include/Tools.hpp"
#include <vector>

static double zenith(const double heightAboveSeaLevel, const double distance) {

	if (heightAboveSeaLevel <= distance){

		double numerator, denominator, alfaRadian, alfaDegree, zenithDegree;

		numerator = pow(global::EARTH_RADIUS, 2) + pow(distance, 2) - pow((global::EARTH_RADIUS + heightAboveSeaLevel), 2);
		denominator = 2 * global::EARTH_RADIUS * distance;
		alfaRadian = acos(numerator / denominator);
		alfaDegree = global::toDegree(alfaRadian);
		zenithDegree = 180 - alfaDegree;

		return zenithDegree;
	} else { return -1; }
}

static double beamWideningVacuum(const double distance, const double apertureDiameter,
                                 const double waveLength, const global::Focus &focus) {

	double a, b, c;

	double waveNumber = global::toWaveNumber(waveLength);
	double dist = global::toMeter(distance);

	a = 4.0 * pow(dist, 2);
	b = pow(waveNumber, 2) * pow(apertureDiameter, 2);
    c = pow(apertureDiameter, 2) / 4.0;

	if (focus == global::optimal){

		return sqrt(a / b);

	} else { return sqrt((a / b) + c); }
}

static double beamWideningAtmosphere(const double distance, const double apertureDiameter,
                                     const double waveLength, const double coherenceLength,
                                     const global::Focus &focus) {

	double a, b, c, d, e, f;

	double waveNumber = global::toWaveNumber(waveLength);
	double dist = global::toMeter(distance);

	a = 4.0 * pow(dist, 2);
	b = pow(waveNumber, 2) * pow(apertureDiameter, 2);

	c = pow(waveNumber, 2) * pow(coherenceLength, 2);

	d = pow(coherenceLength / apertureDiameter, 1 / 3);
	e = pow(1.0 - 0.62 * d, 6 / 5);

	f = pow(apertureDiameter, 2) / 4.0;

	if (focus == global::optimal){

		return sqrt((a / b) + (a / c) * e);

	} else { return sqrt((a / b) + f + (a / c) * e); }

}

static double turbulenceStrength(const double heightAboveSeaLevel, const double windSpeed) {

	double a, b, c, d, e;

	a = 0.00594 * pow((windSpeed / 27.0), 2);
	b = pow(heightAboveSeaLevel * pow(10.0, -5), 10);
	c = exp((heightAboveSeaLevel / 1000) * (-1.0));
	d = 2.7 * pow(10.0, -16) * exp((heightAboveSeaLevel / 1500.0) * (-1.0));
	e = global::A * exp((heightAboveSeaLevel / 100.0) * (-1.0));

	return a * b * c + d + e;
}

static double targetingError(const double distance, const double targetingAngularError) {

	double a, b;

	double dist = global::toMeter(distance);

	a = dist * targetingAngularError;
	b = pow(10.0, -6);
	return a * b;
}

static double totalScattering(const double beamRadius, const double targetingError) {

	double a, b;

	a = pow(beamRadius, 2);
	b = pow(targetingError, 2);

	return sqrt(a + b);
}

static double dynamicTransmittance(const double totalScattering, const double mirrorRadius) {

	double a, b;

	a = (-1.0) * pow(mirrorRadius, 2);
	b = 2.0 * pow(totalScattering, 2);

	return 1.0 - exp(a / b);
}

static double staticTransmittance(const std::vector<double> &molecularScattering,
                                  const std::vector<double> &molecularAbsorption,
                                  const std::vector<double> &aerosolScattering,
                                  const std::vector<double> &aerosolAbsorption,
                                  const std::vector<double> &layers, const double zenith) {

	std::vector<double> scattering(molecularScattering.size());


	for (unsigned long i = 0; i < molecularScattering.size(); ++i) {
		scattering[i] = molecularScattering[i] + aerosolScattering[i];
	}

	std::vector<double> absorption(molecularAbsorption.size());

	for (unsigned long i = 0; i < molecularAbsorption.size(); ++i) {
		absorption[i] = molecularAbsorption[i] + aerosolAbsorption[i];
	}

	double layersSum = 0.0;

	for (unsigned long i = 0; i < layers.size() - 1; ++i) {
		layersSum += (((scattering[i] + scattering[i + 1]) / 2.0) + ((absorption[i] + absorption[i + 1]) / 2.0)) *
											((layers[i + 1] - layers[i]) / cos(zenith * (global::PI / 180.0)));
	}

	return exp((-1) * layersSum);
}

static double beamWideningEarthSpace(const double waveLength, const std::vector<double> &sectors,
                                     const double distance, const double windSpeed, const double zenith){

	double a, b, c, d, e;
	double t0, t1;

	double dist = global::toMeter(distance);
	double waveNumber = global::toWaveNumber(waveLength);

	double sum = 0;

	for (unsigned long i = 0; i < sectors.size() - 1; ++i){

		t0 = turbulenceStrength(sectors[i], windSpeed);
		t1 = turbulenceStrength(sectors[i + 1], windSpeed);

		a = pow(1.0 - sectors[i] / (dist * cos(zenith * (global::PI / 180.0))), 5 / 3);
		b = pow(1.0 - sectors[i + 1] / (dist * cos(zenith * (global::PI / 180.0))), 5 / 3);
		c = (t0 * a + t1 * b) / 2.0;
		d = abs((sectors[i + 1] - sectors[i]) / cos(global::toRadian(zenith)));
		e = c * d;

		sum += e;
	}

	sum = 1.46 * pow(waveNumber, 2) * sum;

	if (sum < 0){
		sum *= (-1);
		sum = pow(sum, -0.6);
		sum *= (-1);
		return sum;
	} else { return pow(sum, -0.6); }
}

 static double beamWideningSpaceEarth(const double waveLength, const std::vector<double> &sectors,
                                      const double distance, const double windSpeed, const double zenith){

	double a, b, c, d, e;
	double t0, t1;

	double dist= global::toMeter(distance);
	double waveNumber = global::toWaveNumber(waveLength);

	double sum = 0;

	for (unsigned long i = 0; i < sectors.size() - 1; ++i){

		t0 = turbulenceStrength(sectors[i], windSpeed);
		t1 = turbulenceStrength(sectors[i + 1], windSpeed);

        a = pow(1.0 - ((dist - (sectors[i] / cos(zenith * (global::PI / 180.0))))) / dist, 5 / 3);
		b = pow(1.0 - ((dist - (sectors[i + 1] / cos(zenith * (global::PI / 180.0))))) / dist, 5 / 3);
		c = (t0 * a + t1 * b) / 2.0;
		d = abs(sectors[i + 1] - sectors[i] / cos(global::toRadian(zenith)));
		e = c * d;

		sum += e;
	}

	return pow(1.46 * pow(waveNumber, 2) * sum, -(0.6));
}
