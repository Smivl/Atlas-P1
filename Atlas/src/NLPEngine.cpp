#include "NLPEngine.h"

void NLPEngine::Init()
{
	Py_Initialize();
	PyRun_SimpleString("import sys; import os; sys.path.insert(1, os.path.join(os.path.join(os.getcwd(), 'src'), 'system'));");
	PyRun_SimpleString("sys.path.insert(2, os.path.join(os.path.join(os.path.join(os.path.join(os.getcwd(), 'external'), 'libs'), 'Python'), 'Packages'));");

	PyObject* m_Module = PyImport_ImportModule("nlcm");

	if (!m_Module)
	{
		ATLAS_ERROR("Error importing python module : nlcm.py");
		return;
	}

	m_RandomResponseFunc = PyObject_GetAttrString(m_Module, (char*)"get_random_response");
	m_AllResponsesFunc = PyObject_GetAttrString(m_Module, (char*)"get_all_responses");
	m_ResponseFromTagFunc = PyObject_GetAttrString(m_Module, (char*)"get_response_from_tag");
	m_ResultFunc = PyObject_GetAttrString(m_Module, (char*)"get_result");
}

void NLPEngine::DeInit()
{
	Py_FinalizeEx();
}

void NLPEngine::SystemInput(const std::string& input, std::shared_ptr<EventDispatcher> dispatcher)
{
	PyObject* Result = PyObject_CallFunction(m_ResultFunc, "s", input.c_str()); // with args
	if (PyLong_Check(Result))
	{
		long Tag = PyLong_AsLong(Result);
		if (1 <= Tag && Tag <= 4)
		{
			Target = Audio;
			std::string channel = GetLayerData(input);

			switch (Tag) {
				case 1: 
				{
					dispatcher->publish<MuteEvent>(new MuteEvent(channel, 1)); 
					break;
				}
				case 2:
				{
					dispatcher->publish<MuteEvent>(new MuteEvent(channel, 0));
					break;
				}
				case 3:
				{
					dispatcher->publish<PauseEvent>(new PauseEvent(channel, 1));
					break;
				}
				case 4:
				{
					dispatcher->publish<PauseEvent>(new PauseEvent(channel, 0));
					break;
				}
				default:
				{
					ATLAS_ERROR("Tag is out of range");
				}
			}
		}
	}
	else
	{
		std::string Tag = std::string(PyUnicode_AsUTF8(Result));
		Result = PyObject_CallFunction(m_ResponseFromTagFunc, "s", Tag.c_str());
		std::cout << std::string(PyUnicode_AsUTF8(Result)) << std::endl;
	}
}

std::string NLPEngine::GetLayerData(const std::string& input)
{
	PyObject* Result = PyObject_CallFunction(m_ResultFunc, "si", input.c_str(), Target);
	return std::string(PyUnicode_AsUTF8(Result));
}

std::vector<std::string> NLPEngine::PyListToVec(PyObject* pyList) {
	std::vector<std::string> cppVector;
	Py_ssize_t size = PyList_Size(pyList);

	for (Py_ssize_t i = 0; i < size; i++) {
		PyObject* pyString = PyList_GetItem(pyList, i);
		std::string str = std::string(PyUnicode_AsUTF8(pyString));
		cppVector.push_back(str);
	}

	return cppVector;
}
