#pragma once

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Standards
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <array>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <functional>
#include <ranges>

#include <source_location>

// Core
#include "core/Hash.h"
#include "core/Format.h"

#define LOG(fmt, ...)   Logger::WriteMessage(Format("{}\n", Format(fmt, __VA_ARGS__)).c_str())