#ifndef _RESOURCEASSIGNMENT_ISOLATEDCORE_H
#define _RESOURCEASSIGNMENT_ISOLATEDCORE_H

#include "Event.h"
#include "EventQueue.h"
#include "RoutingTable.h"
#include <list>


class ResourceAssignment {
	public:
		ResourceAssignment (Network * net, EventQueue * eq) {network = net; eventQueue = eq;}
		~ResourceAssignment () {}

		void check_availability_source (unsigned int predecessor, unsigned int successor); 
		void check_availability_link (vector<int> * circuitRoute);
		void handle_requests (CircuitRequest * circuitRequest);
		void handle_releases (CircuitRelease * circuitRelease);
		
		vector< list<int> > IsoAvailableWL;

	private:
		Network * network;
		EventQueue * eventQueue;
};


#endif


