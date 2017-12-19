/**************************************************
 * First-Fit  
 **************************************************/
// #define DEBUG_print_resource_state_on_the_path
// #define DEBUG_in_check_availability_link
// #define DEBUG_print_SourceAvailableWL
// #define DEBUG_collect_EventID_of_blocked_requests //Need to collaberate with DEBUG_print_EventID_of_blocked_requests


// #define PRINT_allocation_block_release 


#include <iostream>
#include <string>
#include <cmath>
#include "ResourceAssignment_FixedFlex.h"


using namespace std;

void ResourceAssignment::check_availability_source (unsigned int predecessor, unsigned int successor) {
	SourceAvailableWL.clear ();

	for (int j = 0; j < network->NumofWavelengths; j++) {
		if (network->Wavelengths[predecessor][successor][0][j] == false) {
			SourceAvailableWL.push_back (j); 
		}
	}
}

void ResourceAssignment::check_availability_link (vector<int> * circuitRoute, vector< vector<int> > * WLsforAllocation, unsigned int * WLCounter, int WL) {
	bool AvailableFlag = true;
	vector<int> HWLsforAllocation;

	if (circuitRoute->size () > 2) {
		for (int i = 2; i < circuitRoute->size (); i++) {
			if (network->Wavelengths[circuitRoute->at (i - 1)][circuitRoute->at (i)][0][WL] == true) {
				AvailableFlag = false;
				break;
			}
		}
		if (AvailableFlag == true) {
			HWLsforAllocation.push_back (0);
			HWLsforAllocation.push_back (WL);
			WLsforAllocation->push_back (HWLsforAllocation);
			(* WLCounter)++;
		}
	}
	else {
		HWLsforAllocation.push_back (0);
		HWLsforAllocation.push_back (WL);
		WLsforAllocation->push_back (HWLsforAllocation);
		(* WLCounter)++;
	}
}

void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);

	vector<int> CircuitRoute;
	unsigned int wavelength;
	bool AvailableFlag = true;
	vector<int> Wavelengths;
	unsigned int OccupiedWL;
	vector< vector<int> > WLsforAllocation;
	unsigned int WLCounter = 0;

	OccupiedWL = ceil ((double) circuitRequest->Bandwidth / network->BWofWavelength / network->NumofCores);
	CircuitRoute = routingTable.get_shortest_path (circuitRequest->Src, circuitRequest->Dest);

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "Start to print resources on the path before allocation" << endl;
	for (int i = 1; i < CircuitRoute.size (); i++) {
		cout << "On link " << CircuitRoute[i - 1] << " to " << CircuitRoute[i] << endl;
		for (int k = 0; k < network->NumofWavelengths; k++) {
			cout << network->Wavelengths[CircuitRoute[i - 1]][CircuitRoute[i]][0][k] << ' ';
		}
		cout << endl;
	}
	#endif

	// Calculate possible available WLs on each core on the link between source and its successor
	check_availability_source (CircuitRoute[0], CircuitRoute[1]);

	#ifdef DEBUG_print_SourceAvailableWL
	cout << "Start to print SourceAvailableWL" << endl;
	for (int j = 0; j < SourceAvailableWL.size (); j++) {
		cout << SourceAvailableWL[j] << ' ';
	}
	cout << endl;
	#endif

	if (SourceAvailableWL.empty () || SourceAvailableWL.size () < OccupiedWL) AvailableFlag = false; 
	else {
		for (int i = 0; i < SourceAvailableWL.size (); i++) {
			AvailableFlag = true;
			check_availability_link (&CircuitRoute, &WLsforAllocation, &WLCounter, SourceAvailableWL[i]);
			if (WLCounter == OccupiedWL) {
				for (int j = 1; j < CircuitRoute.size (); j++) {
					for (int k = 0; k < WLsforAllocation.size (); k++) {
						network->Wavelengths[CircuitRoute[j - 1]][CircuitRoute[j]][0][WLsforAllocation[k][1]] = true;
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
		network->NumofDoneRequests++;

		#ifdef DEBUG_collect_EventID_of_blocked_requests
		network->BlockedRequests.push_back (circuitRequest->EventID);
		#endif

		#ifdef PRINT_allocation_block_release
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
		cout << "Request " << circuitRequest->EventID << " is blocked" << endl;
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl << endl;
		#endif

		network->NumofFailedRequests++;
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
		cout << "Allocated WLs: " << endl;
		for (int i = 0; i < WLsforAllocation.size (); i++) {
			cout << WLsforAllocation[i][1] << ' ';
		}
		cout << endl;
		cout << "------------------------------------------------------------" << endl << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, WLsforAllocation, circuitRequest->StartTime + circuitRequest->Duration);
		eventQueue->queue_insert (circuitRelease);

		network->NumofAllocatedRequests++;
		network->NumofTransponders = network->NumofTransponders + (WLsforAllocation.size () * network->NumofCores);
	}

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "Start to print resources on the path after allocation" << endl;
	for (int i = 1; i < CircuitRoute.size (); i++) {
		cout << "On link " << CircuitRoute[i - 1] << " to " << CircuitRoute[i] << endl;
		for (int k = 0; k < network->NumofWavelengths; k++) {
			cout << network->Wavelengths[CircuitRoute[i - 1]][CircuitRoute[i]][0][k] << ' ';
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
		for (int k = 0; k < network->NumofWavelengths; k++) {
			cout << network->Wavelengths[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][0][k] << ' ';
		}
		cout << endl;
	}
	#endif

	for (int i = 1; i < circuitRelease->CircuitRoute.size (); i++) {
		for (int j = 0; j < circuitRelease->WLAllocList.size (); j++) {
			network->Wavelengths[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][0][circuitRelease->WLAllocList[j][1]] = false;	
		}
	}
	
	network->NumofDoneRequests++;
	network->NumofTransponders = network->NumofTransponders - circuitRelease->WLAllocList.size () * network->NumofCores;
	#ifdef PRINT_allocation_block_release
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "Release Event: " << circuitRelease->EventID << "\tTime: " << circuitRelease->EventTime << endl;
	cout << "List of released WLs: " << endl;
	for (int i = 0; i < circuitRelease->WLAllocList.size (); i++) {
		cout << circuitRelease->WLAllocList[i][1] << ' ';
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
		cout << "On link " << circuitRelease->CircuitRoute[i-1] << " to " << circuitRelease->CircuitRoute[i] << endl;
		for (int k = 0; k < network->NumofWavelengths; k++) {
			cout << network->Wavelengths[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][0][k] << ' ';
		}
		cout << endl;
	}
	#endif
	
}
