#include "BitcoinExchange.hpp"

void BitcoinExchange::processFile(const std::string& inputFile) {
    std::string inputFileCopy = inputFile;
    std::ifstream file(inputFileCopy.c_str());
    if (!file) {
        throw std::runtime_error("Error: could not open file.");
    }
    
    std::string line;
    while (std::getline(file, line)) {
		std::stringstream lineStream(line); // line을 lineStream에 저장
        std::string date; // "date"
		double value;
		char leftOver;
        
        // "date | value"
        if (line == "date | value") {
            continue;
        }
        
        // date 저장
        std::getline(lineStream, date, '|');
        
		try {
			// 0000-00-00 | 00+00 일 때
			if (!(lineStream >> value) || lineStream >> leftOver || !(lineStream.fail())) {
				throw std::runtime_error("bad input.");
			}

			std::istringstream dateStream(date);
			int year, month, day;
			char dash1, dash2;
			dateStream >> year >> dash1 >> month >> dash2 >> day;

			// dash1, dash2 '-'
			if (date.length() != 11 || dateStream.fail() || dash1 != '-' || dash2 != '-') {
				throw std::runtime_error("invalid date format.");
			}
			
			// year 1000~9999
			if (year < 1000 || year > 9999) {
				throw std::runtime_error("invalid year.");
			}

            // month 1~12
			if (month < 1 || month > 12) {
				throw std::runtime_error("invalid date.");
			}
			
			// 윤년 여부
			bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
			
			// 월에 따른 일 수 계산
			int daysInMonth;
			switch (month) {
				case 2:
					daysInMonth = isLeapYear ? 29 : 28;
					break;
				case 4: case 6: case 9: case 11:
					daysInMonth = 30;
					break;
				default:
					daysInMonth = 31;
			}
			
			// 일 < 1, 일 > daysInMonth
			if (day < 1 || day > daysInMonth) {
				throw std::runtime_error("invalid date.");
			}

			// 값 < 0
			if (value < 0) {
				throw std::runtime_error("not a positive number.");
			}

            // 값 > 1000
			if (value > 1000) {
				throw std::runtime_error("too large a number.");
			}
			
			// 환율 출력
			double exchangeRate = this->getExchangeRate(date);
			
			// date => value = value * exchangeRate
			std::cout << date << "=> " << value << " = " 
						<< std::fixed << std::setprecision(3) 
						<< value * exchangeRate << std::endl;
		} catch (const std::exception& e) {
			// 에러가 발생한 경우 에러 메시지를 출력
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	file.close();
}

BitcoinExchange::BitcoinExchange(std::string const &filename) {
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
        if (!lineStream.fail()) {
            _data[date] = price;
        }
    }
}

double BitcoinExchange::getExchangeRate(std::string const &date) const {
    std::map<std::string, double>::const_iterator it = _data.upper_bound(date); // date보다 큰 첫번째 원소. upper_bound는 date보다 큰 첫번째 원소의 iterator를 반환
    if (it == _data.begin()) {
        throw std::runtime_error("Date too early.");
    }
    --it;
    return it->second;
}

BitcoinExchange::BitcoinExchange() : _data() {}

BitcoinExchange::BitcoinExchange(BitcoinExchange const &other) : _data(other._data) {}

BitcoinExchange& BitcoinExchange::operator=(BitcoinExchange const &other) {
	if (this != &other) {
		_data = other._data;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}
