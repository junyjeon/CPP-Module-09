#include "BitcoinExchange.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    try {
    BitcoinExchange btc("data.csv");
    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1; 
    }
    std::string line;
    while (std::getline(inputFile, line)) {
        std::stringstream lineStream(line); // line을 읽어서 lineStream에 저장
        std::string date; // "date"
        double value; // "value"
        
        // "date | value" 헤더 라인은 건너뜁니다.
        if (line == "date | value") {
            continue;
        }
        
        // lineStream에서 date와 value를 '|' 구분자로 읽어서 저장합니다.
        std::getline(lineStream, date, '|');
        lineStream >> value;
        
        // 입력 파싱에 실패한 경우 에러 메시지를 출력하고 다음 라인으로 넘어갑니다.
        if (lineStream.fail()) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        // date를 파싱하여 환율을 출력합니다.
        try {
            // "year-month-day" 형식에서 년, 월, 일을 추출합니다.
            int year, month, day;
            if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
                throw std::runtime_error("Invalid date format.");
            }

            // 월은 1~12 사이의 값이어야 합니다.
            if (month < 1 || month > 12) {
                throw std::runtime_error("Invalid date.");
            }
            
            // 윤년 여부를 판단합니다.
            bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            double exchangeRate = btc.getExchangeRate(date);
            std::cout << date << " | " << value << " => " << value * exchangeRate << std::endl;
        } catch (const std::exception& e) {
				// 에러가 발생한 경우 에러 메시지를 출력합니다.
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
		// 입력 파일을 닫습니다.
		inputFile.close();
	} catch (const std::exception& e) {
		// 파일 열기 또는 데이터 파싱 중 에러가 발생한 경우 에러 메시지를 출력하고 프로그램을 종료합니다.
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

    return 0;
}

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

