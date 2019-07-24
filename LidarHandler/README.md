# SoftTech

## Documentation

### VLP-16 PCAP Structure
- Header (42 Bytes)
- 12 Fireing Data (1200 Bytes)
  - Block Idetifier (2 Bytes)
  - Rotation Position (2 Bytes)
  - 32 Channel (96 Bytes)
    - Distance (2 Bytes)
    - Intensity (1 Byte)
- Timestamp (4 Bytes)
- Factory (2 Bytes)

### VelodyneVLP16PCAP::readPCAP Function
Read the raw PCAP file and structure the bytes to VLP-16 PCAP Structure. 

### VelodyneVLP16PCAP::parseDataPacket Function
Converts the structured VLP-16 data into cloud point coordinates. 
The azimuth position is calculated based on the Rotation Position and the previous package rotation position. The azimuth cannot be rolled over 360°.  
The vertical position is constant for each channel.
Distance is given for each returned leaser value. 
From the two positions and the distance the x, y, z coordinates are easily calculatable. 

## Demo

### Simple Query
```C++
VelodyneVLP16PCAP capture;

capture.open("fileName");
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

	for (const DataPoint& laser : dataPoints) {
		const unsigned char id = laser.id; // Laser ID (VLP-16 : 0 - 15)
		const unsigned short distance = laser.distance; // Laser Distance ( centimeter )
		const unsigned char intensity = laser.intensity; // Laser Intensity
		const long long unixtime = laser.time; // Laser TimeStamp ( microseconds )
		const double azimuth = laser.azimuth; // Laser Azimuth ( degree )
		const double vertical = laser.vertical; // Laser Vertical ( degree )
		const float x = laser.coordinates.x; //X coordinate
		const float y = laser.coordinates.y; //Y coordinate
		const float z = laser.coordinates.z; //Z coordinate
	}
}
```
### Filter Distance
If an object is closer than a mininun distance then the laser will return with a 0 distance value
```C++
if(laser.distance == 0)
	// Take some action
```

### Filter Azimuth
```C++
if(laser.azimuth > MinimumDegree && laser.azimuth < MaximumDegree)
	// Take some action
```
