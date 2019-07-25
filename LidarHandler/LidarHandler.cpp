#include "VLP16Capture.h"
#include "LidarHandler.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	VelodyneVLP16PCAP capture;
	capture.open_live();

	if (!capture.isOpen()) {
		std::cerr << "Can't open the file." << std::endl;
		return -1;
	}

	int frame_counter = 0;
	int cycle_counter = 0;

	while (capture.isRun()) {
		// Capture One Rotation Laser Data (one frame)
		std::vector<DataPoint> dataPoints;
		capture.retrieve(dataPoints);
		if (dataPoints.empty()) {
			continue;
		}

		if (frame_counter == 2) { break; }

		ColorPoints(dataPoints);
		WriteToCSV(dataPoints, frame_counter);
		//DisplayDistance(dataPoints,cycle_counter);
	}

	return 0;
}

void ColorPoints(std::vector<DataPoint>& frame)
{
	//ColorInOrder(frame,4000,1.5);
	//ColorByLaser(frame);
	//ColorByDistance(frame);
	//ColorByObjects(frame);
}

void ColorByObjects(std::vector<DataPoint>& frame)
{
	for (DataPoint& laser : frame)
	{
		
	}
}

void ColorByDistance(std::vector<DataPoint>& frame)
{
	std::sort(frame.begin(), frame.end(),compareLaserDistance);
	ColorInOrder(frame, 4000, 1.5);
}

// Comparison function for std::sort, by distance
bool compareLaserDistance(const DataPoint& a, const DataPoint& b)
{
	return a.distance < b.distance;
}


void ColorByLaser(std::vector<DataPoint>& frame)
{
	for (DataPoint& laser : frame)
	{
		laser.color = laser.id;
	}
}

void ColorInOrder(std::vector<DataPoint>& frame, int color_switch_threshold, float color_switch_factor)
{
	int color_code = 0;
	int point_num = 1;

	for (DataPoint& laser : frame)
	{
		if (point_num > color_switch_threshold)
		{
			color_code++;
			color_switch_threshold *= color_switch_factor;
		}

		laser.color = color_code;
		if (laser.distance != 0)
		{ point_num++; }
	}
}

void WriteToCSV(const std::vector<DataPoint>& frame, int& frame_counter)
{
	// Create filestream for CSV output
	ofstream outputFile;
	// create a name for the file output
	string filename = "frame" + to_string(frame_counter) + ".csv";
	// create and open the .csv file
	outputFile.open(filename);
	outputFile << "x,y,z,dist,color" << endl;

	// Iterate over every point in the frame
	for (const DataPoint& laser : frame)
	{
		outputFile << to_string(laser.coordinates.x) << ","
			<< to_string(laser.coordinates.y) << ","
			<< to_string(laser.coordinates.z) << ","
			<< to_string(laser.distance) << ","
			<< to_string(laser.color) << endl;
	}

	outputFile.close();
	frame_counter++;
}

void DisplayDistance(const std::vector<DataPoint>& frame, int& cycle_counter)
{
	for (const DataPoint& laser : frame)
	{
		if (laser.azimuth < 1 && laser.azimuth > 0 && (laser.id == 0 || laser.id == 7 || laser.id == 15))
		{
			if (laser.id == 0) { cycle_counter++; }

			//if (true)
			if (cycle_counter > 100)
			{
				cout << " distance: " << laser.distance << " (" << static_cast<int>(laser.id) << ")";
				if (laser.id == 15)
				{
					cycle_counter = 0;
					cout << endl;
				}
			}
		}
	}
}
	//TODO

	//object detection on frames
	//	similar vert+azimuth but distance over threshold
	//		how large of an environment
	//		structure of closest points
