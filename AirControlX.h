#ifndef AIRCONTROLX_H
#define AIRCONTROLX_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime> 
#include <cstdlib>

using namespace std;

class Airline;
class Aircraft;
class Runway;
class AVN;

// Airlines
enum AirlineType { COMMERCIAL, CARGO, MILITARY, MEDICAL }; //military and medical (MedEvac) are both in emergency

struct Airline {
    string name;
    AirlineType type;
    int numAircrafts;
	int numFlights; // basically how many aircrafts are in the air
};

// Aircraft Types
enum AircraftType { COMMERCIAL_FLIGHT, CARGO_FLIGHT, EMERGENCY_FLIGHT };

// Runways
enum RunwayAlignment { NORTH_SOUTH, EAST_WEST, FLEXIBLE };

struct Runway {
    string name;
    RunwayAlignment alignment;
    bool isOccupied;

    Runway( string name, RunwayAlignment alignment) : name(name), alignment(alignment), isOccupied(false) {}
};

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
void simulateFlightPhases(vector<Aircraft>& flights, float deltaTime, float simulationTime);

void checkSpeedViolations( vector<Aircraft>& flights,  vector<AVN>& avns, int& avnCounter);
void handleGroundFaults( vector<Aircraft>& flights);
void displayFlightStatus(const  vector<Aircraft>& flights);

#endif