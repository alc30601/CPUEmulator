# makefile

SOURCE = 	main.cpp

exe : $(SOURCE)
	g++ -std=c++17  -I. -I./TEST $(SOURCE) -o cpuEmulator

clean :
	rm cpuEmulator

