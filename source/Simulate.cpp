#include "../include/tools.hpp"
#include "../include/global.hpp"
#include "../include/Device.hpp"
#include "../include/Atmosphere.hpp"
#include "../include/Channel.hpp"
#include "../include/Entanglement.hpp"

#include <iostream>

int main(void){
    const Atmosphere alice_atm = Atmosphere("Midlatitude", "Summer", "Clear");
    const Atmosphere bob_atm = Atmosphere("Tropical", "Summer", "Hazy");
    const Device alice = Device("alice", 0.0, 0.0);
    const Device bob = Device("bob", 3.0, 0.0);

    Entanglement ent = Entanglement(alice, &alice_atm, bob, &alice_atm, 500.0, 0.0, 0.0);
    ent.simulateDoubleSatelliteDefault(1.0);

    return 0;
}
