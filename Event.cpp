#include "Event.h"


CircuitRequest::CircuitRequest (unsigned int src, unsigned int dest, double startTime, double duration, long long eventID) {
	Src = src;
	Dest = dest;
	StartTime = startTime;
	Duration = duration;
	EventTime = startTime;
	EventID = eventID;
	EventType = c_Request;	

}


CircuitRelease::CircuitRelease (long long eventID, vector<int> &circuitRoute, unsigned int core, unsigned int wavelength, double releaseTime) {
	CircuitRoute = circuitRoute;
	ReleaseTime = releaseTime;
	Core = core;
	Wavelength = wavelength;
	EventTime = releaseTime;
	EventType = c_Release;
	EventID = eventID;
}
