/**************************************************
 * First-Fit  
 **************************************************/
#define DEBUG_print_resource_state_on_the_path
#define DEBUG_in_check_availability_link
#define DEBUG_print_SourceAvailableWL
#define DEBUG_collect_EventID_of_blocked_requests //Need to collaberate with DEBUG_print_EventID_of_blocked_requests


#define PRINT_allocation_block_release 


#include <iostream>
#include <string>
#include <cmath>
#include "ResourceAssignment_FullyFlex.h"


using namespace std;

void ResourceAssignment::check_availability_source (unsigned int predecessor, unsigned int successor) {
	vector<int> HSourceAvailableWL;
	SourceAvailableWL.clear ();

	for (int i = 0; i < network->NumofCores; i++) {
		for (int j = 0; j < network->NumofWavelengths; j++) {
			if (network->Wavelengths[predecessor][successor][i][j] == false) {
				HSourceAvailableWL.push_back (i); 
				HSourceAvailableWL.push_back (j);
				SourceAvailableWL.push_back (HSourceAvailableWL);
				HSourceAvailableWL.clear ();
			}
		}
	}
}

void ResourceAssignment::check_availability_link (vector<int> * circuitRoute, vector< vector<int> > * WLsforAllocation, unsigned int * WLCounter, int core, int wl) {
	bool AvailableFlag = true;
	vector<int> HWLsforAllocation;

	for (int i = 2; i < circuitRoute->size (); i++) {
		if (network->Wavelengths[circuitRoute->at (i - 1)][circuitRoute->at (i)][core][wl] == true) {
			AvailableFlag = false;
			break;
		}
	}
	if (AvailableFlag == true) {
		HWLsforAllocation.push_back (core);
		HWLsforAllocation.push_back (wl);
		WLsforAllocation->push_back (HWLsforAllocation);
		(* WLCounter)++;
	}
}

void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);

	vector<int> CircuitRoute;
	bool AvailableFlag = true;
	vector<int> Wavelengths;
	unsigned int OccupiedWL;
	vector< vector<int> > WLsforAllocation;
	unsigned int WLCounter = 0;
	unsigned int CoreCnter = 0;

	OccupiedWL = circuitRequest->OccupiedWavelengths; 
	CircuitRoute = routingTable.get_shortest_path (circuitRequest->Src, circuitRequest->Dest);

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "Start to print resources on the path before allocation" << endl;
	for (int i = 1; i < CircuitRoute.size (); i++) {
		cout << "On link " << CircuitRoute[i - 1] << " to " << CircuitRoute[i] << endl;	
		for (int j = 0; j < network->NumofCores; j++) {
			cout << "On core " << j << endl;
			for (int k = 0; k < network->NumofWavelengths; k++) {
				cout << network->Wavelengths[CircuitRoute[i - 1]][CircuitRoute[i]][j][k] << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}
	#endif

	// Calculate possible available WLs on each core on the link between source and its successor
	check_availability_source (CircuitRoute[0], CircuitRoute[1]);

	#ifdef DEBUG_print_SourceAvailableWL
	cout << "Start to print SourceAvailableWL" << endl;
	for (int j = 0; j < SourceAvailableWL.size (); j++) {
		cout << SourceAvailableWL[j][0] << ' ' << SourceAvailableWL[j][1] << "    ";
	}
	cout << endl;
	#endif

	if (SourceAvailableWL.empty () || SourceAvailableWL.size () < OccupiedWL) AvailableFlag = false; 
	else {
		for (int i = 0; i < SourceAvailableWL.size (); i++) {
			AvailableFlag = true;
			check_availability_link (&CircuitRoute, &WLsforAllocation, &WLCounter, SourceAvailableWL[i][0], SourceAvailableWL[i][1]);
			if (WLCounter == OccupiedWL) {
				for (int j = 1; j < CircuitRoute.size (); j++) {
					for (int k = 0; k < WLsforAllocation.size (); k++) {
						network->Wavelengths[CircuitRoute[j - 1]][CircuitRoute[j]][WLsforAllocation[k][0]][WLsforAllocation[k][1]] = true;
					}
				}
				break;
			}
			else {
				AvailableFlag = false;
			}
		}
	}

	if (AvailableFlag == false) {

		#ifdef DEBUG_collect_EventID_of_blocked_requests
		network->BlockedRequests.push_back (circuitRequest->EventID);
		#endif

		#ifdef PRINT_allocation_block_release
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
		cout << "Request " << circuitRequest->EventID << " is blocked" << endl;
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl << endl;
		#endif

		network->NumofFailedRequests++;
		network->NumofDoneRequests++;
	}
	else if (AvailableFlag == true) {

		#ifdef PRINT_allocation_block_release
		cout << "------------------------------------------------------------" << endl;
		cout << "Request ID: " << circuitRequest->EventID << "\tStart: " << circuitRequest->EventTime << "\tEnd: " << circuitRequest->StartTime + circuitRequest->Duration << endl;
		cout << "Source: " << circuitRequest->Src << "  Destination: " << circuitRequest->Dest << endl;
		cout << "Path: ";
		for(unsigned int t = 0; t < CircuitRoute.size()-1; t++)
			cout << CircuitRoute.at(t) << " --> ";
		cout << CircuitRoute.at (CircuitRoute.size() - 1) << endl;
		cout << "Allocated wavelengths: (" << WLsforAllocation.size () << ')' << endl;
		int temp = WLsforAllocation[0][0];
		CoreCnter = 1;
		for (int i = 0; i < WLsforAllocation.size (); i++) {
			cout << WLsforAllocation[i][0] << ' ' << WLsforAllocation[i][1] << "    ";
			if (WLsforAllocation[i][0] != temp) {
				CoreCnter++;
				temp = WLsforAllocation[i][0];
			}
		}
		cout << endl;

		cout << "# of Cores: " << CoreCnter << endl;
		cout << "# of Transponders: " << OccupiedWL << endl;
		cout << "Inner Fragmentation: " << (1 - ((double) circuitRequest->DataSize / (circuitRequest->OccupiedWavelengths * network->BWofWavelength))) << endl; 
		cout << "------------------------------------------------------------" << endl << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, WLsforAllocation, circuitRequest->StartTime + circuitRequest->Duration, OccupiedWL);
		eventQueue->queue_insert (circuitRelease);

		network->NumofAllocatedRequests++;
		network->TotalCoresUsed += CoreCnter;
		network->TotalHoldingTime += circuitRequest->Duration;
		network->TotalTranspondersUsed += OccupiedWL;
		network->NumofTransponders = network->NumofTransponders + OccupiedWL;
		network->TotalDataSize += circuitRequest->DataSize;
		network->TotalWLsOccupied += circuitRequest->OccupiedWavelengths;
	}

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "Start to print resources on the path after allocation" << endl;
	for (int i = 1; i < CircuitRoute.size (); i++) {
		cout << "On link " << CircuitRoute[i - 1] << " to " << CircuitRoute[i] << endl;
		for (int j = 0; j < network->NumofCores; j++) {
			cout << "On core " << j << endl;
			for (int k = 0; k < network->NumofWavelengths; k++) {
				cout << network->Wavelengths[CircuitRoute[i - 1]][CircuitRoute[i]][j][k] << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}
	#endif
}

void ResourceAssignment::handle_releases (CircuitRelease * circuitRelease) {

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < circuitRelease->CircuitRoute.size (); i++) {
		cout << circuitRelease->CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "Start to print resources on the path before release" << endl;
	for (int i = 1; i < circuitRelease->CircuitRoute.size (); i++) {
		cout << "On link " << circuitRelease->CircuitRoute[i - 1] << " to " << circuitRelease->CircuitRoute[i] << endl;
		for (int j = 0; j < network->NumofCores; j++) {
			for (int k = 0; k < network->NumofWavelengths; k++) {
				cout << network->Wavelengths[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][j][k] << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}
	#endif

	for (int i = 1; i < circuitRelease->CircuitRoute.size (); i++) {
		for (int j = 0; j < circuitRelease->WLAllocList.size (); j++) {
			network->Wavelengths[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][circuitRelease->WLAllocList[j][0]][circuitRelease->WLAllocList[j][1]] = false;	
		}
	}
	
	network->NumofTransponders = network->NumofTransponders - circuitRelease->WLAllocList.size ();
	network->NumofDoneRequests++;
	#ifdef PRINT_allocation_block_release
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "Release Event: " << circuitRelease->EventID << "\tTime: " << circuitRelease->EventTime << endl;
	cout << "List of released WLs: (" << circuitRelease->WLAllocList.size () << ')' << endl;
	for (int i = 0; i < circuitRelease->WLAllocList.size (); i++) {
		cout << circuitRelease->WLAllocList[i][0] << ' ' << circuitRelease->WLAllocList[i][1] << "    ";
	}
	cout << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	#endif

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < circuitRelease->CircuitRoute.size (); i++) {
		cout << circuitRelease->CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "Start to print resources on the path after release" << endl;
	for (int i = 1; i < circuitRelease->CircuitRoute.size (); i++) {
		cout << "On link " << circuitRelease->CircuitRoute[i - 1] << " to " << circuitRelease->CircuitRoute[i] << endl;
		for (int j = 0; j < network->NumofCores; j++) {
			for (int k = 0; k < network->NumofWavelengths; k++) {
				cout << network->Wavelengths[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][j][k] << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}
	#endif
}

