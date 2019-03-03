#pragma once

#include "pch.h"


class String
{
public:
	static std::vector<std::string> Split(const std::string& str, const std::string& separators);
	// Doesn't care if there are multiple consecutive separators
	static std::vector<std::string> SplitMultiple(const std::string& str, const std::string& separators);
	static std::string GetDirectory(const std::string& path);
	static std::string GetFilename(const std::string& path);
	static std::string ChangeDirectory(const std::string& currentDirectory, const std::string& directory);
	// Returns -1 when not inside, otherwise returns the position
	static int IsInside(const std::string& str, char ch);
	static std::string ToLower(const std::string& str);
	static std::string ToUpper(const std::string& str);
	
	template <class T>
	static std::string ToString(const T& v) {
		return std::to_string(v);
	}

	template <class T>
	static std::string ToStringFloat(const T& v, int digits) {
		std::string res = std::to_string(v);
		size_t i = res.find('.');
		if (i + 1 + digits > res.size() || i == std::string::npos)
			return res;
		return res.substr(0, i + 1 + digits);
	}

	template <class T>
	static T ValueOf(const std::string& str) { LOG_VERB("Cannot convert to string!\n"); }

	template <>
	static int ValueOf(const std::string& str) {
		return atoi(str.c_str());
	}
	template <>
	static int64 ValueOf(const std::string& str) {
		return atoll(str.c_str());
	}
	template <>
	static double ValueOf(const std::string& str) {
		return atof(str.c_str());
	}
	template <>
	static float ValueOf(const std::string& str) {
		return (float)atof(str.c_str());
	}
	template <>
	static std::string ValueOf(const std::string& str) {
		return str;
	}
	template <>
	static bool ValueOf(const std::string& str) {
		std::string lower = ToLower(str);
		if (lower == "false" || str == "0") return false;
		if (lower == "true" || str == "1") return true;
		return false;
	}
};

