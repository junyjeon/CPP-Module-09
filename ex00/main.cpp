#include "BitcoinExchange.hpp"
#include <map>

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Error: could not open file" << std::endl;
        return 1;
    }
	
	std::fstream file(argv[1]);
	if (!file.is_open()) {
		std::cerr << "Error: could not open file" << std::endl;
		return 1;
	}

	std::string line;
	while(std::getline(file, line)) {
		std::stringstream ss(line);
		std::string item;
		while (std::getline(ss, item, '\n')) {
			//날짜 | 환율이 들어오면 map에 저장.

		}
		//
		
	}
	file.close();

	// std::map<std::pair<int, float> ,int>::iterator m;

    // 파일을 읽어서 BitcoinExchange 객체를 생성.
    // csv 형식인지 확인하고, 아니면 예외를 던진다.
	// 평가할 "날짜 | 환율"을 입력받는 두번째 DB를 입력받는다. 
    return 0;
}

