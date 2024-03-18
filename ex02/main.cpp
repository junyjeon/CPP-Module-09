// #include "PMergeMe.hpp"

// int main(int argc, char **argv) {
// 	if (argc < 2) {
// 		std::cerr << "Usage: " << argv[0] << " <sequence of positive integers>\n";
// 		return 1;
// 	}
 	
// 	// 양의 정수 시퀀스 준비
//     std::vector<int> sequence;
//     for (int i = 1; i < argc; ++i) {
//         int num = std::atoi(argv[i]);
//         if (num < 0) {
//             std::cerr << "Error: Only positive integers are allowed.\n";
//             return 1;
//         }
//         sequence.push_back(num);
//     }
//     // std::vector<int> sequence = {5, 3, 4, 1, 2, 8, 7, 6};
    
//     // 첫 번째 줄 출력 (정렬되지 않은 배열)
//     std::cout << "Before: ";
//     for (std::vector<int>::iterator it = sequence.begin(); it != sequence.end(); ++it) {
//         std::cout << *it << " ";
//     }
//     std::cout << "\n";
    
//     // 시간 측정 시작
//     clock_t start = clock();

//     // 정렬 알고리즘 실행 (예: vector 사용)
// 	// PMergeMe pm = PMergeMe(sequence);
// 	// pm.sort();
    
//     // 시간 측정 종료
//	   clock_t end = clock();
//     double elapsed = double(end - start) / CLOCKS_PER_SEC;
    
//     // 두 번째 줄 출력 (정렬된 시퀀스)
//     std::cout << "\nAfter: ";
//     for (std::vector<int>::iterator it = sequence.begin(); it != sequence.end(); ++it) {
//         std::cout << *it << " ";
//     }
//     std::cout << "\n";

//     // 세 번째 줄 출력 (첫 번째 컨테이너 시간 측정)
//     std::cout << std::fixed << std::setprecision(5); // 소수점 아래 5자리까지 표시
//     std::cout << "Time used with std::vector: " << elapsed << " seconds\n";
    
//     // 네 번째 줄 출력 (두 번째 컨테이너 시간 측정)
//     // start = clock();
// 	// PMergeMe pm2 = PMergeMe(sequence);
// 	// pm2.sort();
// 	// end = clock();
// 	// elapsed = end - start;
// 	// std::cout << "Time used with std::deque: " << elapsed << " seconds\n";
    
//     return 0;
// }

#include "PMergeMe.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <sequence of positive integers>\n";
        return 1;
    }
    PMergeMe pm;
    try {
        pm = PMergeMe(argc, argv);
    } catch (std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
	std::cout << "Before: ";
    pm.printSequence();

    clock_t start = clock();
	pm.sort();

	clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    
	std::cout << "After: ";
    pm.printSequence();

    std::cout << std::fixed << std::setprecision(5); // 소수점 아래 5자리까지 표시
    std::cout << "Time used with std::vector : " << elapsed << " seconds\n";
    
    start = clock();
	// PMergeMe pm2 = PMergeMe(sequence);
	// pm2.sort();
	end = clock();
	elapsed = end - start;
	std::cout << "Time used with std::deque : " << elapsed << " seconds\n";
    
	return 0;
}
