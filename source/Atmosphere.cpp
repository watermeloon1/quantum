#include "../include/Atmosphere.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Atmosphere::Atmosphere(const std::string &climate, const std::string &season, const std::string &weather):
    m_climate(climate), m_season(season), m_weather(weather){
    
    readFile("resource/layer_data.csv");
}

Atmosphere::Atmosphere(){}
Atmosphere::~Atmosphere(){}

void Atmosphere::readFile(const std::string &directory) {
    // Open the file
    std::ifstream file(directory);

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "ERROR: unable to open file: \"" << directory << "\""<< std::endl;
        exit(1);
    }

    // Declare variables
    std::string line;
    uint8_t lineCount = 0;

    while (std::getline(file, line)) {
        if (6 < lineCount){
            std::stringstream lineStream(line);
            std::vector<double> row;
            std::string cell;

            // Split the line into cells separated by commas
            while (std::getline(lineStream, cell, ';')) {
                row.emplace_back(strtod(cell.c_str(), nullptr));
            }

            layers.emplace_back(row[0]);

            if (m_climate == "Tropical"){
				molecularAbsorption.emplace_back(row[2]);
				molecularScattering.emplace_back(row[3]);
			}

			else if (m_climate == "Midlatitude"){
				if (m_season == "Summer" ){
					molecularAbsorption.emplace_back(row[5]);
					molecularScattering.emplace_back(row[6]);
				}
				else if (m_season == "Winter"){
					molecularAbsorption.emplace_back(row[8]);
					molecularScattering.emplace_back(row[9]);
				} else {
                    std::cerr << "ERROR: can not recognise provided season\n" << std::endl;
                    exit(1);
                }
			} else {
                std::cerr << "ERROR: can not recognise provided climate\n" << std::endl;
                exit(1);
            }

			if (m_weather == "Clear"){
				aerosolAbsorption.emplace_back(row[11]);
				aerosolScattering.emplace_back(row[12]);
			}
			else if (m_weather == "Hazy"){
				aerosolAbsorption.emplace_back(row[14]);
				aerosolScattering.emplace_back(row[15]);
			} else {
                std::cerr << "ERROR: can not recognise provided weather\n" << std::endl;
                exit(1);
            }
        }
        ++lineCount;
    }

    // Close the file
    file.close();
    std::cout << "LOG: read data from \"" << directory << "\" " << "successfully" << std::endl;
}
