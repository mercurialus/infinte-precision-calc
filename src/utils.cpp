#include "utils.hpp"

BigInt padRight(BigInt& num, int n)
{
	BigInt padded = num;
	while (padded.size() < n)
		padded.value.push_back('0');
	return padded;
}

std::string padRight(const std::string& x, int n)
{
	std::string padded = x;
	while (padded.size() < n)
		padded.push_back('0');
	return padded;
}
BigInt padLeft(BigInt& num, int n)
{
	BigInt padded = num;
	while (padded.size() < n)
		padded.value.insert(padded.value.begin(), '0'); // prepend '0'
	return padded;
}
std::string padLeft(const std::string& x, int n)
{
	std::string padded = x;
	while (padded.size() < n)
		padded.insert(padded.begin(), '0'); // prepend '0'
	return padded;
}

std::pair<std::string, uint32_t> divideDecimalString(const std::string& numStr, uint64_t divisor)
{
	std::string result;
	uint64_t rem = 0;

	for (char ch : numStr)
	{
		rem = rem * 10 + (ch - '0');
		if (!result.empty() || rem >= divisor)
			result += (rem / divisor) + '0';
		rem %= divisor;
	}

	if (result.empty())
		result = "0";
	return { result, static_cast<uint32_t>(rem) };
}