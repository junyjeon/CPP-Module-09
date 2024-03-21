#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <map>
#include <iomanip>

class  BitcoinExchange {
public:
	double getExchangeRate(std::string const &) const;
	void processFile(std::string const &);
	// void printExchangeRate(std::string const &) const;

	BitcoinExchange();
	BitcoinExchange(std::string const &);
	~BitcoinExchange();
private:
	BitcoinExchange(BitcoinExchange const &);
	BitcoinExchange &operator=(BitcoinExchange const &);
	std::map<std::string, double> _data;
};

#endif // BITCOINEXCHANGE_HPP

/*
날짜에 맞는 비트코인 가격을 출력하는 프로그램.
csv DB
second DB를 입력받아 prices/dates to evaluate? 를 저장.
프로그램 명: btc
파일을 인수로 사용
date | value
year-month-day
0~1000 float or positive integer -> float 사용
year-month-day => 환율 = value
날짜가 없으면 가장 가까운 과거의 날짜를 사용

// std::map<std::pair<int, float> ,int>::iterator m;

파일을 읽어서 BitcoinExchange 객체를 생성.
csv 형식인지 확인하고, 아니면 예외를 던진다.
평가할 "날짜 | 환율"을 입력받는 두번째 DB를 입력받는다. 
*/

