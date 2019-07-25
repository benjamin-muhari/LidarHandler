// Display distance values of some lasers for testing
void DisplayDistance(const std::vector<DataPoint>& frame, int& cycle_counter);

// Writes contents of one frame to a CSV file
void WriteToCSV(const std::vector<DataPoint>& frame, int& frame_counter);

// Color points
void ColorPoints(std::vector<DataPoint>& frame);

void ColorByDistance(std::vector<DataPoint>& frame);

void ColorByLaser(std::vector<DataPoint>& frame);

// Colors points as they come in order (default order is azimuth if frame is not sorted)
void ColorInOrder(std::vector<DataPoint>& frame, int color_switch_threshold, float color_switch_factor);

// Comparison function for std::sort, by distance
bool compareLaserDistance(const DataPoint& a, const DataPoint& b);
