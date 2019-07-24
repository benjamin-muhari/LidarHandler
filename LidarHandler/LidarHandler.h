using Frame = std::vector<Coordinates>;

// Old: writes frames to CSV

// Display distance values of some lasers for testing
void DisplayDistance(const double& azimuth, const unsigned char& id, int& cycle_counter, const unsigned short& distance);

// Incomplete: will color a frame according to distance metrics
void ColorByDistance(DataPoint& dataPoint);

// Iterates over a frame
void ProcessFrame(std::vector<DataPoint> frame, int& cycle_counter, int& frame_counter);