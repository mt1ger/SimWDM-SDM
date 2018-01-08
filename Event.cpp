#include "Event.h"


CircuitRequest::CircuitRequest (unsigned int src, unsigned int dest, double startTime, double duration, long long eventID, unsigned int dataSize, unsigned int occupiedWavelengths) {
	Src = src;
	Dest = dest;
	StartTime = startTime;
	Duration = duration;
	EventTime = startTime;
	EventID = eventID;
	EventType = c_Request;	
	DataSize = dataSize;
	OccupiedWavelengths = occupiedWavelengths;
}


CircuitRelease::CircuitRelease (long long eventID, vector<int> &circuitRoute, vector< vector<int> > & wlAllocList , double releaseTime, unsigned int transpondersUsed) {
	CircuitRoute = circuitRoute;
	ReleaseTime = releaseTime;
	WLAllocList = wlAllocList;
	EventTime = releaseTime;
	EventType = c_Release;
	EventID = eventID;
	TranspondersUsed = transpondersUsed;
}
