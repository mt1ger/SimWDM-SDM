#ifndef _RESOURCEASSIGNMENT_FULLYFLEXSDM_H
#define _RESOURCEASSIGNMENT_FULLYFLEXSDM_H

#include "Event.h"
#include "EventQueue.h"
#include "RoutingTable.h"


class ResourceAssignment {
	public:
		ResourceAssignment (Network * net, EventQueue * eq) {network = net; eventQueue = eq;}
		~ResourceAssignment () {}

		void check_availability_source (unsigned int predecessor, unsigned int successor); 
		void check_availability_link (vector<int> * circuitRoute, vector< vector<int> > * WLsforAllocation, unsigned int * WLCounter, int core, int wl);
		void handle_requests (CircuitRequest * circuitRequest);
		void handle_releases (CircuitRelease * circuitRelease);
		
		vector< vector<int> > SourceAvailableWL;

	private:
		Network * network;
		EventQueue * eventQueue;
};


#endif


