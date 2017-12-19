/**************************************************
 * First-Fit  
 **************************************************/
// #define DEBUG_print_resource_state_on_the_path
// #define DEBUG_in_check_availability_link
// #define DEBUG_print_IsoAvailableWL
// #define DEBUG_collect_EventID_of_blocked_requests //Need to collaberate with DEBUG_print_EventID_of_blocked_requests


// #define PRINT_allocation_block_release 


#include <iostream>
#include <string>
#include <cmath>
#include "ResourceAssignment_IsolatedCore.h"


using namespace std;

void ResourceAssignment::check_availability_source (unsigned int predecessor, unsigned int successor) {
	list<int> HAvailableWL;

	IsoAvailableWL.clear ();
	for (int i = 0; i < network->NumofCores; i++) {
		HAvailableWL.push_back (i);
		for (int j = 0; j < network->NumofWavelengths; j++) {
			if (network->Wavelengths[predecessor][successor][i][j] == false) {
				HAvailableWL.push_back (j);
			}
		}
		IsoAvailableWL.push_back (HAvailableWL);
		HAvailableWL.clear ();
	}
}

void ResourceAssignment::check_availability_link (vector<int> * circuitRoute) {
	vector<int> HWLsforAllocation;

	for (int i = 2; i < circuitRoute->size (); i++) {
		for (int j = 0; j < IsoAvailableWL.size (); j++) {
			list<int>::iterator k = IsoAvailableWL[j].begin ();
			for (k++ ; k != IsoAvailableWL[j].end (); k++) {
				if (network->Wavelengths[circuitRoute->at (i - 1)][circuitRoute->at (i)][j][*k] == true) {
					k = IsoAvailableWL[j].erase (k);
					k--;
				}
			}
		}
	}
}

void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);

	vector<int> CircuitRoute;
	unsigned int wavelength;
	bool AvailableFlag = false;
	vector<int> Wavelengths;
	unsigned int OccupiedWL;
	vector< vector<int> > WLsforAllocation;
	vector<int> HWLsforAllocation;
	unsigned int WLCounter = 0;

	OccupiedWL = ceil ((double) circuitRequest->Bandwidth / network->BWofWavelength );
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
	check_availability_link (&CircuitRoute);

	#ifdef DEBUG_print_IsoAvailableWL
	cout << "Start to print AvailableWL" << endl;
	for (int j = 0; j < IsoAvailableWL.size (); j++) {
		cout << "On Core " << j << endl;
		list<int>::iterator it = IsoAvailableWL[j].begin ();
		for (; it != IsoAvailableWL[j].end (); it ++) {
			cout << *it << ' ';
		}
		cout << endl;
	}
	cout << endl;
	#endif

	for (int i = 0; i < network->NumofCores; i++) {
		if (OccupiedWL > (IsoAvailableWL[i].size () - 1)) {
		}
		else {
			AvailableFlag = true;
			list<int>::iterator iter = IsoAvailableWL[i].begin ();
			iter++;

			for (int j = 0; j < OccupiedWL; j++) {
				HWLsforAllocation.push_back (i);
				HWLsforAllocation.push_back (*iter);
				for (int k = 1; k < CircuitRoute.size (); k++) {
					network->Wavelengths[CircuitRoute.at (k - 1)][CircuitRoute.at (k)][HWLsforAllocation[0]][HWLsforAllocation[1]] = true;
				}
				WLsforAllocation.push_back (HWLsforAllocation);
				HWLsforAllocation.clear ();
				iter++;
			}
			break;
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
		cout << "Allocated wavelengths: (" << WLsforAllocation.size () << ')' << endl;
		for (int i = 0; i < WLsforAllocation.size (); i++) {
			cout << WLsforAllocation[i][0] << ' ' << WLsforAllocation[i][1] << "    ";
		}
		cout << endl;
		cout << "# of Transponders: " << OccupiedWL << endl;
		cout << "------------------------------------------------------------" << endl << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, WLsforAllocation, circuitRequest->StartTime + circuitRequest->Duration);
		eventQueue->queue_insert (circuitRelease);

		network->NumofAllocatedRequests++;
		network->NumofTransponders = network->NumofTransponders + OccupiedWL;
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
		cout << "On link " << circuitRelease->CircuitRoute[i-1] << " to " << circuitRelease->CircuitRoute[i] << endl;
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
