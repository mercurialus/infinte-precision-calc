#pragma once

#include <iostream>
#include <string>
#include <vector>

class BigInt
{
private:
	std::vector<uint32_t> value; // Will be storing in reverse for ease of implementation
	bool isNegative;

	friend class BigFloat;

	void StripZeros();
	static BigInt addAbsolute(const BigInt& BigInt1, const BigInt& BigInt2);
	static BigInt subAbsolute(const BigInt& BigInt1, const BigInt& BigInt2);
	static int compareAbsolute(const BigInt& BigInt1, const BigInt& BigInt2);

public:
	// Constructors
	BigInt();
	BigInt(const std::string& number);
	BigInt(int number);

	// Overloading Operators
	BigInt operator+(const BigInt& other) const;
	BigInt operator*(const BigInt& other) const;
	BigInt operator-(const BigInt& other) const;
	BigInt operator/(const BigInt& other) const;
	BigInt operator%(const BigInt& other) const;

	bool operator==(const BigInt& other) const;
	bool operator!=(const BigInt& other) const;
	bool operator<(const BigInt& other) const;
	bool operator>(const BigInt& other) const;
	bool operator<=(const BigInt& other) const;
	bool operator>=(const BigInt& other) const;
	BigInt operator+=(const BigInt& other) const;
	BigInt operator-=(const BigInt& other) const;
	BigInt operator*=(const BigInt& other) const;
	BigInt operator/=(const BigInt& other) const;
	BigInt operator%=(const BigInt& other) const;


	// I/O stream overloading
	friend std::ostream& operator<<(std::ostream& out, const BigInt& val);
	friend std::istream& operator>>(std::istream& in, BigInt& val);

	// Custom toString converter for
	// pretty-printing (will be helpful in stream overloading)
	std::string toString() const;

	// Friend Functions for easy implementation
	friend BigInt karatsubaMultiply(const BigInt& x, const BigInt& y);

	// Utils
	size_t size() const;
	friend BigInt padRight(BigInt& BigInt1, int n);
	friend BigInt padLeft(BigInt& BigInt1, int n);
};