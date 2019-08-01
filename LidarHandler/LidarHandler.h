//#include <vector>
#ifndef DATAPOINT_H
#define DATAPOINT_H
#include "DataPoint.h"
#endif

// Exported func. to python, just to run the original cpp program
void CallMain();

// Display distance values of some lasers for testing
void DisplayDistance(const std::vector<DataPoint>& frame, int& cycle_counter);

// Writes contents of one frame to a CSV file
void WriteToCSV(const std::vector<DataPoint>& frame, int& frame_counter);

// COLORING ----------------------------------------------------------------------------------------------

// Color points
void ColorPoints(std::vector<DataPoint>& frame);

// Colors points as they come in order (default order is azimuth if frame is not sorted)
void ColorInOrder(std::vector<DataPoint>& frame, int color_switch_threshold, float color_switch_factor);

void ColorBy4Diff(std::vector<DataPoint>& frame);

void ColorByDistance(std::vector<DataPoint>& frame);

void ColorByLaser(std::vector<DataPoint>& frame);

void ColorByObjects(std::vector<DataPoint>& frame);

// UTIL --------------------------------------------------------------------------------------------------
// Comparison function for std::sort, by distance
// Hide DataPoint(laser) when condition is true; for testing purposes
bool HideLaser(DataPoint& laser, const bool condition);
