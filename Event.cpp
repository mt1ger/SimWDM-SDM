#include "Event.h"


CircuitRequest::CircuitRequest (unsigned int src, unsigned int dest, double startTime, double duration, long long eventID, unsigned int bandwidth) {
	Src = src;
	Dest = dest;
	StartTime = startTime;
	Duration = duration;
	EventTime = startTime;
	EventID = eventID;
	EventType = c_Request;	
	Bandwidth = bandwidth;
}


CircuitRelease::CircuitRelease (long long eventID, vector<int> &circuitRoute, vector<int> & wlAllocList , double releaseTime) {
	CircuitRoute = circuitRoute;
	ReleaseTime = releaseTime;
	WLAllocList = wlAllocList;
	EventTime = releaseTime;
	EventType = c_Release;
	EventID = eventID;
}
