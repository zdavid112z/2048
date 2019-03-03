#include "pch.h"
#include "string.h"


std::vector<std::string> String::Split(const std::string& str, const std::string& separators)
{
	std::vector<std::string> res;
	int beg = 0;
	for (uint i = 0; i < str.size(); i++)
	{
		bool isSeparator = IsInside(separators, str[i]) != -1;
		if (isSeparator)
		{
			res.push_back(str.substr(beg, i - beg));
			beg = i;
			beg++;
		}
	}
	res.push_back(str.substr(beg));
	return res;
}

int String::IsInside(const std::string& str, char ch)
{
	for (uint i = 0; i < str.size(); i++)
		if (str[i] == ch) return i;
	return -1;
}

std::string String::ToLower(const std::string& str)
{
	std::string res = str;
	for (int i = 0; i < res.size(); i++)
		res[i] = tolower(res[i]);
	return res;
}

std::string String::ToUpper(const std::string& str)
{
	std::string res = str;
	for (int i = 0; i < res.size(); i++)
		res[i] = toupper(res[i]);
	return res;
}

std::vector<std::string> String::SplitMultiple(const std::string& str, const std::string& separators)
{
	std::vector<std::string> res;
	auto beg = str.end();
	auto it = str.begin();
	for (it = str.begin(); it != str.end(); it++)
	{
		bool isSeparator = std::find(separators.begin(), separators.end(), *it) != separators.end();
		if (beg == str.end() && !isSeparator)
			beg = it;
		else if (isSeparator && beg != str.end())
		{
			res.push_back(str.substr(beg - str.begin(), it - beg));
			beg = str.end();
		}
	}
	if (beg != str.end())
		res.push_back(str.substr(beg - str.begin()));
	return res;
}

std::string String::GetDirectory(const std::string& path)
{
	for (int i = path.size(); i >= 0; i--)
	{
		if (path[i] == '\\' || path[i] == '/')
			return path.substr(0, i + 1);
	}
	return "";
}

std::string String::GetFilename(const std::string& path)
{
	for (int i = path.size(); i >= 0; i--)
	{
		if (path[i] == '\\' || path[i] == '/')
			return path.substr(i + 1);
	}
	return path;
}

std::string String::ChangeDirectory(const std::string& currentDirectory, const std::string& directory)
{
	if (currentDirectory.empty() || currentDirectory == "\\")
		return directory;
	return currentDirectory + '\\' + directory;
}

