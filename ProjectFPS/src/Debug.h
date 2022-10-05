#pragma once
class Debug
{
public:
	static void Log(const char* message);
	static void Log(const char* invoker, const char* message);
	static void Log(const std::string& message);
	static void Log(const std::string& invoker, const std::string& message);

	static void LogWarning(const char* message);
	static void LogWarning(const char* invoker, const char* message);
	static void LogWarning(const std::string& message);
	static void LogWarning(const std::string& invoker, const std::string& message);

	static void LogError(const char* message);
	static void LogError(const char* invoker, const char* message);
	static void LogError(const std::string& message);
	static void LogError(const std::string& invoker, const std::string& message);
};

