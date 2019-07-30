#include <Python.h>
#include <iostream>
#include <string>

PyObject* return_float(PyObject*, PyObject* o)
{
	return PyFloat_FromDouble(2.5);
}

void print_something()
{
	std::cout << "Valami";
}

//PyObject* return_vector(PyObject*, PyObject* o)
//{
//	PyObject* asd = new PyObject;
//	return asd;
//}

//void print_this(PyObject*, PyObject* o)
//{
//	std::string mystr = (o->ob_type->tp_str)(o);
//	std::cout<< (*Py_TYPE(o)->tp_str)(o);
//	res = (*Py_TYPE(o)->tp_str)(o);
//	std::cout << ;
//}

//
//PyObject* return_string()
//{
//	PyObject* two_point_5 = PyFloat_FromDouble(2.5);
//	PyObject* tp5asString = PyObject_Str(two_point_5);
//
//}


static PyMethodDef cppdemo_methods[] = {
	{"get_float", (PyCFunction)return_float, METH_O, nullptr},
	{"print_something", (PyCFunction)print_something, METH_O, nullptr},
	//{"get_vector", (PyCFunction)return_vector, METH_O, nullptr},
	//{"print_this", (PyCFunction)print_this, METH_O, nullptr},
	{nullptr, nullptr, 0, nullptr}
};

static PyModuleDef cppdemo_module = { PyModuleDef_HEAD_INIT,"LidarHandler","Just read a vector from cpp",0,cppdemo_methods };

PyMODINIT_FUNC PyInit_LidarHandler() { return PyModule_Create(&cppdemo_module); }