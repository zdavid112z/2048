#pragma once

#include "pch.h"
 
class Files
{
public:
	static char* ReadFileBinary(const std::string& path, uint64& len);
	static std::string ReadFileText(const std::string& path, size_t bufferSize = 4096);
	static void WriteTextFile(const std::string& path, const std::string& text);
};

