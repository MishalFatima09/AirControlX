#include "AirControlX.h"
#include <iostream>
#include <cstdlib> // For random number generation
#include <ctime>   // For time functions

using namespace std;

// 1. Airlines Initialization
void initializeAirlines( vector<Airline>& airlines) {
    airlines.push_back({ "PIA", COMMERCIAL, 6, 4 });
    airlines.push_back({ "AirBlue", COMMERCIAL, 4, 4 });
    airlines.push_back({ "FedEx", CARGO, 3, 2 });
    airlines.push_back({ "Pakistan Airforce", MILITARY, 2, 1 });
    airlines.push_back({ "Blue Dart", CARGO, 2, 2 });
    airlines.push_back({ "AghaKhan Air Ambulance", MEDICAL, 2, 1 });
}

// 3. Runways Initialization
void initializeRunways( vector<Runway>& runways) {
    runways.push_back({ "RWY-A", NORTH_SOUTH });
    runways.push_back({ "RWY-B", EAST_WEST });
    runways.push_back({ "RWY-C", FLEXIBLE });
}

// Function to generate a random float between min and max
float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// Function to get AircraftType from AirlineType
AircraftType getAircraftTypeFromAirlineType(AirlineType airlineType) {
    switch (airlineType) {
    case CARGO:
        return CARGO_FLIGHT;
    case MEDICAL:
    case MILITARY:
        return EMERGENCY_FLIGHT;
    default:
        return COMMERCIAL_FLIGHT;
    }
}

// 4. Flight Arrivals and Dispatching + 5. Cargo Flight Restrictions
void generateFlights( vector<Aircraft>& flights, const  vector<Airline>& airlines, float simulationTime, vector<Runway>& runways) {
    //track the cargo flights to ensure there is at least one.
    bool cargoFlightGenerated = false;
    for (const auto& airline : airlines) {
        for (int i = 0; i < airline.numFlights; ++i) {
            // Create a flight number (e.g., "PIA123", "FDX456")
             string flightNumber = airline.name +  to_string(i + 1);

            // Determine AircraftType based on AirlineType
            AircraftType aircraftType = getAircraftTypeFromAirlineType(airline.type);

            Aircraft newAircraft(flightNumber, airline, aircraftType);

            //assign cargo flight
            if (aircraftType == CARGO_FLIGHT && !cargoFlightGenerated) {
                newAircraft.assignedRunway = &runways[2]; // Assign to RWY-C
                cargoFlightGenerated = true;
            }

            flights.push_back(newAircraft);
        }
    }
    if (!cargoFlightGenerated && !flights.empty()) {
        flights[0].type = CARGO_FLIGHT;
        flights[0].assignedRunway = &runways[2];
    }
}

// 6. Flight Speed Monitoring
void simulateFlightPhases( vector<Aircraft>& flights, float deltaTime) {
    for (auto& aircraft : flights) {
        switch (aircraft.phase) {
        case AT_GATE:
            aircraft.speed = 0.0f;
            if (deltaTime > 2.0f) { // Example: Taxi after 2 seconds
                aircraft.phase = TAXI;
                aircraft.speed = randomFloat(15.0f, 30.0f);
            }
            break;
        case TAXI:
            if (deltaTime > 5.0f) { // Example: Takeoff after 5 seconds
                aircraft.phase = TAKEOFF_ROLL;
                aircraft.speed = 0.0f;
            }
            break;
        case TAKEOFF_ROLL:
            aircraft.speed += 50.0f * deltaTime; // Example acceleration
            if (aircraft.speed > 290.0f) {
                aircraft.hasAVN = true; // Speed violation
            }
            if (deltaTime > 10.0f) { // Example: Climb after 10 seconds
                aircraft.phase = CLIMB;
                aircraft.speed = randomFloat(250.0f, 463.0f);
            }
            break;
        case CLIMB:
            if (deltaTime > 15.0f) {
                aircraft.phase = CRUISE;
                aircraft.speed = randomFloat(800.0f, 900.0f);
            }
            break;
        case CRUISE:
            // Cruise logic here
            break;
        case APPROACH:
            // Approach logic here
            break;
        case LANDING:
            // Landing logic here
            break;
        case HOLDING:
            // Holding logic here
            break;
        case DEPARTURE:
            // Departure logic here
            break;
        }
    }
}

void checkSpeedViolations( vector<Aircraft>& flights,  vector<AVN>& avns, int& avnCounter) {
    for (auto& aircraft : flights) {
        switch (aircraft.phase) {
        case APPROACH:
            if (aircraft.speed < 240.0f || aircraft.speed > 290.0f) {
                aircraft.hasAVN = true;
            }
            break;
        case TAKEOFF_ROLL:
            if (aircraft.speed > 290.0f) {
                aircraft.hasAVN = true;
            }
            break;
        case AT_GATE:
            if (aircraft.speed > 10.0f) {
                aircraft.hasAVN = true;
            }
            break;
        case TAXI:
            if (aircraft.speed > 30.0f) {
                aircraft.hasAVN = true;
            }
            break;
        case CLIMB:
            if (aircraft.speed > 463.0f) {
                aircraft.hasAVN = true;
            }
            break;
        case CRUISE:
            if (aircraft.speed < 800.0f || aircraft.speed > 900.0f) {
                aircraft.hasAVN = true;
            }
            break;
        case LANDING:
            if (aircraft.speed > 240.0f) {
                aircraft.hasAVN = true;
            }
            break;
        case HOLDING:
            if (aircraft.speed > 600.0f) {
                aircraft.hasAVN = true;
            }
            break;
        case DEPARTURE:
            break;
        }
        if (aircraft.hasAVN) {
            AVN avn;
            avn.id = ++avnCounter;
            avn.aircraft = &aircraft;
            avn.issueTime =  time(0); // Current time
            avn.isActive = true;
            avns.push_back(avn);
            aircraft.hasAVN = false; // Reset flag
        }
    }
}

// 8. Fault Handling (Ground Only)
void handleGroundFaults( vector<Aircraft>& flights) {
    //basic implemenation. Needs further elaboration
    if (rand() % 100 < 5) { // 5% chance of a ground fault
        int faultIndex = rand() % flights.size();
        if (flights[faultIndex].phase == TAXI || flights[faultIndex].phase == AT_GATE) {
             cout << "Ground fault detected for flight: " << flights[faultIndex].flightNumber <<  endl;
            flights.erase(flights.begin() + faultIndex);
        }
    }
}

void displayFlightStatus(const  vector<Aircraft>& flights) {
     cout << "\n--- Flight Status ---" <<  endl;
    for (const auto& aircraft : flights) {
         cout << "Flight: " << aircraft.flightNumber
            << ", Phase: ";
        switch (aircraft.phase) {
        case AT_GATE:  cout << "At Gate"; break;
        case TAXI:  cout << "Taxi"; break;
        case TAKEOFF_ROLL:  cout << "Takeoff Roll"; break;
        case CLIMB:  cout << "Climb"; break;
        case CRUISE:  cout << "Cruise"; break;
        case APPROACH:  cout << "Approach"; break;
        case LANDING:  cout << "Landing"; break;
        case HOLDING:  cout << "Holding"; break;
        case DEPARTURE:  cout << "Departure"; break;
        }
         cout << ", Speed: " << aircraft.speed << " km/h"
            << ", Runway: " << (aircraft.assignedRunway ? aircraft.assignedRunway->name : "None")
            << (aircraft.hasAVN ? ", AVN Active" : "") <<  endl;
    }
}