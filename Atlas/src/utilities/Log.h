#pragma once

#include <memory>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Log
{

public:

	static void Init();

	static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

private:

	static std::shared_ptr<spdlog::logger> s_CoreLogger;
};



// Core log macros
#define ATLAS_TRACE(...)    Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ATLAS_INFO(...)     Log::GetCoreLogger()->info(__VA_ARGS__)
#define ATLAS_WARN(...)     Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ATLAS_ERROR(...)    Log::GetCoreLogger()->error(__VA_ARGS__)
#define ATLAS_CRITICAL(...) Log::GetCoreLogger()->critical(__VA_ARGS__)

