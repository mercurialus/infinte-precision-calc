#include "bigint.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
using std::cout, std::endl;

// Constructors
BigInt::BigInt() : isNegative(false) {}

BigInt::BigInt(const std::string& number)
{
	isNegative = !number.empty() && number[0] == '-';

	int start = isNegative ? 1 : 0;
	for (int i = number.size(); i > start; i -= 4)
	{
		int end = i;
		int begin = std::max(start, i - 4);
		std::string segment = number.substr(begin, end - begin);
		value.push_back(std::stoi(segment));
	}

	StripZeros();
}

BigInt::BigInt(int number)
{
	isNegative = number < 0;
	number = std::abs(number);

	while (number > 0)
	{
		value.push_back(number % 10000);
		number /= 10000;
	}

	if (value.empty())
		value.push_back(0);
}

// Remove leading zeros
void BigInt::StripZeros()
{
	while (value.size() > 1 && value.back() == 0)
	{
		value.pop_back();
	}

	if (value.size() == 1 && value[0] == 0)
	{
		isNegative = false;
	}
}

// Compare absolute values
int BigInt::compareAbsolute(const BigInt& a, const BigInt& b)
{
	if (a.value.size() != b.value.size())
		return a.value.size() > b.value.size() ? 1 : -1;

	for (int i = a.value.size() - 1; i >= 0; --i)
	{
		if (a.value[i] != b.value[i])
			return a.value[i] > b.value[i] ? 1 : -1;
	}

	return 0;
}

// Addition ignoring signs
BigInt BigInt::addAbsolute(const BigInt& a, const BigInt& b)
{
	BigInt result;
	result.value.clear();

	size_t maxSize = std::max(a.value.size(), b.value.size());
	uint32_t carry = 0;

	for (size_t i = 0; i < maxSize || carry != 0; ++i)
	{
		uint32_t aVal = i < a.value.size() ? a.value[i] : 0;
		uint32_t bVal = i < b.value.size() ? b.value[i] : 0;

		uint32_t sum = aVal + bVal + carry;
		result.value.push_back(sum % 10000);
		carry = sum / 10000;
	}

	return result;
}

// Subtraction ignoring signs: assumes a >= b
BigInt BigInt::subAbsolute(const BigInt& a, const BigInt& b)
{
	BigInt result;
	result.value.clear();

	uint32_t borrow = 0;

	for (size_t i = 0; i < a.value.size(); ++i)
	{
		int32_t aVal = a.value[i];
		int32_t bVal = i < b.value.size() ? b.value[i] : 0;

		int32_t diff = aVal - bVal - borrow;

		if (diff < 0)
		{
			diff += 10000;
			borrow = 1;
		}
		else
		{
			borrow = 0;
		}

		result.value.push_back(diff);
	}

	result.StripZeros();
	return result;
}

// Operator Overloading
BigInt BigInt::operator+(const BigInt& other) const
{
	if (isNegative == other.isNegative)
	{
		BigInt result = addAbsolute(*this, other);
		result.isNegative = isNegative;
		return result;
	}
	else
	{
		if (compareAbsolute(*this, other) >= 0)
		{
			BigInt result = subAbsolute(*this, other);
			result.isNegative = isNegative;
			return result;
		}
		else
		{
			BigInt result = subAbsolute(other, *this);
			result.isNegative = other.isNegative;
			return result;
		}
	}
}

BigInt BigInt::operator-(const BigInt& other) const
{
	if (isNegative != other.isNegative)
	{
		BigInt result = addAbsolute(*this, other);
		result.isNegative = isNegative;
		return result;
	}
	else
	{
		if (compareAbsolute(*this, other) >= 0)
		{
			BigInt result = subAbsolute(*this, other);
			result.isNegative = isNegative;
			return result;
		}
		else
		{
			BigInt result = subAbsolute(other, *this);
			result.isNegative = !isNegative;
			return result;
		}
	}
}

BigInt BigInt::operator*(const BigInt& other) const
{
	BigInt result;
	uint32_t BASE = 10000;
	result.value.resize(value.size() + other.value.size(), 0);
	result.isNegative = (isNegative != other.isNegative);

	for (size_t i = 0; i < value.size(); i++)
	{
		uint64_t carry = 0;
		for (size_t j = 0; j < other.value.size() || carry != 0; j++)
		{
			uint64_t curr = result.value[i + j] + carry;
			if (j < other.value.size())
				curr += (uint64_t)value[i] * other.value[j];

			result.value[i + j] = (uint32_t)(curr % BASE);
			carry = curr / BASE;
		}
	}
	result.StripZeros();
	if (result.value.size() == 1 && result.value[0] == 0)
		result.isNegative = false;
	return result;
}
BigInt BigInt::operator/(const BigInt& divisor) const
{
	if (divisor == BigInt("0"))
		throw std::runtime_error("Division By Zero");

	BigInt dividend = *this;

	BigInt quotient;
	quotient.value.resize(dividend.value.size(), 0);

	BigInt current("0");

	for (int i = (int)dividend.value.size() - 1; i >= 0; --i)
	{

		current.value.insert(current.value.begin(), dividend.value[i]);
		current.StripZeros();

		int x = 0;
		int l = 0, r = 9999;

		while (l <= r)
		{
			int m = (l + r) / 2;
			BigInt testVal = divisor * BigInt(std::to_string(m));
			if (testVal <= current)
			{
				x = m;
				l = m + 1;
			}
			else
			{
				r = m - 1;
			}
		}

		quotient.value[i] = x;

		BigInt xAsBigInt(std::to_string(x));
		current = current - (divisor * xAsBigInt);
	}

	quotient.StripZeros();

	if (quotient != BigInt("0"))
	{
		quotient.isNegative = (isNegative != divisor.isNegative);
	}

	return quotient;
}

BigInt BigInt::operator%(const BigInt& other) const
{
	BigInt quotient = *this / other;
	BigInt product = quotient * other;
	BigInt remainder = *this - product;
	return remainder;
}


bool BigInt::operator==(const BigInt& other) const
{
	return isNegative == other.isNegative && value == other.value;
}

bool BigInt::operator!=(const BigInt& other) const
{
	return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const
{
	if (isNegative != other.isNegative)
		return isNegative;

	int cmp = compareAbsolute(*this, other);
	return isNegative ? cmp > 0 : cmp < 0;
}

bool BigInt::operator>(const BigInt& other) const
{
	return other < *this;
}

bool BigInt::operator<=(const BigInt& other) const
{
	return !(*this > other);
}

bool BigInt::operator>=(const BigInt& other) const
{
	return !(*this < other);
}

std::string BigInt::toString() const
{
	if (value.empty())
		return "0";

	std::ostringstream oss;
	if (isNegative)
		oss << '-';

	oss << value.back(); // first (most significant) group

	for (int i = value.size() - 2; i >= 0; --i)
		oss << std::setw(4) << std::setfill('0') << value[i];

	return oss.str();
}

std::ostream& operator<<(std::ostream& out, const BigInt& val)
{
	std::string s = val.toString();
	if (s.find_first_of('-') != std::string::npos && val.size() == 1 && s[1] == '0')
		out << s.substr(1);
	else
		out << s;
	return out;
}

std::istream& operator>>(std::istream& in, BigInt& val)
{
	std::string s;
	in >> s;
	val = BigInt(s);
	return in;
}

size_t BigInt::size() const
{
	return value.size();
}