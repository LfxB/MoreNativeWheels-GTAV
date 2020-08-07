#pragma once
#include <cstdint>
#include <vector>

namespace Memory {
	uintptr_t FindPattern(const char* pattern, const char* mask);
	std::vector<uintptr_t> FindPatterns(const char* pattern, const char* mask);
}