#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <thread>

#define NUMOFSEEDS 30
#define NUMOFTHREADS 3600 

using namespace std;
mutex mtx;

void thread_function (string cmd) {
	mtx.lock ();
	system (cmd.c_str ());
	// cout << cmd << endl;
	mtx.unlock ();
}

int main () {
	vector<int> Core, BW;
	double Number;
	static unsigned int NumofRequests = 100000;
	static string Exec = "~/Desktop/Simulator/SimWDM-SDM/Sim_FixedFlex"; 
	static string Topo = "~/Desktop/Simulator/SimWDM-SDM/Topology/NSF14.txt";
	thread ThreadPointer[3600];
	unsigned int cnt = 0;

	ofstream plot ("Plot.txt");

	Core.push_back (1);
	Core.push_back (4);
	Core.push_back (7);

	BW.push_back (10);
	// BW.push_back (40);
	// BW.push_back (100);
	

	cout << "Input a number for seed and press enter: " << endl;
	cin >> Number; 
	srand (Number);


	for (unsigned int k = 0; k < Core.size (); k++) {
		for (unsigned int bw = 0; bw < BW.size (); bw++) {
			for (unsigned int seed = 0; seed < NUMOFSEEDS; seed++) {
				for (unsigned int l = 100; l < 491; l = l + 10) {
					string Cmd = Exec + ' ' + Topo + ' ' + to_string (NumofRequests) + ' ' + to_string (BW[bw]) + ' ' + to_string (Core[k]) + ' ' + to_string (l * Core[k]) + " 1 " + to_string ((double) rand () / 65535);
					ThreadPointer[cnt] = thread (thread_function, Cmd);
					cnt++;
				}
			} 
		}
	}

	for (int i = 0; i < NUMOFTHREADS; i++) {
		ThreadPointer[i].join ();
	}

	plot.close ();

	return 1;
}




