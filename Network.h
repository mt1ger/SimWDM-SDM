#ifndef _NETWORK_H
#define _NETWORK_H

#include <vector>
// #include "TrafficGenerator.h"
// #include "RoutingTable.h"
#include "EventQueue.h"

#define MAX_BW_REQUEST 200 // Gb/s
#define FULL_BW 1600 // Gb/s
#define PERCENTAGE_GUARDBAND 0.1 // 10%

using namespace std;

class RoutingTable;

class Network {
	public:
		/*** Functions ***/  
		void init ();
		void simulation ();


		/*** VARIABLES ***/ 
		// Inputted Variables   
		char FileName[500];
		double Lambda, Mu;
		long long NumofRequests;
		unsigned int BWofWavelength;
		unsigned int NumofCores;

		// Topology Variables
		unsigned int NumofNodes; 
		vector< vector<double> > NodesWeight;
		vector< vector< vector<int> > > routingTable;

		// Timer and counters 
		long long RequestCounter;
		long long NumofTransponders;
		long long NumofAllocatedRequests;
		long long NumofDoneRequests;
		long long NumofFailedRequests;
		double SystemClock;

		// Resource Variables
		unsigned int NumofWavelengths;
		vector< vector< vector< vector<bool> > > > Wavelengths;

		// Debuggin Variables
		vector<int> BlockedRequests;
		
	private:
};

#endif
