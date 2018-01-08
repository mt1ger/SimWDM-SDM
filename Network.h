 #ifndef _NETWORK_H
#define _NETWORK_H

#include <vector>
// #include "TrafficGenerator.h"
// #include "RoutingTable.h"
#include "EventQueue.h"

#define MAX_DATASIZE_REQUEST 200 // Gb/s
// #define FULL_BW 230 // Gb/s
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


		// Metrics per Request
		double TotalHoldingTime;
		long long TotalTranspondersUsed;
		long long TotalCoresUsed;
		double AvgHoldingTime;
		double AvgTranspondersUsed;
		double AvgCoresUsed;


		// Metrics to Measure Fregmentation
		long long TotalWLsOccupied;
		long long TotalWLGsOccupied;
		long long TotalDataSize;

		double AvgInnFrag;
		double AvgExtFrag;
		double AvgHybridFrag;


		// Timer and counters 
		long long RequestCounter; // Generated Number of Requests
		long long NumofAllocatedRequests;
		long long NumofTransponders; // Number of TranspondersUsed at a Specific Time
		long long NumofDoneRequests; // Requests which are successfully allocated and released, or blocked will be considered as DoneRequests
		long long NumofFailedRequests; // Number of Requests that are blocked;
		long long MaxNumofTransponders; // For one time simulation, Maximum Number of Transponders Used Simultaneously.
		double SystemClock;


		// Resource Variables
		unsigned int NumofWavelengths;
		vector< vector< vector< vector<bool> > > > Wavelengths;

		// Debugging Variables
		vector<int> BlockedRequests;
		
	private:
};

#endif
