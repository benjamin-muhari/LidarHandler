#define PYBIND
#ifdef PYBIND
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


PYBIND11_MODULE(LidarHandler, m) {
	py::class_<VelodyneVLP16PCAP>(m, "VeloParser")
		.def(py::init<>())
		.def("open_live", &VelodyneVLP16PCAP::open_live)
		.def("read_frame_vector", &VelodyneVLP16PCAP::export_frame)
		.def("read_frame_pyobj", &VelodyneVLP16PCAP::export_frame_pyobj);

	m.def("run_cpp_program", &CallMain, R"pbdoc(
        Call the original c++ program!
    )pbdoc");

#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif
}

#endif