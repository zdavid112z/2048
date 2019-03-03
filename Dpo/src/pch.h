#pragma once

#include <cstring>
#include <cstdlib>
#include <cmath>

#ifdef PRODUCTION
#define LOG_INFO(...)
#define LOG_VERB(...)
#elif defined NDEBUG
#include <cstdio>
#define LOG_INFO(...) printf(__VA_ARGS__) 
#define LOG_VERB(...)
#else
#include <cstdio>
#define LOG_INFO(...) printf(__VA_ARGS__) 
#define LOG_VERB(...) printf(__VA_ARGS__) 
#endif

#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <bitset>

using std::pair;
using std::sort;
using std::vector;
using std::queue;
using std::priority_queue;
using std::string;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef long double lfloat;

typedef uint8 byte;
typedef unsigned int uint;
typedef long long llong;
typedef unsigned long long ullong;