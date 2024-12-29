#pragma once

#include "events/Event.h"
#include "events/EventDispatcher.h"
#include "utilities/Log.h"

#include <string>
#include <iostream>
#include <vector>

#define PY_SSIZE_T_CLEAN
#include <Python/Python.h>

enum TargetLayer
{
	Main,
	Audio
};

class NLPEngine
{
public:
	NLPEngine() = default;
	~NLPEngine() = default;

	void Init();
	void DeInit();

	void SystemInput(const std::string& input, std::shared_ptr<EventDispatcher> dispatcher);

	std::string GetLayerData(const std::string& input);

private:
	std::vector<std::string> PyListToVec(PyObject* pyList);

private:
	TargetLayer Target = Main;

	PyObject* m_RandomResponseFunc{ nullptr };
	PyObject* m_AllResponsesFunc{ nullptr };
	PyObject* m_ResponseFromTagFunc{ nullptr };
	PyObject* m_ResultFunc{ nullptr };
};