#include "bigfloat.hpp"  
#include <iomanip>  
#include <iostream>  
#include <sstream>  
#include <stdexcept>
#include <regex>
#include <string>  
using std::cout, std::endl;


BigFloat::BigFloat() : isNegative(false), scale(0), unscaled(0) {}

BigFloat::BigFloat(const std::string& number)
{
	if (number.empty())
	{
		std::cerr << "Empty string" << endl;
	}
	isNegative = (number[0] == '-');
	int start = (number[0] == '+' || number[0] == '-') ? 1 : 0;
	int dot = number.find('.', start);
	std::string intPart, fracPart;
	if (dot == std::string::npos)
	{
		intPart = number.substr(start);
		fracPart = "";
	}
	else {
		intPart = number.substr(start,dot);
		fracPart = number.substr(dot + 1);
	}
	if (intPart.empty())
		intPart = "0";
	if(fracPart.empty())
		fracPart="0";

	intPart = std::regex_replace(intPart, std::regex("^0+(?!$)"), "");
	fracPart = std::regex_replace(fracPart, std::regex("^0+(?!$)"), "");
	scale = fracPart.length();

	std::string combined = intPart + fracPart;
	if (combined.empty()) combined = "0";
	unscaled = BigInt(combined);
	if (unscaled.size() == 1 && unscaled.toString() == "0")
			isNegative = false;
}

BigFloat::BigFloat(double number)
	: BigFloat(std::to_string(number)) {
}

BigFloat::BigFloat(int number)
	: BigFloat(std::to_string(number)) {

}


//void BigFloat::StripZeros()
//{
//	while (value.size() > 1 && value.back() == 0)
//	{
//		value.pop_back();
//	}
//	if (value.size() == 1 && value[0] == 0)
//	{
//		IsNegative = false;
//		decimal = 0;
//	}
//}

BigInt BigFloat::pow10(int n) const
{
	if (n <= 0)
		return BigInt(1);
	
	int k = n / 4;
	int r = n % 4;
	BigInt ten4 = BigInt(10000);
	BigInt res = BigInt(1);

	for (int i = 0; i < k; i++)
		res = res*ten4;
	int small = (int)pow(10, r);
	res = res*BigInt(small);
	return res;
}

bool BigFloat::operator==(const BigFloat& other) const
{
	return (compareAbsolute(*this, other) == 0) ? true : false;
}

bool BigFloat::operator!=(const BigFloat& other) const
{
	return !(*this == other);
}

bool BigFloat::operator<(const BigFloat& other) const
{
	if (this->isNegative != other.isNegative)
		return this->isNegative;

	bool both_negative = this->isNegative;
	int scale_diff = this->scale - other.scale;
	BigInt left = this->unscaled;
	BigInt right = other.unscaled;

	if (scale_diff > 0) {
		right = right * this->pow10(scale_diff);
	}
	else if (scale_diff < 0) {
		left = left * this->pow10(-scale_diff);
	}
		
	bool result = left < right;

	return both_negative ? !result && (left != right) : result;
	
}

bool BigFloat::operator>(const BigFloat& other) const {
	return !(*this < other);
}

bool BigFloat::operator<=(const BigFloat& other) const
{
	return (*this < other) || (*this == other);
}
bool BigFloat::operator>=(const BigFloat& other) const
{
	return (*this > other) || (*this == other);
}



BigFloat BigFloat::operator+(const BigFloat & other) const 
{
	int common = std::max(this->scale, other.scale);
	BigInt u1 = this->unscaled * pow10(common - this->scale);
	BigInt u2 = other.unscaled * pow10(common - other.scale);

	BigInt res;
	bool neg;
	
	if (this->isNegative == other.isNegative)
	{
		res = u1 + u2;
		res.isNegative = this->isNegative;
	}
	else {
		int cmp = BigInt::compareAbsolute(u1,u2);
		if (cmp >= 0)
		{
			res = u1 - u2;
			neg = this->isNegative;
		}
		else {
			res = u2 - u1;
			neg = other.isNegative;
		}
	}

	BigFloat out("");
	out.unscaled = res;
	out.scale = common;
	out.isNegative = neg && !(res.size() == 1 && res.toString() == "0");
	out.StripZeros();
	return out;
}

BigFloat BigFloat::operator-(const BigFloat & other) const 
{
	return (*this) + (BigFloat(-1) * other);
}

BigFloat BigFloat::operator*(const BigFloat& other) const
{
	BigFloat out(0);
	out.unscaled = this->unscaled * other.unscaled;
	out.scale = this->scale + other.scale;

	out.isNegative = (this->isNegative != other.isNegative);
	out.StripZeros();
	return out;
}

BigFloat BigFloat::operator/(const BigFloat& other) const {
	if (other.unscaled == BigInt(0)) {
		throw std::runtime_error("Division by zero");
	}

	BigInt numerator = this->unscaled;
	BigInt denominator = other.unscaled;
	BigInt quotient = numerator / denominator;
	BigInt remainder = numerator % denominator;

	BigInt ten = BigInt(10);
	int resultScale = 0;
	const int MAX_PRECISION = 10000;
	// We'll build the full decimal expansion until remainder == 0
	// Shift quotient to the left by multiplying and appending decimal digits
	while (remainder != 0 && resultScale < MAX_PRECISION) {
		remainder = remainder * ten;
		BigInt digit = remainder / denominator;
		quotient = quotient * ten + digit;
		remainder = remainder % denominator;
		resultScale++;
	}

	BigFloat result;
	result.unscaled = quotient;
	result.scale = this->scale - other.scale + resultScale;
	result.isNegative = (this->isNegative != other.isNegative); // XOR
	result.StripZeros();  // Optional: remove trailing zeroes if you have this

	return result;
}
std::pair<BigInt, BigInt> BigFloat::divMod10(BigInt num) const
{
	std::vector<uint32_t> res(num.value.size(), 0);
	uint32_t rem = 0;

	for (int i = static_cast<int>(num.value.size()) - 1; i >= 0; --i) {
		uint32_t cur = num.value[i] + rem * 10000;
		res[i] = cur / 10;
		rem = cur % 10;
	}

	BigInt q;
	q.value = std::move(res);
	q.StripZeros();  // or q.stripZeros(); depending on your naming

	BigInt r(rem);  // assuming BigInt has constructor from int

	return { q, r };
}


void BigFloat::StripZeros()
{
	while (scale > 0) {
		if (unscaled.value[0] % 10 != 0)
			break;
		// divide by 10 by repeated subtraction of digits
		auto [quotient,remainder] = divMod10(unscaled);
		if (remainder.size() == 1 && remainder.toString()=="0") { // remainder 0
			unscaled = quotient;
			scale--;
		}
		else
			break;
	}
	// if value is exactly 0 -> sign = positive
	if (unscaled.size() == 1 && unscaled.toString()=="0")
		isNegative = false;
}

int BigFloat::compareAbsolute(const BigFloat& a, const BigFloat& b) {
	// Align scales by multiplying the unscaled part with appropriate power of 10
	int scale_diff = a.scale - b.scale;

	BigInt left = a.unscaled;
	BigInt right = b.unscaled;

	if (scale_diff > 0) {
		// b has fewer fractional digits->scale b up
		right = right * b.pow10(scale_diff);
	}
	else if (scale_diff < 0) {
		// a has fewer fractional digits->scale a up
		left = left * a.pow10(-scale_diff);
	}

	return BigInt::compareAbsolute(left, right);
}



std::string BigFloat::toString() const {
	std::string raw = unscaled.toString();  // Assume BigInt has toString()
	std::string result;

	// Add sign if negative and unscaled is not zero
	if (isNegative && raw != "0") {
		result += '-';
	}

	int len = raw.length();

	// Case 1: If scale is 0, it's an integer
	if (scale == 0) {
		result += raw;
		return result;
	}

	// Case 2: If scale >= number of digits, pad with leading zeros
	if (scale >= len) {
		result += "0.";
		result += std::string(scale - len, '0');
		result += raw;
		return result;
	}

	// Case 3: Insert decimal point inside the number
	int dotPos = len - scale;
	result += raw.substr(0, dotPos);
	result += '.';
	result += raw.substr(dotPos);

	return result;
}

std::ostream& operator<<(std::ostream& out, const BigFloat& val)
{
	std::string output = val.toString();
	out << output;
	return out;
}

std::istream& operator>>(std::istream& in, BigFloat& val)
{
	std::string input;
	in >> input; 

	try {
		val = BigFloat(input);
	}
	catch (const std::exception& e) {
		in.setstate(std::ios::failbit);
	}

	return in;
}
