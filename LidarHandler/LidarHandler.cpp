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

		// stop program after 2 frames (for writing to CSV)
		//if (frame_counter == 2) { break; }

		// testing
		cout << dataPoints.size() << endl;
		continue;


		ColorPoints(dataPoints);
		WriteToCSV(dataPoints, frame_counter);
		//DisplayDistance(dataPoints,cycle_counter);
	}

	return 0;
}

void CallMain()
{
	main();
	cout << "End of C++ program" << endl;
}

void ColorPoints(std::vector<DataPoint>& frame)
{
	//ColorInOrder(frame,4000,1.5);
	ColorByLaser(frame);
	//ColorByDistance(frame);
	//ColorBy4Diff(frame);
	//ColorByObjects(frame);
}

void ColorBy4Diff(std::vector<DataPoint>& frame)
{
	int color_code = 0;
	double last_azimuth = 0;
	int last_id = 0;
	
	for (DataPoint& laser : frame)
	{
		if (laser.azimuth > last_azimuth)
		{
			color_code = 0;
		}
		else if (laser.id < last_id)
		{
			color_code++;
		}

		laser.color = color_code;
		last_azimuth = laser.azimuth;
		last_id = laser.id;

		//HideLaser(laser)
	}
}

bool HideLaser(DataPoint& laser, const bool condition)
{
	if (condition)
	{
		laser.coordinates.x = 0;
		laser.coordinates.y = 0;
		laser.coordinates.z = 0;
		laser.distance = 0;

		return true;
	}
	else
		return false;
}

void ColorByObjects(std::vector<DataPoint>& frame)
{
	float distance_threshold = 100;
	float last_distance[16];
	int color_code[16];
	
	for (int i = 0; i < 16; i++)
	{
		last_distance[i] = 0;
		color_code[i] = 0;
	}

	int step = 0;

	for (DataPoint& laser : frame)
	{	
		// Hide every laser but 7th for testing
		if (HideLaser(laser, laser.id != 7))
			continue;
		
		if (std::abs(laser.distance - last_distance[laser.id]) > distance_threshold)
		{
			color_code[laser.id] = (color_code[laser.id] + 1) % 2;
		}

		laser.color = color_code[laser.id];
		last_distance[laser.id] = laser.distance;
	}
}

void ColorByDistance(std::vector<DataPoint>& frame)
{
	std::sort(frame.begin(), frame.end(), DataPoint::compareDistance);
	ColorInOrder(frame, 4000, 1.5);
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
	outputFile << "x,y,z,dist,id,az,color" << endl;

	// Iterate over every point in the frame
	for (const DataPoint& laser : frame)
	{
		//if (true)
		if (laser.distance != 0)
		{
			outputFile << to_string(laser.coordinates.x) << ","
				<< to_string(laser.coordinates.y) << ","
				<< to_string(laser.coordinates.z) << ","
				<< to_string(laser.distance) << ","
				<< to_string(laser.id) << ","
				<< to_string(laser.azimuth) << ","
				<< to_string(laser.color) << endl;
		}
	}

	outputFile.close();
	frame_counter++;
}

void DisplayDistance(const std::vector<DataPoint>& frame, int& cycle_counter)
{
	for (const DataPoint& laser : frame)
	{

		// 1 point from every 10th frame
		//cout << laser.distance;
		//if (cycle_counter > 9)
		//{
		//	cout << laser.distance << endl;
		//	cycle_counter = 0;
		//	break;
		//}
		//cycle_counter++;
		//break;
		
		
		if (laser.azimuth < 1 && laser.azimuth > 0 && (laser.id == 0 || laser.id == 7 || laser.id == 15))
		{
			if (laser.id == 0) { cycle_counter++; }

			//if (true)
			if (cycle_counter > 50)
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
