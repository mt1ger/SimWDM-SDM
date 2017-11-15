#ifndef _EVENT_H
#define _EVENT_H


#include <iostream>
#include <vector>


using namespace std;

enum Event_t {c_Request, c_Release};

class Event {
	public:
	Event () {}
	~Event () {}

	// unsigned int EventID;
	double EventTime;
	long long EventID;
	Event_t EventType;

	private:
};


class CircuitRequest : public Event {
	public:
		CircuitRequest (unsigned int src, unsigned int dest, double startTime, double duration, long long eventID, unsigned int bandwidth); 
		~CircuitRequest () {}
		unsigned int Src;
		unsigned int Dest;
		double StartTime;
		double Duration;
		unsigned int Bandwidth;
};


class CircuitRelease : public Event {
	public:
		CircuitRelease (long long eventID, vector<int> & circuitRoute, vector<vector<int> > & wlAllocList, double releaseTime);
		~CircuitRelease () {}
		vector< vector<int> > WLAllocList;
		vector<int> CircuitRoute;
		double ReleaseTime;
	
};

#endif
