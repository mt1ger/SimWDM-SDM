#define DEBUG_print_new_built_Event

#include "TrafficGenerator.h"

#define MAX_OCCUPIED_SPECTRAL_SLOTS 30


using namespace std;


void TrafficGenerator::gen_unicast_sd (unsigned int * src, unsigned int * dest) {
	do {
		* src = uniform_rv (network->NumofNodes);
	} while (* src == network->NumofNodes);


	do {
		* dest = uniform_rv (network->NumofNodes);
	} while ((* dest == * src) || (* dest == network->NumofNodes));
	
}


void TrafficGenerator::gen_temporal_parameters (double * time, unsigned int LorM) {
	* time = exponential_rv (LorM);
}


void TrafficGenerator::gen_first_request () {
	unsigned int src, dest, occupiedSpectralSlots;
	double startTime = 0;
	double duration;
	CircuitRequest * request;

	gen_unicast_sd (&src, &dest);
	gen_temporal_parameters (&duration, network->Mu);
	
	request = new CircuitRequest (src, dest, startTime, duration, network->RequestCounter);
	
	network->RequestCounter++;
	#ifdef DEBUG_print_new_built_Event
	cout << "New Built event:" << endl;
	cout << request->Src << ' ' << request->Dest << ' ' << request->StartTime << ' ' << request->Duration << ' ' << endl;
	#endif

	eventQUeue->ev_Queue.push_back (request);
}


//releases' generation will be taken care of in "handle_requests" in "ResourceAssignment.cpp" 
void TrafficGenerator::gen_request (double systemTime) {
	unsigned int src, dest, occupiedSpectralSlots;
	double time, startTime, duration;
	CircuitRequest * request;

	gen_unicast_sd (&src, &dest);
	gen_temporal_parameters (&duration, network->Mu);
	gen_temporal_parameters (&time, network->Lambda);
	do {
		occupiedSpectralSlots = uniform_rv (MAX_OCCUPIED_SPECTRAL_SLOTS);
	} while (occupiedSpectralSlots == 0);
	startTime = systemTime + time;
	
	request = new CircuitRequest (src, dest, startTime, duration, network->RequestCounter);
	
	network->RequestCounter++;

	#ifdef DEBUG_print_new_built_Event
	cout << "\tNew Built event:" << endl;
	cout << '\t' << request->EventID << ' ' <<request->Src << ' ' << request->Dest << ' ' << request->StartTime << ' ' << request->Duration << ' ' << endl;
	#endif

	eventQUeue->queue_insert (request);
}
