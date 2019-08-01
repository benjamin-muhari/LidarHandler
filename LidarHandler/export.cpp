#define PYBIND
#ifndef PYBIND
// dont't compile this if pybind is being used

#include <Python.h>
#include <iostream>
#include "VLP16Capture.h"
#include "LidarHandler.h"


PyObject* get_100_frames(PyObject*, PyObject* o)
{
	int channeldID = PyLong_AsLong(o);

	VelodyneVLP16PCAP capture;
	capture.open_live(channeldID);

	std::vector<DataPoint> dataPoints;
	PyObject* list_of_frames = PyList_New(0);
	int frame_count = 0;

	while (capture.isRun() && frame_count <100)
	{
		capture.retrieve(dataPoints);
		if (dataPoints.empty())
			continue;
		else
		{
			std::cout << "number of points in this frame: " << dataPoints.size() << std::endl;

			PyObject* frame_as_list = PyList_New(0);

			for (const DataPoint& laser : dataPoints)
			{
				PyObject* one_point = PyList_New(0);
				PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.x));
				PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.y));
				PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.z));

				PyList_Append(frame_as_list, one_point);
			}

			PyList_Append(list_of_frames, frame_as_list);
			frame_count++;
		}
	}

	return list_of_frames;
}

PyObject* get_a_frame(PyObject*,PyObject* o)
{
	int channeldID = PyLong_AsLong(o);
	
	VelodyneVLP16PCAP capture;
	capture.open_live(channeldID);

	std::vector<DataPoint> dataPoints;
	while (capture.isRun())
	{
		capture.retrieve(dataPoints);
		if (dataPoints.empty())
			continue;
		else
			break;
	}

	std::cout << "number of points in this frame: " << dataPoints.size() << std::endl;

	PyObject* frame_as_list = PyList_New(0);

	for (const DataPoint& laser : dataPoints)
	{
		PyObject* one_point = PyList_New(0);
		PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.x));
		PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.y));
		PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.z));

		PyList_Append(frame_as_list, one_point);
	}

	return frame_as_list;
}

static PyMethodDef lidarhandler_methods[] = {
	// The first property is the name exposed to Python, the second is the C++
	// function name that contains the implementation.
	{ "get_frame", (PyCFunction)get_a_frame, METH_O, nullptr },
	{ "get_100_frames", (PyCFunction)get_100_frames, METH_O, nullptr},
	{ "run_cpp_program", (PyCFunction)CallMain, METH_O, nullptr},

	// Terminate the array with an object containing nulls.
	{ nullptr, nullptr, 0, nullptr }
};

static PyModuleDef lidarhandler_module = {
	PyModuleDef_HEAD_INIT,
	"LidarHandler",										// Module name to use with Python import statements
	"Reads LIDAR point cloud data trough ethernet",		// Module description
	0,
	lidarhandler_methods								// Structure that defines the methods of the module
};

PyMODINIT_FUNC PyInit_LidarHandler() {
	return PyModule_Create(&lidarhandler_module);
}

#endif