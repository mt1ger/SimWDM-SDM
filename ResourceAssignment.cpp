/**************************************************
 * First-Fit  
 **************************************************/
// #define DEBUG_print_resource_state_on_the_path
// #define DEBUG_in_check_availability_link
// #define DEBUG_print_SourceAvailableWL
// #define DEBUG_collect_EventID_of_blocked_requests //Need to collaberate with DEBUG_print_EventID_of_blocked_requests


#include <iostream>
#include "ResourceAssignment.h"
#include <string>
 

void ResourceAssignment::check_availability_source (unsigned int predecessor, unsigned int successor, CircuitRequest * circuitRequest) {
	vector<int> HSourceAvailableWL;

	SourceAvailableWL.clear ();
	for (int i = 0; i < network->NumofCores; i++) {
		for (int j = 0; j < network->NumofWavelengths; j++) {
			if (network->Wavelengths[predecessor][successor][i][j] == false) {
				HSourceAvailableWL.push_back (i);
				HSourceAvailableWL.push_back (j);
			}
			if (!HSourceAvailableWL.empty ()) {
				SourceAvailableWL.push_back (HSourceAvailableWL);
			}
			HSourceAvailableWL.clear ();
		}
	}
}

void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);	

	vector<int> CircuitRoute;
	unsigned int core, wavelength;
	bool AvailableFlag = true;
	bool WAvailableFlag = true;

	CircuitRoute = routingTable.get_shortest_path (circuitRequest->Src, circuitRequest->Dest);

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "Start to print resources on the path before allocation" << endl;
	for (int i = 1; i < CircuitRoute.size (); i++) {
		cout << "On link " << CircuitRoute[i-1] << " to " << CircuitRoute[i] << endl;
		for (int j = 0; j < network->NumofCores; j++) {
			for (int k = 0; k < network->NumofWavelengths; k++) {
				cout << network->Wavelengths[CircuitRoute[i - 1]][CircuitRoute[i]][j][k] << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}
	#endif

	// Calculate possible available WLs on each core on the link between source and its successor
	check_availability_source (CircuitRoute[0], CircuitRoute[1], circuitRequest);

	#ifdef DEBUG_print_SourceAvailableWL
	cout << "Start to print SourceAvailableWL" << endl;
	for (int i = 0; i < SourceAvailableWL.size (); i++) {
		for (int j = 0; j < SourceAvailableWL[i].size (); j++) {
			cout << SourceAvailableWL[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;
	#endif
 
	// Loop for all the possible SpectralSlotSections on the link between source and its successor
	if (SourceAvailableWL.empty ()) AvailableFlag = false;
	else {
		//Loop for cores
		for (int i = 0; i < SourceAvailableWL.size (); i++) {
			AvailableFlag = true;
			for (int k = 2; k < CircuitRoute.size (); k++) {
				// For each link on the path, check if the selected WL is available on the selected core over the link
				if (network->Wavelengths[CircuitRoute[k - 1]][CircuitRoute[k]][SourceAvailableWL[i][0]][SourceAvailableWL[i][1]] == true) {
					AvailableFlag = false;
					break;
				}
			}
			if (AvailableFlag == true) {
				for (int n = 1; n < CircuitRoute.size (); n++) {
					network->Wavelengths[CircuitRoute[n - 1]][CircuitRoute[n]][SourceAvailableWL[i][0]][SourceAvailableWL[i][1]] = true;	
				}
				core = SourceAvailableWL[i][0];
				wavelength = SourceAvailableWL[i][1];
				break;
			}
		}
	}

	if (AvailableFlag == false) {
		network->NumofDoneRequests++;

		#ifdef DEBUG_collect_EventID_of_blocked_requests
		network->BlockedRequests.push_back (circuitRequest->EventID);
		#endif

		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
		cout << "Request " << circuitRequest->EventID << " is blocked" << endl;
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl << endl;
		network->NumofFailedRequests++;
	}
	else if (AvailableFlag == true) {
		cout << "------------------------------------------------------------" << endl;
		cout << "Request ID: " << circuitRequest->EventID << "\tStart: " << circuitRequest->EventTime << "\tEnd: " << circuitRequest->StartTime + circuitRequest->Duration << endl;
		cout << "Source: " << circuitRequest->Src << "  Destination: " << circuitRequest->Dest << endl;
		cout << "Path: ";
		for(unsigned int t = 0; t < CircuitRoute.size()-1; t++)
			cout << CircuitRoute.at(t) << " --> ";
		cout << CircuitRoute.at (CircuitRoute.size() - 1) << endl;
		cout << "Core: " << core << " Wavelength: " << wavelength << endl; 
		cout << "------------------------------------------------------------" << endl << endl;

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, core, wavelength, circuitRequest->StartTime + circuitRequest->Duration);
		eventQueue->queue_insert (circuitRelease);

		network->NumofAllocatedRequests++;
	}

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "Start to print resources on the path after allocation" << endl;
	for (int i = 1; i < CircuitRoute.size (); i++) {
		cout << "On link " << CircuitRoute[i-1] << " to " << CircuitRoute[i] << endl;
		for (int j = 0; j < network->NumofCores; j++) {
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

	for (int i = 1; i < circuitRelease->CircuitRoute.size (); i++) {
		network->Wavelengths[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][circuitRelease->Core][circuitRelease->Wavelength] = false;	
	}
	
	network->NumofDoneRequests++;
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "Release Event: " << circuitRelease->EventID << "\tTime: " << circuitRelease->EventTime << endl;
	cout << "Core: " << circuitRelease->Core << "  Wavelength: " << circuitRelease->Wavelength << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;

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

	
	

