#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file) {
        throw std::runtime_error("Could not open file.");
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string date;
        double price;

        std::getline(lineStream, date, ',');
        lineStream >> price;
        if (!lineStream.fail()) { // pri
            _data[date] = price;
        }
    }
}

double BitcoinExchange::getExchangeRate(const std::string& date) const {
    std::map<std::string, double>::const_iterator it = _data.upper_bound(date); // date보다 큰 첫번째 원소 upper_bound는 date보다 큰 첫번째 원소의 iterator를 반환
    if (it == _data.begin()) {
        throw std::runtime_error("Date too early.");
    }
    --it;
    return it->second;
}

BitcoinExchange::BitcoinExchange() : _data() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _data(other._data) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
	if (this != &other) {
		_data = other._data;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}
