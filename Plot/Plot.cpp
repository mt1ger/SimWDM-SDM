#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

using namespace std;

int main () {
	vector<int> Core;
	double Number;
	static unsigned int NumofRequests = 100000;
	static string Exec = "~/Desktop/Simulator/SimWDM-SDM/Simulator"; 
	static string Topo = "~/Desktop/Simulator/SimWDM-SDM/Topology/NSF14.txt";
	fstream plot ("Plot.txt", fstream::out | fstream::app);

	Core.push_back (1);
	Core.push_back (2);
	Core.push_back (4);

	cout << "Input a number for seed and press enter: " << endl;
	cin >> Number; 
	srand (Number);


	for (unsigned int k = 0; k < Core.size (); k++) {
		for (unsigned int i = 0; i < 10; i++) {
			for (unsigned int l = 10; l < 101; l = l + 10) {
				string Cmd = Exec + ' ' + Topo + ' ' + to_string (NumofRequests) + " 7 " + to_string (Core[k]) + ' ' + to_string (l) + " 1 " + to_string ((double) rand () / 65535);
				// cout << Cmd << endl;
				system (Cmd.c_str ());
			}
		} 
	}

	return 1;
}




