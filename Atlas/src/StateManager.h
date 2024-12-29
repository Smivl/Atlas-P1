#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <streambuf>
#include <unordered_map>
#include <chrono>
#include <format>

#include "utilities/Cryptography.h"
#include "utilities/Log.h"

struct LogData
{
	const std::chrono::system_clock::time_point Time{ std::chrono::system_clock::now() };
	const std::string Input;
	const std::string Output;

	LogData(const std::string& I, const std::string& O) : Input{ I }, Output{ O } {}
};

class StateManager
{
public:
	static void LoadState();
	static void SaveState();
	
	static void NewLog(LogData Data);

	// Queues object data for saving (it will only save if SaveState is called after)
	static void QueueObjectItem(std::string& ObjName, std::string VarName, std::string VarValue);
private:
	static const std::string m_ObjectEnd;
	static const std::string m_ObjectName;
	static const std::string m_EndLine;
	static const std::string m_VarLim;

	static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_StateData;
};

