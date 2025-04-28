// #ifndef AIRCONTROLX_H
// #define AIRCONTROLX_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <queue> // For flight queues
#include <map>   // For scheduled times
#include <thread> // For thread synchronization (if needed)
#include <mutex> // For mutex (if needed)
#include <condition_variable> // For condition variable (if needed)

using namespace std;

// Forward Declarations
class Airline;
class Aircraft;
class Runway;
class AVN;

// 1. Airlines (unchanged from Module 1)
enum AirlineType { COMMERCIAL = 0, CARGO = 1, MILITARY = 2, MEDICAL = 3 };

struct Airline {
    string name;
    AirlineType type;
    int numAircrafts;
	int numFlights; // basically how many aircrafts are in the air

     // Constructor to initialize Airline
     Airline(const string& name, AirlineType type, int numAircrafts, int numFlights)
     : name(name), type(type), numAircrafts(numAircrafts), numFlights(numFlights) {}
};


// 2. Aircraft Types (unchanged from Module 1)
enum AircraftType { COMMERCIAL_FLIGHT = 0, CARGO_FLIGHT = 1, EMERGENCY_FLIGHT = 2 };

// 3. Runways (updated with mutex for synchronization)
enum RunwayAlignment { NORTH_SOUTH, EAST_WEST, FLEXIBLE };

struct Runway {
    string name;
    RunwayAlignment alignment;
    bool isOccupied;
    mutex runwayMutex; // Mutex for runway access

    Runway(string name, RunwayAlignment alignment) : name(name), alignment(alignment), isOccupied(false) {}

    void acquire() { runwayMutex.lock(); isOccupied = true; }
    void release() { runwayMutex.unlock(); isOccupied = false; }
};

// 4. Flight Arrivals and Dispatching + 5. Cargo Flight Restrictions
// 6. Flight Speed Monitoring (updated with priority and scheduled time)
enum FlightPhase {
    WAITING,
    HOLDING,
    APPROACH,
    LANDING,
    TAXI,
    AT_GATE,
    TAKEOFF_ROLL,
    CLIMB,
    CRUISE,
    DEPARTURE
};

enum FlightDirection { NORTH, SOUTH, EAST, WEST }; // Added for directions
enum FlightPriority { EMERGENCY_PRIORITY, VIP_PRIORITY, CARGO_PRIORITY, COMMERCIAL_PRIORITY };

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
    FlightDirection direction; // Added direction
    FlightPriority priority;  // Added priority
    time_t scheduledTime; // Added scheduled time
    time_t arrivalTime;
    time_t departureTime;

    Aircraft(string flightNumber, Airline airline, AircraftType type)
        : flightNumber(flightNumber), airline(airline), type(type), phase(WAITING), speed(0.0f),
          assignedRunway(nullptr), hasAVN(false), direction(NORTH), priority(COMMERCIAL_PRIORITY) {}
};

// 7. Airspace Violation Notice (AVN) (unchanged from Module 1)
struct AVN {
    int id;
    Aircraft* aircraft;
    time_t issueTime;
    bool isActive;
};

// Function Prototypes
void initializeAirlines(vector<Airline>& airlines);
void initializeRunways(vector<unique_ptr<Runway>>& runways);
void generateFlights(vector<Aircraft>& flights, const vector<Airline>& airlines, float simulationTime);

void simulateFlightPhases(std::vector<Aircraft>& flights, float deltaTime, std::vector<std::unique_ptr<Runway>>& runways, std::vector<Aircraft>& arrivalQueue, std::vector<Aircraft>& departureQueue);
//void simulateFlightPhases(vector<Aircraft>& flights, float deltaTime, vector<Runway>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue);
void checkSpeedViolations(vector<Aircraft>& flights, vector<AVN>& avns, int& avnCounter);
void handleGroundFaults(vector<Aircraft>& flights);
void displayFlightStatus(const vector<Aircraft>& flights);

// // Module 2 Functions
void addFlight(vector<Aircraft>& flights, const vector<Airline>& airlines);
void scheduleFlights(vector<Aircraft>& flights, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue);

Runway* allocateRunway(Aircraft& aircraft, vector<unique_ptr<Runway>>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue);

// Runway* allocateRunway(Aircraft& aircraft, vector<Runway>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue);


void manageArrivalDepartureFlow(vector<Aircraft>& flights, float deltaTime, vector<unique_ptr<Runway>>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue);

// void manageArrivalDepartureFlow(vector<Aircraft>& flights, float deltaTime, vector<Runway>& runways, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue);
// void handleFlightPriority(vector<Aircraft>& flights, vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue);
// void manageQueuesAndDelays(vector<Aircraft>& arrivalQueue, vector<Aircraft>& departureQueue);
// void trackAircraftStatus(const vector<Aircraft>& flights);

// #endif // AIRCONTROLX_H