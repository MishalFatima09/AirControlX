#include "AirControlX.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    vector<Airline> airlines;
    vector<Runway> runways;
    vector<Aircraft> flights;
    vector<AVN> avns;
    int avnCounter = 0;

    initializeAirlines(airlines);
    initializeRunways(runways);

    float simulationTime = 0.0f;
    float deltaTime = 1.0f; // Time step for simulation (adjust as needed)
    const float simulationDuration = 300.0f; // 5 minutes in seconds

    generateFlights(flights, airlines, simulationTime, runways);

    while (simulationTime < simulationDuration) {
        simulateFlightPhases(flights, deltaTime);
        checkSpeedViolations(flights, avns, avnCounter);
        handleGroundFaults(flights);
        displayFlightStatus(flights);

        simulationTime += deltaTime;
         this_thread::sleep_for( chrono::milliseconds(1000)); // Simulate time passing
    }

     cout << "\n--- Simulation Complete ---" <<  endl;

    return 0;
}