#ifndef _NETWORK_H
#define _NETWORK_H

#include <vector>
// #include "TrafficGenerator.h"
// #include "RoutingTable.h"
#include "EventQueue.h"

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
		unsigned int NumofWavelengths;
		unsigned int NumofCores;

		// Topology Variables
		unsigned int NumofNodes; 
		vector< vector<double> > NodesWeight;
		vector< vector< vector<int> > > routingTable;

		// Timer and counters 
		long long RequestCounter;
		long long NumofAllocatedRequests;
		long long NumofDoneRequests;
		long long NumofFailedRequests;
		double SystemClock;

		vector< vector< vector< vector<bool> > > > Wavelengths;


		vector<int> BlockedRequests;

		
	private:
};

#endif
