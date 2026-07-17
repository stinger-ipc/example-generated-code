

#include <iostream>
#include <sstream>
#include <syslog.h>
#include <chrono>
#include <thread>
#include "client.hpp"
#include "structs.hpp"
#include "discovery.hpp"
#include "enums.hpp"
#include <stinger/error/return_codes.hpp>
#include <stinger/mqtt/brokerconnection.hpp>

using namespace stinger::gen::prop_only;

int main(int argc, char** argv)
{
    // Create a connection to the broker
    auto conn = std::make_shared<stinger::mqtt::BrokerConnection>("localhost", 1883, "prop-only-client-demo");
    conn->SetLogLevel(LOG_DEBUG);
    conn->SetLogFunction([](int level, const char* msg)
                         {
                             std::cout << "[" << level << "] " << msg << std::endl;
                         });

    // Discover a service ID for a prop-only service.
    InstanceInfo serviceInfo;
    { // restrict scope
        PropOnlyDiscovery discovery(conn);
        auto serviceInfoFut = discovery.GetSingleton();
        auto serviceInfoFutStatus = serviceInfoFut.wait_for(std::chrono::seconds(15));
        if (serviceInfoFutStatus == std::future_status::timeout) {
            std::cerr << "Failed to discover service instance within timeout." << std::endl;
            return 1;
        }
        serviceInfo = serviceInfoFut.get();
    }

    // Create the client object.
    PropOnlyClient client(conn, serviceInfo);

    // Register callbacks for signals.

    // Register callbacks for property updates.
    client.registerHomeAddressPropertyCallback([](Address address)
                                               {
                                                   std::cout << "Received update for home_address property: " << "address=" << "[Address object]"
                                                             << std::endl;
                                               });

    client.registerFavoriteCountryPropertyCallback([](Country country)
                                                   {
                                                       std::cout << "Received update for favorite_country property: " << "country=" << countryStrings.at(static_cast<int>(country))
                                                                 << std::endl;
                                                   });

    // Call each method with example values.

    std::cout << "Connected and waiting.  Use Ctrl-C to exit." << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}