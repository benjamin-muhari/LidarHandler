#include "VLP16Capture.h"
#include "LidarHandler.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	cout << "Program Starting" << endl;

	VelodyneVLP16PCAP capture;

	Frame current_frame = Frame();

	float last_azimuth = 0;
	int cycle_counter = 0;
	int frame_counter = 0;

	capture.open_live();

	if (!capture.isOpen()) {
		std::cerr << "Can't open the file." << std::endl;
		return -1;
	}

	while (capture.isRun()) {
		// Capture One Rotation Laser Data
		std::vector<DataPoint> dataPoints;
		capture.retrieve(dataPoints);
		if (dataPoints.empty()) {
			continue;
		}

		ProcessFrame(dataPoints,cycle_counter,frame_counter);
	}

	return 0;
}

void ProcessFrame(std::vector<DataPoint> frame, int& cycle_counter, int& frame_counter)
{
	// Create filestream for CSV output
	ofstream outputFile;
	// create a name for the file output
	string filename = "exampleOutput" + to_string(frame_counter) + ".csv";
	// create and open the .csv file
	outputFile.open(filename);
	outputFile << "x,y,z" << endl;

	// Iterate over every point in the frame
	for (DataPoint& laser : frame)
	{
		ColorByDistance(laser);

		// Write frame to a CSV file
		// outputFile << to_string(laser.coordinates.x) << "," << to_string(laser.coordinates.y) << "," << to_string(laser.coordinates.z) << endl;

		DisplayDistance(laser.azimuth, laser.id, cycle_counter, laser.distance);
	}
	outputFile.close();
	frame_counter++;
}

void ColorByDistance(DataPoint& dataPoint)
{
	// Logic?

	dataPoint.color = 10;
}

void DisplayDistance(const double& azimuth, const unsigned char& id, int& cycle_counter, const unsigned short& distance)
{
	if (azimuth < 1 && azimuth > 0 && (id == 0 || id == 7 || id == 15))
	{
		if (id == 0) { cycle_counter++; }

		if (true)
		//if (cycle_counter > 100)
		{
			cout << " distance: " << distance << " (" << static_cast<int>(id) << ")";
			if (id == 15)
			{
				cycle_counter = 0;
				cout << endl;
			}
		}
	}
}