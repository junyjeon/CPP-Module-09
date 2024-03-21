#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <map>

class  BitcoinExchange {
public:
	double getExchangeRate(std::string const &) const;
	// void printExchangeRate(std::string const &) const;

	BitcoinExchange();
	BitcoinExchange(std::string const &filename);
	~BitcoinExchange();
private:
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	std::map<std::string, double> _data;
};

#endif // BITCOINEXCHANGE_HPP
