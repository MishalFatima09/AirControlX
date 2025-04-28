#include "m2.h" // your header file

int main() {
    // 1. Initialize airlines and runways
    vector<Airline> airlines;
    initializeAirlines(airlines);
    cout << "Initialized Airlines:\n";
    for (const auto& airline : airlines) {
        cout << "  " << airline.name << ", Flights: " << airline.numFlights << endl;
    }

    //vector<Runway> runways;
    vector<unique_ptr<Runway>> runways;
    initializeRunways(runways);
    cout << "Initialized Runways:\n";
    for (const auto& r : runways) {
        cout << r->name << endl;
    }

    // 2. Generate flights
    vector<Aircraft> flights;
    generateFlights(flights, airlines, 60); // simulate 60 minutes
    cout << "\nGenerated Flights:\n";
    displayFlightStatus(flights);

    // 3. Simulate Flight Phases (needs runways and queues)
    vector<Aircraft> arrivalQueue;
    vector<Aircraft> departureQueue;
    simulateFlightPhases(flights, 5, runways, arrivalQueue, departureQueue); // deltaTime = 5 min
    cout << "\nAfter Simulating Phases:\n";
    displayFlightStatus(flights);

    // 4. Check Speed Violations
    vector<AVN> avns;
    int avnCounter = 0;
    checkSpeedViolations(flights, avns, avnCounter);
    cout << "\nAVNs Issued: " << avns.size() << endl;

    // 5. Handle ground faults
    handleGroundFaults(flights);

    // 6. Add a new flight
    addFlight(flights, airlines);
    cout << "\nAfter Adding Flight:\n";
    displayFlightStatus(flights);

    // 7. Schedule flights into arrival/departure queues
    scheduleFlights(flights, arrivalQueue, departureQueue);

    // 8. Allocate runway for first flight
    if (!arrivalQueue.empty()) {
        // Allocate a runway for the first aircraft in the arrivalQueue
        Runway* allocatedRunway = allocateRunway(arrivalQueue[0], runways, arrivalQueue, departureQueue);
        if (allocatedRunway) {
            cout << "\nAllocated Runway: " << allocatedRunway->name << " to " << arrivalQueue[0].flightNumber << endl;
        } else {
            cout << "\nNo runway available for flight " << arrivalQueue[0].flightNumber << endl;
        }
    }
    
    // // 9. Manage arrivals and departures
    manageArrivalDepartureFlow(flights, 5, runways, arrivalQueue, departureQueue);

    // // 10. Handle priorities
    // handleFlightPriority(flights, arrivalQueue, departureQueue);

    // // 11. Manage queues/delays
    // manageQueuesAndDelays(arrivalQueue, departureQueue);

    // // 12. Track aircraft status
    // trackAircraftStatus(flights);


    // cout<<"hey"<<endl;
    return 0;
}
