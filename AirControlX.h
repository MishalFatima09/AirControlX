#ifndef AIRCONTROLX_H
#define AIRCONTROLX_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime> 
#include <cstdlib> // srand

using namespace std;

class Airline;
class Aircraft;
class Runway;
class AVN;

// 1. Airlines
enum AirlineType { COMMERCIAL, CARGO, MILITARY, MEDICAL }; //military and medical (MedEvac) are both in emergency

struct Airline {
    string name;
    AirlineType type;
    int numAircrafts;
	int numFlights; // basically how many aircrafts are in the air
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
    float initialSpeed; 
    float currentSpeed; 

    Aircraft( string flightNumber, Airline airline, AircraftType type)
        : flightNumber(flightNumber), airline(airline), type(type), phase(AT_GATE), speed(0.0f), assignedRunway(nullptr), hasAVN(false) {
    }
};

// 7. Airspace Violation Notice (AVN)
struct AVN {
    int id; //AVN id
    Aircraft* aircraft;
    time_t issueTime;
    bool isActive;
};

// Function Prototypes
void initializeAirlines( vector<Airline>& airlines);
void initializeRunways( vector<Runway>& runways);
void generateFlights( vector<Aircraft>& flights, const  vector<Airline>& airlines, float simulationTime, vector<Runway>& runways);
//void simulateFlightPhases( vector<Aircraft>& flights, float deltaTime);

void simulateFlightPhases(vector<Aircraft>& flights, float deltaTime, float simulationTime);

void checkSpeedViolations( vector<Aircraft>& flights,  vector<AVN>& avns, int& avnCounter);
void handleGroundFaults( vector<Aircraft>& flights);
void displayFlightStatus(const  vector<Aircraft>& flights);

#endif // AIRCONTROLX_H