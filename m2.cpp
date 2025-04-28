#include "m2.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <map>
#include <algorithm> // For sorting

using namespace std;

// Helper Functions (from Module 1, with minor updates)
float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

AircraftType getAircraftTypeFromAirlineType(AirlineType airlineType) {
    switch (airlineType) {
        case COMMERCIAL:
            return COMMERCIAL_FLIGHT;
        case CARGO:
            return CARGO_FLIGHT;
        case MILITARY:
        case MEDICAL:
            return EMERGENCY_FLIGHT;
        default:
            // Default case (shouldn't reach here based on the enum)
            return COMMERCIAL_FLIGHT;
    }
}
// 1. Airlines Initialization (unchanged from Module 1)
void initializeAirlines(vector<Airline>& airlines) {
    airlines.push_back(Airline("PIA", COMMERCIAL, 6, 4));
    airlines.push_back(Airline("AirBlue", COMMERCIAL, 4, 4));
    airlines.push_back(Airline("FedEx", CARGO, 3, 2));
    airlines.push_back(Airline("Pakistan Airforce", MILITARY, 2, 1));
    airlines.push_back(Airline("Blue Dart", CARGO, 2, 2));
    airlines.push_back(Airline("AghaKhan Air Ambulance", MEDICAL, 2, 1));
}

// 3. Runways Initialization (unchanged from Module 1)
void initializeRunways(std::vector<std::unique_ptr<Runway>>& runways) {
    runways.push_back(std::make_unique<Runway>("RWY-A", NORTH_SOUTH));
    runways.push_back(std::make_unique<Runway>("RWY-B", EAST_WEST));
    runways.push_back(std::make_unique<Runway>("RWY-C", FLEXIBLE));
}


// 4. Flight Arrivals and Dispatching + 5. Cargo Flight Restrictions (Updated)
void generateFlights(vector<Aircraft>& flights, const vector<Airline>& airlines, float simulationTime) {
    bool cargoFlightGenerated = false;
    for (const auto& airline : airlines) {
        for (int i = 0; i < airline.numFlights; ++i) {
            string flightNumber = airline.name + to_string(i + 1);
            AircraftType aircraftType = getAircraftTypeFromAirlineType(airline.type);
            Aircraft newAircraft(flightNumber, airline, aircraftType);

            // Assign flight direction based on airline type (simplified)
            if (airline.type == COMMERCIAL) {
                if (rand() % 2 == 0) newAircraft.direction = NORTH;
                else newAircraft.direction = SOUTH;
            } else {
                if (rand() % 2 == 0) newAircraft.direction = EAST;
                else newAircraft.direction = WEST;
            }

            //Basic priority assignment
            if (airline.type == MEDICAL || airline.type == MILITARY) {
                newAircraft.priority = EMERGENCY_PRIORITY;
            } else if (airline.type == CARGO) {
                newAircraft.priority = CARGO_PRIORITY;
            }

            //Basic schedule time (needs improvement)
            newAircraft.scheduledTime = time(0) + rand() % 300; // Now + up to 5 minutes

            if (aircraftType == CARGO_FLIGHT && !cargoFlightGenerated) {
                cargoFlightGenerated = true;
            }

            flights.push_back(newAircraft);
        }
    }
    if (!cargoFlightGenerated && !flights.empty()) {
        flights[0].type = CARGO_FLIGHT;
    }
}


void simulateFlightPhases(vector<Aircraft>& flights, float deltaTime, vector<unique_ptr<Runway>>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue) {
    for (auto& aircraft : flights) {
        switch (aircraft.phase) {
            case WAITING:
                // (Optional) Logic for checking arrivalQueue/departureQueue and assigning runways
                break;

            case HOLDING:
                aircraft.speed = randomFloat(400.0f, 600.0f);
                if (deltaTime > 2.0f) {
                    aircraft.phase = APPROACH;
                }
                break;

            case APPROACH:
                aircraft.speed = randomFloat(240.0f, 290.0f);
                if (deltaTime > 4.0f) {
                    aircraft.phase = LANDING;
                }
                break;

            case LANDING:
                aircraft.speed = 240.0f;
                aircraft.speed -= 20.0f * deltaTime;
                if (aircraft.speed < 30.0f || deltaTime > 8.0f) {
                    aircraft.phase = TAXI;
                    aircraft.speed = randomFloat(15.0f, 30.0f);
                }
                break;

            case TAXI:
                if (aircraft.assignedRunway) {
                    aircraft.assignedRunway->release(); // release the runway (raw pointer)
                    aircraft.assignedRunway = nullptr;   // clear pointer after release
                }

                if (aircraft.type == CARGO_FLIGHT || aircraft.type == EMERGENCY_FLIGHT) {
                    if (deltaTime > 10.0f) {
                        aircraft.phase = AT_GATE;
                        aircraft.speed = 0.0f;
                    }
                } else if (aircraft.direction == EAST || aircraft.direction == WEST) {
                    if (deltaTime > 10.0f) {
                        aircraft.phase = TAKEOFF_ROLL;
                        aircraft.speed = 0.0f;
                    }
                } else {
                    if (deltaTime > 10.0f) {
                        aircraft.phase = AT_GATE;
                        aircraft.speed = 0.0f;
                    }
                }
                break;

            case AT_GATE:
                aircraft.speed = 0.0f;
                break;

            case TAKEOFF_ROLL:
                aircraft.speed += 50.0f * deltaTime;
                if (aircraft.speed > 290.0f) {
                    aircraft.hasAVN = true;
                }
                if (deltaTime > 5.0f) {
                    aircraft.phase = CLIMB;
                    aircraft.speed = randomFloat(250.0f, 463.0f);
                }
                break;

            case CLIMB:
                if (deltaTime > 10.0f) {
                    aircraft.phase = CRUISE;
                    aircraft.speed = randomFloat(800.0f, 900.0f);
                }
                break;

            case CRUISE:
                // Cruise phase
                break;

            case DEPARTURE:
                // Departure phase
                break;
        }
    }
}


// // 6. Flight Speed Monitoring (Updated with phase transitions)
// void simulateFlightPhases(vector<Aircraft>& flights, float deltaTime, vector<Runway>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue) {
//     for (auto& aircraft : flights) {
//         switch (aircraft.phase) {
//             case WAITING:
//                 // Logic to move to HOLDING/TAXI based on schedule and runway availability
//                 // This is where you'd check queues and runway allocation
//                 break;
//             case HOLDING:
//                 aircraft.speed = randomFloat(400.0f, 600.0f);
//                 if (deltaTime > 2.0f) {
//                     aircraft.phase = APPROACH;
//                 }
//                 break;
//             case APPROACH:
//                 aircraft.speed = randomFloat(240.0f, 290.0f);
//                 if (deltaTime > 4.0f) {
//                     aircraft.phase = LANDING;
//                 }
//                 break;
//             case LANDING:
//                 aircraft.speed = 240.0f;
//                 aircraft.speed -= 20.0f * deltaTime;
//                 if (aircraft.speed < 30.0f || deltaTime > 8.0f) {
//                     aircraft.phase = TAXI;
//                     aircraft.speed = randomFloat(15.0f, 30.0f);
//                 }
//                 break;
//             case TAXI:
//                 if (aircraft.assignedRunway) {
//                     aircraft.assignedRunway->release();
//                     aircraft.assignedRunway = nullptr;
//                 }
//                 if (aircraft.type == CARGO_FLIGHT || aircraft.type == EMERGENCY_FLIGHT) {
//                     if (deltaTime > 10.0f) {
//                         aircraft.phase = AT_GATE;
//                     }
//                 } else if (aircraft.direction == EAST || aircraft.direction == WEST) {
//                     if (deltaTime > 10.0f) {
//                         aircraft.phase = TAKEOFF_ROLL;
//                         aircraft.speed = 0.0f;
//                     }
//                 } else {
//                     if (deltaTime > 10.0f) {
//                         aircraft.phase = AT_GATE;
//                     }
//                 }
//                 break;
//             case AT_GATE:
//                 aircraft.speed = 0.0f;
//                 break;
//             case TAKEOFF_ROLL:
//                 aircraft.speed += 50.0f * deltaTime;
//                 if (aircraft.speed > 290.0f) {
//                     aircraft.hasAVN = true;
//                 }
//                 if (deltaTime > 5.0f) {
//                     aircraft.phase = CLIMB;
//                     aircraft.speed = randomFloat(250.0f, 463.0f);
//                 }
//                 break;
//             case CLIMB:
//                 if (deltaTime > 10.0f) {
//                     aircraft.phase = CRUISE;
//                     aircraft.speed = randomFloat(800.0f, 900.0f);
//                 }
//                 break;
//             case CRUISE:
//                 // Cruise logic
//                 break;
//             case DEPARTURE:
//                 // Departure logic
//                 break;
//         }
//     }
// }

// 7. Airspace Violation Notice (AVN) (unchanged from Module 1)
void checkSpeedViolations(vector<Aircraft>& flights, vector<AVN>& avns, int& avnCounter) {
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
            avn.issueTime = time(0);
            avn.isActive = true;
            avns.push_back(avn);
            aircraft.hasAVN = false;
        }
    }
}

// 8. Fault Handling (Ground Only) (unchanged from Module 1)
void handleGroundFaults(vector<Aircraft>& flights) {
    if (rand() % 100 < 5) {
        int faultIndex = rand() % flights.size();
        if (flights[faultIndex].phase == TAXI || flights[faultIndex].phase == AT_GATE) {
            cout << "Ground fault detected for flight: " << flights[faultIndex].flightNumber << endl;
            flights.erase(flights.begin() + faultIndex);
        }
    }
}

void displayFlightStatus(const vector<Aircraft>& flights) {
    cout << "\n--- Flight Status ---" << endl;
    for (const auto& aircraft : flights) {
        cout << "Flight: " << aircraft.flightNumber
                  << ", Phase: ";
        switch (aircraft.phase) {
            case WAITING: cout << "Waiting"; break;
            case AT_GATE: cout << "At Gate"; break;
            case TAXI: cout << "Taxi"; break;
            case TAKEOFF_ROLL: cout << "Takeoff Roll"; break;
            case CLIMB: cout << "Climb"; break;
            case CRUISE: cout << "Cruise"; break;
            case APPROACH: cout << "Approach"; break;
            case LANDING: cout << "Landing"; break;
            case HOLDING: cout << "Holding"; break;
            case DEPARTURE: cout << "Departure"; break;
        }
        cout << ", Speed: " << aircraft.speed << " km/h"
                  << ", Runway: " << (aircraft.assignedRunway ? aircraft.assignedRunway->name : "None")
                  << ", Direction: ";
        switch (aircraft.direction) {
            case NORTH: cout << "North"; break;
            case SOUTH: cout << "South"; break;
            case EAST: cout << "East"; break;
            case WEST: cout << "West"; break;
        }
        cout << ", Priority: ";
        switch (aircraft.priority) {
            case EMERGENCY_PRIORITY: cout << "Emergency"; break;
            case VIP_PRIORITY: cout << "VIP"; break;
            case CARGO_PRIORITY: cout << "Cargo"; break;
            case COMMERCIAL_PRIORITY: cout << "Commercial"; break;
        }
        cout << (aircraft.hasAVN ? ", AVN Active" : "") << endl;
    }
}

// ---------------------- Module 2 Functions --------------------------

// // FR1.1: The system shall allow entry of arrival and departure flight data...
void addFlight(vector<Aircraft>& flights, const vector<Airline>& airlines) {
    // Implementation for adding a new flight
    // This would typically involve user input or reading from a file/database
    // For simplicity, let's create a dummy flight

    // Find an airline (for example, the first one)
    if (!airlines.empty()) {
        const Airline& airline = *airlines.begin();
        string flightNumber = "TEST123";
        AircraftType type = COMMERCIAL_FLIGHT;
        FlightDirection direction = NORTH;
        FlightPriority priority = COMMERCIAL_PRIORITY;
        time_t scheduledTime = time(0) + 600; // Now + 10 minutes

        Aircraft newAircraft(flightNumber, airline, type);
        newAircraft.direction = direction;
        newAircraft.priority = priority;
        newAircraft.scheduledTime = scheduledTime;

        flights.push_back(newAircraft);
        cout << "Flight " << flightNumber << " added to the system.\n";
    } else {
        cout << "No airlines available to add flight.\n";
    }
}

// // FR1.2: The system shall generate a schedule queue for both incoming and outgoing flights.
// // This function now also considers scheduled time
void scheduleFlights(vector<Aircraft>& flights, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue) {
    arrivalQueue.clear();
    departureQueue.clear();

    time_t currentTime = time(0);

    for (Aircraft& flight : flights) {
        if (flight.phase == WAITING) {
            if (flight.direction == NORTH || flight.direction == SOUTH) {
                if (flight.scheduledTime <= currentTime) {
                    arrivalQueue.push_back(flight);
                }
            } else if (flight.direction == EAST || flight.direction == WEST) {
                if (flight.scheduledTime <= currentTime) {
                    departureQueue.push_back(flight);
                }
            }
        }
    }

    // Sort queues by priority and then scheduled time (FCFS within priority)
    sort(arrivalQueue.begin(), arrivalQueue.end(), [](const Aircraft& a, const Aircraft& b) {
        if (a.priority != b.priority) {
            return a.priority < b.priority; // Higher priority comes first (lower enum value is higher priority)
        }
        return a.scheduledTime < b.scheduledTime; // Earlier scheduled time comes first
    });

    sort(departureQueue.begin(), departureQueue.end(), [](const Aircraft& a, const Aircraft& b) {
        if (a.priority != b.priority) {
            return a.priority < b.priority;
        }
        return a.scheduledTime < b.scheduledTime;
    });

    cout << "Arrival Queue Size: " << arrivalQueue.size() << ", Departure Queue Size: " << departureQueue.size() << endl;
}

Runway* allocateRunway(Aircraft& aircraft, vector<unique_ptr<Runway>>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue) {
    // Check direction and assign runways accordingly
    if (aircraft.direction == NORTH || aircraft.direction == SOUTH) {
        if (!runways[0]->isOccupied) {
            runways[0]->acquire();
            return runways[0].get();  // RWY-A
        } else if (!runways[2]->isOccupied && aircraft.priority == EMERGENCY_PRIORITY) {
            runways[2]->acquire();
            return runways[2].get();  // RWY-C for emergency
        }
    } else if (aircraft.direction == EAST || aircraft.direction == WEST) {
        if (!runways[1]->isOccupied) {
            runways[1]->acquire();
            return runways[1].get();  // RWY-B
        } else if (!runways[2]->isOccupied && aircraft.priority == EMERGENCY_PRIORITY) {
            runways[2]->acquire();
            return runways[2].get();  // RWY-C for emergency
        }
    }

    // If it's a cargo flight and RWY-C is available
    if (aircraft.type == CARGO_FLIGHT && !runways[2]->isOccupied) {
        runways[2]->acquire();
        return runways[2].get();  // RWY-C for cargo
    }

    return nullptr;  // No runway available
}



// // FR2.1 & FR2.2: The system shall allocate runways based on flight direction... Only one aircraft may use a runway at a time...
// Runway* allocateRunway(Aircraft& aircraft, vector<Runway>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue) {
//     if (aircraft.direction == NORTH || aircraft.direction == SOUTH) {
//         if (!runways[0].isOccupied) {
//             runways[0].acquire();
//             return &runways[0]; // RWY-A
//         } else if (!runways[2].isOccupied && aircraft.priority == EMERGENCY_PRIORITY) {
//             runways[2].acquire();
//             return &runways[2]; // RWY-C for emergency
//         }
//     } else if (aircraft.direction == EAST || aircraft.direction == WEST) {
//         if (!runways[1].isOccupied) {
//             runways[1].acquire();
//             return &runways[1]; // RWY-B
//         } else if (!runways[2].isOccupied && aircraft.priority == EMERGENCY_PRIORITY) {
//             runways[2].acquire();
//             return &runways[2]; // RWY-C for emergency
//         }
//     }
//     if (aircraft.type == CARGO_FLIGHT && !runways[2].isOccupied) {
//         runways[2].acquire();
//         return &runways[2]; // RWY-C for cargo
//     }
//     return nullptr; // No runway available
// }

// // FR3.1 & FR3.2: The system shall manage aircraft progress through phases... Aircraft movements shall follow real-world timing...
// void manageArrivalDepartureFlow(vector<Aircraft>& flights, float deltaTime, vector<Runway>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue) {
//     for (auto& aircraft : flights) {
//         if (aircraft.phase == WAITING) {
//             // Check if the aircraft is at the front of the appropriate queue and a runway is available
//             if ((aircraft.direction == NORTH || aircraft.direction == SOUTH) &&
//                 !arrivalQueue.empty() && &aircraft == &arrivalQueue.front()) {
//                 Runway* runway = allocateRunway(aircraft, runways, arrivalQueue, departureQueue);
//                 if (runway) {
//                     aircraft.assignedRunway = runway;
//                     aircraft.phase = HOLDING;
//                     aircraft.arrivalTime = time(0); // Record arrival time
//                     arrivalQueue.erase(arrivalQueue.begin()); // Remove from queue
//                     cout << "Flight " << aircraft.flightNumber << " assigned to " << runway->name << " for arrival.\n";
//                 }
//             } else if ((aircraft.direction == EAST || aircraft.direction == WEST) &&
//             !departureQueue.empty() && &aircraft == &departureQueue.front()) {
//      Runway* runway = allocateRunway(aircraft, runways, arrivalQueue, departureQueue);
//      if (runway) {
//          aircraft.assignedRunway = runway;
//          aircraft.phase = TAXI; // Start taxiing for departure
//          aircraft.departureTime = time(0); // Record departure time
//          departureQueue.erase(departureQueue.begin()); // Remove from queue
//          cout << "Flight " << aircraft.flightNumber << " assigned to " << runway->name << " for departure.\n";
//      }
//  }
// }
// // Phase transitions based on time and conditions (simplified)
// switch (aircraft.phase) {
//  case HOLDING:
//      if (time(0) - aircraft.arrivalTime > 5) { // Example: Hold for 5 seconds
//          aircraft.phase = APPROACH;
//          cout << "Flight " << aircraft.flightNumber << " begins approach.\n";
//      }
//      break;
//  case APPROACH:
//      if (time(0) - aircraft.arrivalTime > 10) { // Example: Approach takes 5 seconds
//          aircraft.phase = LANDING;
//          cout << "Flight " << aircraft.flightNumber << " begins landing.\n";
//      }
//      break;
//  case LANDING:
//      if (time(0) - aircraft.arrivalTime > 15) { // Example: Landing takes 5 seconds
//          aircraft.phase = TAXI;
//          aircraft.assignedRunway->release(); // Release the runway
//          aircraft.assignedRunway = nullptr;
//          cout << "Flight " << aircraft.flightNumber << " exits runway.\n";
//      }
//      break;
//  case TAXI:
//      if (aircraft.direction == NORTH || aircraft.direction == SOUTH) {
//          if (time(0) - aircraft.arrivalTime > 20) { // Example: Taxi to gate after landing
//              aircraft.phase = AT_GATE;
//              cout << "Flight " << aircraft.flightNumber << " arrives at gate.\n";
//          }
//      } else {
//          if (time(0) - aircraft.departureTime > 5) { // Example: Taxi to takeoff
//              aircraft.phase = TAKEOFF_ROLL;
//              cout << "Flight " << aircraft.flightNumber << " begins takeoff roll.\n";
//          }
//      }
//      break;
//  case TAKEOFF_ROLL:
//      if (time(0) - aircraft.departureTime > 10) { // Example: Takeoff
//          aircraft.phase = DEPARTURE;
//          aircraft.assignedRunway->release();
//          aircraft.assignedRunway = nullptr;
//          cout << "Flight " << aircraft.flightNumber << " departs.\n";
//      }
//      break;
//  case DEPARTURE:
//      // Departure logic (e.g., remove from simulation after some time)
//      break;
//  // Add cases for other phases as needed (CLIMB, CRUISE, etc.)
// }
// }
// }


void manageArrivalDepartureFlow(vector<Aircraft>& flights, float deltaTime, vector<unique_ptr<Runway>>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue) {
    for (auto& aircraft : flights) {
        if (aircraft.phase == WAITING) {
            // Check if the aircraft is at the front of the appropriate queue and a runway is available
            if ((aircraft.direction == NORTH || aircraft.direction == SOUTH) &&
                !arrivalQueue.empty() && &aircraft == &arrivalQueue.front()) {
                Runway* runway = allocateRunway(aircraft, runways, arrivalQueue, departureQueue);
                if (runway) {
                    aircraft.assignedRunway = runway;  // Assign raw pointer
                    aircraft.phase = HOLDING;
                    aircraft.arrivalTime = time(0); // Record arrival time
                    arrivalQueue.erase(arrivalQueue.begin()); // Remove from queue
                    cout << "Flight " << aircraft.flightNumber << " assigned to " << aircraft.assignedRunway->name << " for arrival.\n";
                }
            } else if ((aircraft.direction == EAST || aircraft.direction == WEST) &&
                !departureQueue.empty() && &aircraft == &departureQueue.front()) {
                Runway* runway = allocateRunway(aircraft, runways, arrivalQueue, departureQueue);
                if (runway) {
                    aircraft.assignedRunway = std::move(runway);  // Transfer ownership
                    aircraft.phase = TAXI; // Start taxiing for departure
                    aircraft.departureTime = time(0); // Record departure time
                    departureQueue.erase(departureQueue.begin()); // Remove from queue
                    cout << "Flight " << aircraft.flightNumber << " assigned to " << aircraft.assignedRunway->name << " for departure.\n";
                }
            }
        }

        // Phase transitions based on time and conditions (simplified)
        switch (aircraft.phase) {
            case HOLDING:
                if (time(0) - aircraft.arrivalTime > 5) { // Example: Hold for 5 seconds
                    aircraft.phase = APPROACH;
                    cout << "Flight " << aircraft.flightNumber << " begins approach.\n";
                }
                break;
            case APPROACH:
                if (time(0) - aircraft.arrivalTime > 10) { // Example: Approach takes 5 seconds
                    aircraft.phase = LANDING;
                    cout << "Flight " << aircraft.flightNumber << " begins landing.\n";
                }
                break;
            case LANDING:
                if (time(0) - aircraft.arrivalTime > 15) { // Example: Landing takes 5 seconds
                    aircraft.phase = TAXI;
                    aircraft.assignedRunway->release(); // Release the runway
                    aircraft.assignedRunway = nullptr; // Reset the runway pointer
                    cout << "Flight " << aircraft.flightNumber << " exits runway.\n";
                }
                break;
            case TAXI:
                if (aircraft.direction == NORTH || aircraft.direction == SOUTH) {
                    if (time(0) - aircraft.arrivalTime > 20) { // Example: Taxi to gate after landing
                        aircraft.phase = AT_GATE;
                        cout << "Flight " << aircraft.flightNumber << " arrives at gate.\n";
                    }
                } else {
                    if (time(0) - aircraft.departureTime > 5) { // Example: Taxi to takeoff
                        aircraft.phase = TAKEOFF_ROLL;
                        cout << "Flight " << aircraft.flightNumber << " begins takeoff roll.\n";
                    }
                }
                break;
            case TAKEOFF_ROLL:
                if (time(0) - aircraft.departureTime > 10) { // Example: Takeoff
                    aircraft.phase = DEPARTURE;
                    aircraft.assignedRunway->release();
                    aircraft.assignedRunway = nullptr;
                    cout << "Flight " << aircraft.flightNumber << " departs.\n";
                }
                break;
            case DEPARTURE:
                // Departure logic (e.g., remove from simulation after some time)
                break;
            // Add cases for other phases as needed (CLIMB, CRUISE, etc.)
        }
    }
}
