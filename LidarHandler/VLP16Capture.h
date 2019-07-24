#include <vector>
#include <string>
#include <pcap.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <functional>
#include <iomanip>
#include <sstream>

#include "DataPoint.h"


class VelodyneVLP16PCAP
{
protected:

	//These data by definition
	static const int MAX_NUM_LASERS = 16;
	//https://velodynelidar.com/docs/notes/63-9276%20Rev%20A%20VLP-16%20Application%20Note%20-%20Packet%20Structure%20&%20Timing%20Definition_Locked.pdf
	//12/24 page
	const std::vector<double> vertAngle = { -15.0, 1.0, -13.0, 3.0, -11.0, 5.0, -9.0, 7.0, -7.0, 9.0, -5.0, 11.0, -3.0, 13.0, -1.0, 15.0 };
	const double time_between_firings = 2.304;
	const double time_half_idle = 18.432;
	const double time_total_cycle = 55.296 * 2;

	static const int LASER_PER_FIRING = 32;
	static const int FIRING_PER_PKT = 12;

	pcap_t* pcap = nullptr;
	std::string filename = "";

	std::thread* thread = nullptr;
	std::atomic_bool run = { false };
	std::mutex mutex;
	std::queue<std::vector<DataPoint>> queue;

#pragma pack(push, 1)
	typedef struct LaserReturn //three bytes
	{
		uint16_t distance; // 2 bytes of distance
		uint8_t intensity; // 1 byte of calibrated reflection
	}LaserReturn;
#pragma pack(pop)

#pragma pack(push, 1)
	struct FiringData
	{
		uint16_t blockIdentifier; // 2 byte flag
		uint16_t rotationalPosition; // 2 bytes azimuth
		LaserReturn laserReturns[32]; // 32 data points 
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct Factory
	{
		uint8_t mode; //Return mode
		uint8_t model; //Product ID

	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct DataPacket //1206 bytes
	{
		FiringData firingData[12]; //12 peaces * 100 bytes [2 + 2 + 32 * (2 + 1)] = 1200
		uint32_t timestamp; // 4 bytes timeStamp
		Factory factory; // 2 bytes factory
	};
#pragma pack(pop)

public:

	VelodyneVLP16PCAP();

	~VelodyneVLP16PCAP();

	// Open live stream
	const bool open_live();

	// Open the file
	const bool open(const std::string& filename); 

	// Check Open
	const bool isOpen();

	// Check Run
	const bool isRun();

	// Close Capture
	void close();

	// Retrieve Captured Data
	void retrieve(std::vector<DataPoint>& lasers);

private:
	void readPCAP();

	void parseDataPacket(const DataPacket* packet, std::vector<DataPoint>& lasers, double& last_azimuth);


};