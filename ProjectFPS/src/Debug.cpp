#include "pch.h"
#include "Debug.h"

#include <stdio.h>

void Debug::Log(const char* message)
{
	printf("INFO - %s\n", message);
}

void Debug::Log(const char* invoker, const char* message)
{
	printf("INFO - %s :: %s\n", invoker, message);
}

void Debug::Log(const std::string& message)
{
	Log(message.c_str()); // for now
}

void Debug::Log(const std::string& invoker, const std::string& message)
{
	Log(invoker.c_str(), message.c_str());
}

void Debug::LogWarning(const char* message)
{
	printf("WARNING - %s\n", message);
}

void Debug::LogWarning(const char* invoker, const char* message)
{
	printf("WARNING - %s :: %s\n", invoker, message);
}

void Debug::LogWarning(const std::string& message)
{
	LogWarning(message.c_str());
}

void Debug::LogWarning(const std::string& invoker, const std::string& message)
{
	LogWarning(invoker.c_str(), message.c_str());
}

void Debug::LogError(const char* message)
{
	printf("ERROR - %s\n", message);
}

void Debug::LogError(const char* invoker, const char* message)
{
	printf("ERROR - %s :: %s\n", invoker, message);
}

void Debug::LogError(const std::string& message)
{
	LogError(message.c_str());
}

void Debug::LogError(const std::string& invoker, const std::string& message)
{
	LogError(invoker.c_str(), message.c_str());
}