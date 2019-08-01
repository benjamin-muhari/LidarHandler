#define _USE_MATH_DEFINES

#include <vector>
#include <pcap.h>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <functional>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "VLP16Capture.h"


using namespace std;

VelodyneVLP16PCAP::VelodyneVLP16PCAP()
{
}

VelodyneVLP16PCAP::~VelodyneVLP16PCAP()
{
	close();
}

pybind11::handle VelodyneVLP16PCAP::export_frame_pyobj()
{
	//PyObject* pyfloat = PyFloat_FromDouble(2.5);
	//pybind11::handle ret = pyfloat;
	//return ret;

	std::vector<DataPoint> dataPoints;
	while (this->isRun())
	{
		this->retrieve(dataPoints);

		if (dataPoints.empty() || dataPoints.size() == 0) { continue; }
		else { break; }
	}

	PyObject* frame_as_list = PyList_New(0);

	for (const DataPoint& laser : dataPoints)
	{
		PyObject* one_point = PyList_New(0);
		PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.x));
		PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.y));
		PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.z));

		PyList_Append(frame_as_list, one_point);
	}

	pybind11::handle ret = frame_as_list;
	return ret;
}

std::vector<std::vector<float>> VelodyneVLP16PCAP::export_frame()
{
	std::vector<DataPoint> dataPoints;
	while (this->isRun())
	{
		this->retrieve(dataPoints);

		if (dataPoints.empty() || dataPoints.size() == 0) { continue; }
		else { break; }
	}
	std::vector<std::vector<float>> frame_as_2D_vector = std::vector<std::vector<float>>();

	for (const DataPoint& laser : dataPoints)
	{
		std::vector<float> xyz = std::vector<float>();
		xyz.push_back(laser.coordinates.x);
		xyz.push_back(laser.coordinates.y);
		xyz.push_back(laser.coordinates.z);

		frame_as_2D_vector.push_back(xyz);
	}
	//std::cout << "called read_frame() vector size: " << frame_as_2D_vector.size() << std::endl;
	return frame_as_2D_vector;
}

const bool VelodyneVLP16PCAP::open_live(const int channelID)
{
	pcap_if_t* alldevs;
	pcap_if_t* d;
	int inum;
	int i = 0;
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	//List available channels if none is specified in channelID
	if (channelID == 0)
	{
		for (d = alldevs; d; d = d->next)
		{
			printf("%d. %s", ++i, d->name);
			if (d->description)
				printf(" (%s)\n", d->description);
			else
				printf(" (No description available)\n");
		}

		if (i == 0)
		{
			printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
			return -1;
		}

		printf("Enter the interface number (1-%d):", i);
		scanf_s("%d", &inum);
	}
	else
	{
		inum = channelID;
		for (d = alldevs; d; d = d->next)
			i++;
	}

	if (inum < 1 || inum > i)
	{
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

	/* Open the device */
	/* Open the adapter */
	if ((adhandle = pcap_open_live(d->name,	// name of the device
		65536,			// portion of the packet to capture. 
					    // 65536 grants that the whole packet will be captured on all the MACs.
		1,				// promiscuous mode (nonzero means promiscuous)
		1000,			// read timeout
		errbuf			// error buffer
	)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	// Convert PCAP_NETMASK_UNKNOWN to 0xffffffff
	struct bpf_program filter;
	std::ostringstream oss;
	cout << oss.str();
	if (pcap_compile(adhandle, &filter, oss.str().c_str(), 0, 0xffffffff) == -1) {
		throw std::runtime_error(pcap_geterr(adhandle));
		return false;
	}

	if (pcap_setfilter(adhandle, &filter) == -1) {
		throw std::runtime_error(pcap_geterr(adhandle));
		return false;
	}

	this->VelodyneVLP16PCAP::pcap = adhandle;

	// Start Capture Thread
	run = true;
	thread = new std::thread(std::bind(&VelodyneVLP16PCAP::readPCAP, this));

	return true;
}

const bool VelodyneVLP16PCAP::open(const std::string& filename)
{
	// Check Running
	if (isRun()) {
		close();
	}

	// Open PCAP File
	char error[PCAP_ERRBUF_SIZE];

	pcap_t* pcap = pcap_open_offline(filename.c_str(), error);
	if (!pcap) {
		throw std::runtime_error(error);
		return false;
	}

	// Convert PCAP_NETMASK_UNKNOWN to 0xffffffff
	struct bpf_program filter;
	std::ostringstream oss;
	if (pcap_compile(pcap, &filter, oss.str().c_str(), 0, 0xffffffff) == -1) {
		throw std::runtime_error(pcap_geterr(pcap));
		return false;
	}

	if (pcap_setfilter(pcap, &filter) == -1) {
		throw std::runtime_error(pcap_geterr(pcap));
		return false;
	}

	this->VelodyneVLP16PCAP::pcap = pcap;
	this->VelodyneVLP16PCAP::filename = filename;

	// Start Capture Thread
	run = true;
	thread = new std::thread(std::bind(&VelodyneVLP16PCAP::readPCAP, this));

	return true;
}

// Check Open
const bool VelodyneVLP16PCAP::isOpen()
{
	std::lock_guard<std::mutex> lock(mutex);
	return (pcap != nullptr);
}

// Check Run
const bool VelodyneVLP16PCAP::isRun()
{
	// Returns True when Thread is Running or Queue is Not Empty
	std::lock_guard<std::mutex> lock(mutex);
	return (run || !queue.empty());
}

// Close Capture
void VelodyneVLP16PCAP::close()
{
	run = false;
	// Close Capturte Thread
	if (thread && thread->joinable()) {
		thread->join();
		thread->~thread();
		delete thread;
		thread = nullptr;
	}

	std::lock_guard<std::mutex> lock(mutex);

	// Close PCAP
	if (pcap) {
		pcap_close(pcap);
		pcap = nullptr;
		filename = "";
	}

	// Clear Queue
	std::queue<std::vector<DataPoint>>().swap(queue);
};

// Retrieve Captured Data
void VelodyneVLP16PCAP::retrieve(std::vector<DataPoint> & lasers)
{
	// Pop One Rotation Data from Queue
	while (queue.empty() || !mutex.try_lock()) {}

	lasers = std::move(queue.front());
	queue.pop();
	mutex.unlock();
}

void VelodyneVLP16PCAP::readPCAP()
{
	double last_azimuth = 0.0;
	std::vector<DataPoint> dataPoints;

	while (run) {
		// Retrieve Header and Data from PCAP
		struct pcap_pkthdr* header;
		const unsigned char* data;
		const int ret = pcap_next_ex(pcap, &header, &data); //read the next packet from a pcap_t 
		if (ret <= 0) {
			break;
		}

		// Check Packet Data Size
		// Data Blocks ( 100 bytes * 12 blocks ) + Time Stamp ( 4 bytes ) + Factory ( 2 bytes ) = 1206 bytes
		if ((header->len - 42) != 1206) { //-42 while the header is 42 bytes
			continue;
		}

		// Retrieve Unix Time ( microseconds )
		std::stringstream ss;
		ss << header->ts.tv_sec << std::setw(6) << std::left << std::setfill('0') << header->ts.tv_usec;
		const long long unixtime = std::stoll(ss.str());

		// Convert to DataPacket Structure
		const DataPacket* packet = reinterpret_cast<const DataPacket*>(data + 42);
		parseDataPacket(packet, dataPoints, last_azimuth);
	}
	run = false;
}

void VelodyneVLP16PCAP::parseDataPacket(const DataPacket * packet, std::vector<DataPoint> & lasers, double& last_azimuth)
{
	// Retrieve Unix Time ( microseconds )
	const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	const std::chrono::microseconds epoch = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
	const long long unixtime = epoch.count();

	// Azimuth delta is the angle from one firing sequence to the next one 
	// Calculate the azimuth_delta value based on the packages
	long double azimuth_delta = 0.0;
	if (packet->firingData[1].rotationalPosition < packet->firingData[0].rotationalPosition) {
		azimuth_delta = ((packet->firingData[1].rotationalPosition + 36000) - packet->firingData[0].rotationalPosition);
	}
	else {
		azimuth_delta = (packet->firingData[1].rotationalPosition - packet->firingData[0].rotationalPosition);
	}

	// Processing Packet: 12 firings per package
	for (int firing_index = 0; firing_index < FIRING_PER_PKT; firing_index++) {
		// Retrieve Firing Data
		const FiringData firing_data = packet->firingData[firing_index];
		// 16 laser per firing
		for (int laser_index = 0; laser_index < LASER_PER_FIRING; laser_index++) {
			// Retrieve Rotation Azimuth
			double azimuth = static_cast<double>(firing_data.rotationalPosition);
			double laser_relative_time = LASER_PER_FIRING * time_between_firings + time_half_idle * (laser_index / MAX_NUM_LASERS);

			azimuth += azimuth_delta * laser_relative_time / time_total_cycle;

			// TODO DELETE, only for testing
			if (first)
			{
				first = false;
				cout<<endl<<"First azimuth: "<<azimuth<<endl;
			}

			// Reset Rotation Azimuth if it rolled over 360
			if (azimuth >= 36000)
			{
				azimuth -= 36000;
			}

			// Complete Retrieve Capture One Rotation Data
			if (last_azimuth > azimuth) {
				// Push One Rotation Data to Queue
				mutex.lock();
				if (queue.size() >= 1)
				{
					queue.pop();
				}
				queue.push(std::move(lasers));
				mutex.unlock();
				lasers.clear();
			}

			DataPoint laser;
			//Save the values
			laser.azimuth = azimuth / 100.0f;
			laser.vertical = vertAngle[laser_index % MAX_NUM_LASERS]; //Lut[constant values based on documentation]
			laser.distance = static_cast<float>(firing_data.laserReturns[laser_index].distance) * 2.0f / 10.0f;
			laser.intensity = firing_data.laserReturns[laser_index].intensity;
			laser.id = static_cast<unsigned char>(laser_index % MAX_NUM_LASERS);
			laser.time = unixtime + static_cast<long long>(laser_relative_time);

			//calculate coordinates
			laser.coordinates.x = static_cast<float>((laser.distance / 100.0f) *
				std::cos(laser.vertical * M_PI / 180.0f) * std::cos(laser.azimuth * M_PI / 180.0f));
			laser.coordinates.y = static_cast<float>(((laser.distance / 100.0f) *
				std::cos(laser.vertical * M_PI / 180.0f)) * std::sin(laser.azimuth * M_PI / 180.0f));
			laser.coordinates.z = static_cast<float>(((laser.distance / 100.0f) *
				std::sin(laser.vertical * M_PI / 180.0f)));


			lasers.push_back(laser);
			// Update Last Rotation Azimuth
			last_azimuth = azimuth;
		}
	}
}

