#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <bigint.hpp>

class BigFloat
{
private:
	BigInt unscaled;
	int scale;
	bool isNegative;

	void StripZeros();
	//static BigFloat addAbsolute(const BigFloat& BigInt1, const BigFloat& BigInt2);
	//static BigFloat subAbsolute(const BigFloat& BigInt1, const BigFloat& BigInt2);
	static int compareAbsolute(const BigFloat& a, const BigFloat& b);
public:
	// Constructors
	BigFloat();
	BigFloat(const std::string& number);
	BigFloat(double number);
	BigFloat(int number);

	// Overloading Operators
	BigFloat operator+(const BigFloat& other) const;
	BigFloat operator*(const BigFloat& other) const;
	BigFloat operator-(const BigFloat& other) const;
	BigFloat operator/(const BigFloat& other) const;
	std::pair<BigInt, BigInt> divMod10(BigInt num) const;

	BigInt pow10(int n) const;

	bool operator==(const BigFloat& other) const;
	bool operator!=(const BigFloat& other) const;
	bool operator<(const BigFloat& other) const;
	bool operator>(const BigFloat& other) const;
	bool operator<=(const BigFloat& other) const;
	bool operator>=(const BigFloat& other) const;

	// I/O stream overloading
	friend std::ostream& operator<<(std::ostream& out, const BigFloat& val);
	friend std::istream& operator>>(std::istream& in, BigFloat& val);

	// Custom toString converter for
	// pretty-printing (will be helpful in stream overloading)
	std::string toString() const;
};