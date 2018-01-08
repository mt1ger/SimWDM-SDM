**************************************************
* Simulator of WDM-SDM optical network
* Author: Yue Wang
* Version: 1.1
* Bandwidth of Spectrum: 1600Gb/s 
* Bandwidth of Wavelength: 10Gb/s, 40Gb/s, 100Gb/s
* Number of Wavelengths: 160, 40, 16
**************************************************


Version 1.0:
1. Time slot did not considered.
2. First-fit applied.
3. Core continuity constraint obeyed.
4. Wavelength continuity constraint obeyed.
5. Each core is processed isolatedly.
6. Each request occupies one wavelength.
7. Unicast applied.




Version 1.1:
1. Time slot did not considered.
2. First-fit applied.
3. Core continuity constraint obeyed.
4. Wavelength continuity constraint obeyed.

/* Modified since Version 1.0 */
5. Now each request occupies bandwidth (Gb/s), the number of wavelength occupied needs to be computed.
6. Fixed-grid SDM allocation scheme applied.

6. Unicast applied.





Version 1.2:
1. Time slot did not considered.
2. First-fit applied.
3. Core continuity constraint obeyed.
4. Wavelength continuity constraint obeyed.
5. Each request occupies bandwidth (Gb/s), the number of wavelength occupied needs to be computed.

/* Modified since Version 1.1 */
6. Need to choose ResourceAssignment.cpp files to apply different allocation scheme: 
	ResourceAssignment_FixedGridSDM.cpp:
		fixed-grid SDM allocation scheme applied.
	ResourceAssignment_FullyFlexSDM.cpp:
		Fully Flexable SDM allocation scheme support.
7. Need to modify header used by Network.cpp.
8. Need to use different makefile to compile:
	Differnet "Event.cpp"s associated with each allocation scheme.
	Different makefiles associated with each allocation scheme.

9. Unicast applied.





Version 1.3;
1. Time slot did not considered.
2. First-fit applied.
3. Core continuity constraint obeyed.
4. Wavelength continuity constraint obeyed.
5. Each request occupies bandwidth (Gb/s), the number of wavelength occupied needs to be computed.
6. Need to choose ResourceAssignment.cpp files to apply different allocation scheme: 
	ResourceAssignment_FixedGridSDM.cpp:
		fixed-grid SDM allocation scheme applied.
	ResourceAssignment_FullyFlexSDM.cpp:
		Fully Flexable SDM allocation scheme support.
7. Need to modify header used by Network.cpp.
8. Need to use different makefile to compile:
	Differnet "Event.cpp"s associated with each allocation scheme.
	Different makefiles associated with each allocation scheme.
9. Unicast applied.

/* Modified From Version 1.2 */
10. Add Metrics to measure: Internal Fragmentation
							External Fragmentation
							Hybrid Fragmentation
							Cores Used per Request
							Transponders Used per Request


