#include "../include/tools.hpp"
#include "../include/global.hpp"
#include "../include/Device.hpp"
#include "../include/Atmosphere.hpp"
#include "../include/Channel.hpp"

#include <iostream>

int main(void){
    Atmosphere atm = Atmosphere("Tropical", "Summer", "Clear");
    Device dev1 = Device("alice", 0.0, 0.0);
    Device dev2 = Device("bob", 0.0, 0.0, 500);
    Channel *chann;

    try {
        chann = new Channel(dev1, dev2, atm);
    } catch (const std::runtime_error &e){
        global::LOG(e.what());
        return 0;
    }

    global::LOG(chann -> getQuantumBitErrorRate());

    delete chann;
    return 0;
}
