#pragma once
#include "bigint.hpp"
#include "bigfloat.hpp"

BigInt padRight(BigInt& x, int n);
std::string padRight(std::string& x, int n);
std::pair<std::string, uint32_t> divideDecimalString(const std::string& numStr, uint64_t divisor);