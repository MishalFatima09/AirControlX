#ifndef AIRCONTROLX_H
#define AIRCONTROLX_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime> // For time-related functions
#include <cstdlib> // For random number generation

using namespace std;

// Forward Declarations (to avoid circular dependencies)
class Airline;
class Aircraft;
class Runway;
class AVN;

// 1. Airlines
enum AirlineType { COMMERCIAL, CARGO, MILITARY, MEDICAL };

struct Airline {
     string name;
    AirlineType type;
    int numAircrafts;
    int numFlights; // Flights are aircrafts in operation [cite: 12]
};

// 2. Aircraft Types
enum AircraftType { COMMERCIAL_FLIGHT, CARGO_FLIGHT, EMERGENCY_FLIGHT };

// 3. Runways
enum RunwayAlignment { NORTH_SOUTH, EAST_WEST, FLEXIBLE };

struct Runway {
     string name;
    RunwayAlignment alignment;
    bool isOccupied;

    Runway( string name, RunwayAlignment alignment) : name(name), alignment(alignment), isOccupied(false) {}
};

// 6. Flight Speed Monitoring
enum FlightPhase { HOLDING, APPROACH, LANDING, TAXI, AT_GATE, TAKEOFF_ROLL, CLIMB, CRUISE, DEPARTURE };

struct Aircraft {
     string flightNumber;
    Airline airline;
    AircraftType type;
    FlightPhase phase;
    float speed;
    Runway* assignedRunway;
    bool hasAVN;
    float initialSpeed; // for storing initial speed
    float currentSpeed; // For tracking current speed

    Aircraft( string flightNumber, Airline airline, AircraftType type)
        : flightNumber(flightNumber), airline(airline), type(type), phase(AT_GATE), speed(0.0f), assignedRunway(nullptr), hasAVN(false) {
    }
};

// 7. Airspace Violation Notice (AVN)
struct AVN {
    int id;
    Aircraft* aircraft;
     time_t issueTime;
    bool isActive;
};

// Function Prototypes
void initializeAirlines( vector<Airline>& airlines);
void initializeRunways( vector<Runway>& runways);
void generateFlights( vector<Aircraft>& flights, const  vector<Airline>& airlines, float simulationTime, vector<Runway>& runways);
void simulateFlightPhases( vector<Aircraft>& flights, float deltaTime);
void checkSpeedViolations( vector<Aircraft>& flights,  vector<AVN>& avns, int& avnCounter);
void handleGroundFaults( vector<Aircraft>& flights);
void displayFlightStatus(const  vector<Aircraft>& flights);

#endif // AIRCONTROLX_H