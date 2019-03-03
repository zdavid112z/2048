#include "pch.h"
#include "files.h"


char* Files::ReadFileBinary(const std::string& path, uint64& len)
{
	FILE* file = fopen(path.c_str(), "rb");
	_fseeki64(file, 0, SEEK_END);
	len = _ftelli64(file);
	_fseeki64(file, 0, SEEK_SET);
	char* result = new char[size_t(len) + 1];
	fread(result, 1, size_t(len), file);
	result[len] = 0;
	fclose(file);
	return result;
}

std::string Files::ReadFileText(const std::string& path, size_t bufferSize)
{
	FILE* file = fopen(path.c_str(), "r");
	std::string result;
	char* buffer = (char*) alloca(bufferSize);
	char* line;
	while ((line = fgets(buffer, bufferSize, file)) != NULL)
		result += line;
	fclose(file);
	return result;
}

void Files::WriteTextFile(const std::string& path, const std::string& text)
{
	FILE* file = fopen(path.c_str(), "w");
	fprintf(file, "%s", text.c_str());
	fclose(file);
}

