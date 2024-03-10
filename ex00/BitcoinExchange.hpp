#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>

template <typename T>
typename T::iterator easyfind(T &container, int value);

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
*/