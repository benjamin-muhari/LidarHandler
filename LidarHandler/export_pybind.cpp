//#define PYCAPI
#ifndef PYCAPI
// dont't compile this if c-api style exporting is being used
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>

#include <iostream>
#include <vector>

#include "LidarHandler.h"
#include "VLP16Capture.h"

using namespace std;
namespace py = pybind11;

class Cord
{
private:
	int x = 10;
	int y = 2;
	vector<vector<DataPoint>> _vector;
	vector<vector<int>> int_vector;
public:
	Cord()
	{
		vector<DataPoint> asd = vector<DataPoint>();
		for (int i=0;i<100;i++)
			asd.push_back(DataPoint());

		for (int i = 0; i < 10; i++)
			_vector.push_back(asd);

		vector<int> qwe = vector<int>();
		for (int i = 0; i < 100; i++)
			qwe.push_back(i);

		for (int i = 0; i < 10; i++)
			int_vector.push_back(qwe);
	}
	
	const int X() { return x; }
	const int Y() { return y; }
	const int SumOfCords() { return x + y; }
	void set(int a, int b)
	{
		x = a;
		y = b;
	}

	vector<vector<DataPoint>> readQ()
	{
		return _vector;
	}

	vector<vector<int>> readIntQ()
	{
		return int_vector;
	}
};

void HelloWorld()
{
	cout << "Hello World!" << endl;
}

PYBIND11_MODULE(example, m) {
	
}

PYBIND11_MODULE(LidarHandler, m) {
	py::class_<VelodyneVLP16PCAP>(m, "VeloParser")
		.def(py::init<>())
		.def("open_live", &VelodyneVLP16PCAP::open_live)
		.def("read_frame_vector", &VelodyneVLP16PCAP::export_frame)
		.def("read_frame_pyobj", &VelodyneVLP16PCAP::export_frame_pyobj);

	//m.def("run_cpp_program", &CallMain, R"pbdoc(
 //       Call the original c++ program!
 //   )pbdoc");

	py::class_<DataPoint>(m, "DataPoint")
		.def(py::init<>());

	py::class_<Cord>(m, "MyCord")
		.def(py::init<>())
		.def("getX", &Cord::X)
		.def("getY", &Cord::Y)
		.def("setXY", &Cord::set)
		.def("readCordQueue", &Cord::readQ)
		.def("readIntQueue", &Cord::readIntQ);

	m.def("hw", &HelloWorld, R"pbdoc(
        Hello world!
    )pbdoc");

#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif
}

#endif