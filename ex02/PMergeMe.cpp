#include "PMergeMe.hpp"

void PMergeMe::sort() {
	// 1. indexPairs 초기화. pair<int(값), int(인덱스)>
	// 예) 2 1 4 3 -> (2, 0), (1, 1), (4, 2), (3, 3)
	initIndexPairs();
	
	// 2. pairs 초기화. pair<int(큰 수), int(작은 수)>
	// 예) 2 1 4 3 -> (2, 1), (4, 3)
	initPairs();
	
	// 3. 요소 그룹화. 첫 번째 요소들을 짝지어 pair(큰 수, 작은 수)의 개수가 2개 이하가 될 때까지 그룹화(재귀)
	// 홀수일 경우, 마지막 요소는 pair의 두 번째 요소를 -1로 설정
	// 예) (값) 2 1 4 3 -> (2, 1), (4, 3)
	// 예) (index) 0 1 2 3 -> (0, 1), (2, 3)

	// 예2) 8 2 4 6 1 3 7 5 -> (8, 2), (6, 4), (3, 1), (7, 5)
	// -> [(8, 2), (6, 4)]와 [(3, 1), (7, 5)] 비교
	// -> 최종적으로 [(8, 6)]과 [(7, 3)]이 남음
	GroupElements(pairs);

    // 4. 첫 번째 요소(큰 값) 기준 정렬. 그룹이 2개 이하일 때, pair의 첫 번째 요소(큰 값)들을 기준으로 삽입 정렬.
	// 첫 번째 요소가 이동할 때, 두 번째 요소도 함께 이동해야 함 -> 아직 모름.
    // 예) (2, 1), (4, 3) -> (4, 3), (2, 1)
	// 예2) (8, 6), (7, 3) -> (7, 3), (8, 6)
	sortPairsByFirstElement(pairs);
	
	// 5. pair의 두 번째 요소들을 첫 번째 요소들에 삽입. 두 번째 요소들을 첫 번째 요소에 삽입하는 과정에서 야콥스탈 수(Jacobsthal numbers)를 기준으로 이진 탐색
    // 예) (4, 3), (2, 1)에서 두 번째 요소인 3과 1을 이미 정렬된 첫 번째 요소보다 작은 것(3은 4)(1은 2)까지만 비교하여 삽입. 최종 결과: 1 2 3 4
	insertSecondElement(pairs);
	// insertSecondElement() 이 함수 안에서
	// 5-1. 야콥스탈 수 계산 calculateJacobsthalNumbers();
	// => J(n) = J(n-1) + 2J(n-2) + 1 (n > 1)
	// 5-2. 야콥 스탈 수를 이용한 이진 탐색으로 삽입 위치 찾기
	// 5-3. 찾은 위치에 삽입

	// 6. 정렬된 pair를 원래 배열에 적용
	applySortedPairs();
}

// 1. indexPairs 초기화. pair<int(값), int(인덱스)>
void PMergeMe::initIndexPairs() {
	for (size_t i = 0; i < arr.size(); ++i) {
		indexPairs.push_back(std::make_pair(arr[i], i));
	}
}

// 2. pairs 초기화. pair<int(큰 수), int(작은 수)>
void PMergeMe::initPairs() {
	for (size_t i = 0; i < arr.size(); i += 2) {
		if (i + 1 < arr.size()) {
			pairs.push_back(std::make_pair(std::max(arr[i], arr[i + 1]), std::min(arr[i], arr[i + 1])));
    		std::cout << "짝" << i / 2 << ": (" << arr[i] << ", " << arr[i + 1] << ")\n";
		} else {
			pairs.push_back(std::make_pair(arr[i], -1)); // 홀수면 마지막 원소 -1로 설정
			std::cout << "짝" << i / 2 << ": (" << arr[i] << ", -1)\n";
		}
	}
}

// 3. 요소 그룹화. 첫 번째 요소들을 짝지어 pair(큰 수, 작은 수)의 개수가 2개 이하가 될 때까지 그룹화(재귀)
// 홀수일 경우, 마지막 요소는 pair의 두 번째 요소를 -1로 설정
void PMergeMe::GroupElements(std::vector<std::pair<int, int> >& currentPairs) {
	if (currentPairs.size() <= 2) {
		sortPairsByFirstElement(currentPairs);
		return;
	}

	// 2개 이상일 때, pair의 첫 번째 원소들을 기준으로 삽입 정렬
	// 첫 번째 원소가 이동할 때, 두 번째 원소도 함께 이동
	std::vector<std::pair<int, int> > newPairs;
	for (size_t i = 0; i < currentPairs.size(); i += 2) {
		if (i + 1 < currentPairs.size()) {
            newPairs.push_back(std::make_pair(std::max(currentPairs[i].first, currentPairs[i + 1].first), std::min(currentPairs[i].second, currentPairs[i + 1].second)));
		} else {
			newPairs.push_back(currentPairs[i]); // 홀수 개의 그룹 처리
		}
	}

	for (std::vector<std::pair<int, int> >::iterator it = newPairs.begin(); it != newPairs.end(); ++it) {
		std::cout << "그룹(재귀): (" << it->first << ", " << it->second << ")\n";
	}

	GroupElements(newPairs);
}

// 4. 첫 번째 요소 기준 정렬. 그룹이 2개 이하일 때, pair의 첫 번째 요소들을 기준으로 삽입 정렬.
// 첫 번째 요소가 이동할 때, 두 번째 요소도 함께 이동해야 함 -> 아직 모름.
void PMergeMe::sortPairsByFirstElement(std::vector<std::pair<int, int> >& pairs) {
	for (size_t i = 1; i < pairs.size(); ++i) {
        std::pair<int, int> key = pairs[i];
        int j = i - 1;

        // key의 첫 번째 원소를 기준으로 삽입 정렬 수행
        while (j >= 0 && pairs[j].first > key.first) {
            pairs[j + 1] = pairs[j];
            j = j - 1;
        }
        pairs[j + 1] = key;
    }
}

// 5. 두 번째 요소들을 첫 번째 요소에 삽입. 두 번째 요소들을 첫 번째 요소에 삽입하는 과정에서 야콥스탈 수(Jacobsthal numbers)를 기준으로 이진 탐색. 야콥스탈 수를 사용하는 이유는 아래 예)에 있음.
// 예) (4, 3), (2, 1)에서 두 번째 원소인 3과 1을 이미 정렬된 첫 번째 원소보다 작은 것(3 -> 4)(1 -> 2)까지만, 그리고 야콥 스탈 수를 아이디어로 비교하여 삽입. 최종 결과: 1 2 3 4
// J(n) = J(n-1) + 2J(n-2) + 1 (n > 1) 
// void PMergeMe::insertSecondElement(std::vector<std::pair<int, int> >& sortedArr) {
//     // 1. 야콥스탈 수열 생성
//     std::vector<int> jacobsthalNumbers = calculateJacobsthalNumbers(sortedArr.size());

//     // 가정: sortedArr는 이미 정렬된 상태이며, 여기에 새로운 요소를 삽입하려고 함.
// 	// insertPos는 삽입할 위치를 나타내는 변수이며, 초기값은 적절히 설정해야 함.
// 	int insertPos = 0; // 삽입 위치 초기화

// 	for (size_t i = 0; i < pairs.size(); ++i) {
// 		if (pairs[i].second != -1) {
// 			int target = pairs[i].second; // 삽입하려는 값
// 			int insertPos = -1; // 삽입 위치 초기화
// 			int stepIndex = jacobsthalNumbers.size() - 1; // 가장 큰 야콥스탈 수부터 시작
// 			int pos = 0; // 탐색 시작 위치
// 			while (stepIndex >= 0 && pos < sortedArr.size()) {
// 				int step = jacobsthalNumbers[stepIndex]; // 현재 단계에서의 건너뛸 요소 수
// 				if (pos + step < sortedArr.size() && sortedArr[pos + step].first <= target) {
// 					// 현재 위치 + 건너뛸 요소 수가 삽입할 값보다 작거나 같으면, 거기서부터 다시 탐색 시작
// 					pos += step;
// 				} else {
// 					// 삽입할 값보다 큰 요소를 찾았거나, 더 이상 건너뛸 수 없으면 다음 단계로
// 					stepIndex--;
// 				}
// 			}
// 			insertPos = pos; // 삽입 위치 결정

// 			// 야콥스탈 수열을 이용해 삽입 위치 결정
			
// 			// 여기서는 단순화를 위해 실제 야콥스탈 수열을 사용하는 로직 대신, 삽입 위치를 찾는 기본적인 아이디어만 제공
// 			// 실제로는 야콥스탈 수열의 값을 이용해 삽입 위치를 결정하는 복잡한 로직이 필요함

// 			// 예시: target 값을 적절한 위치에 삽입하는 로직 구현
// 			// 이 부분은 실제 야콥스탈 수열을 이용한 삽입 로직에 따라 달라질 수 있음
// 			for (size_t j = 0; j < sortedArr.size(); ++j) {
// 				if (sortedArr[j].first > target) {
// 					insertPos = j;
// 					break;
// 				}
// 			}

// 			// 찾은 위치에 삽입
// 			if (insertPos != -1) {
// 				sortedArr.insert(sortedArr.begin() + insertPos, std::make_pair(target, -1));
// 			}
// 		}
//     }
// }

void PMergeMe::insertSecondElement(std::vector<std::pair<int, int> >& sortedArr) {
    // 1. 야콥스탈 수열 생성. 배열의 크기보다 작거나 같은 가장 큰 야콥스탈 수를 찾음.
    std::vector<int> jacobsthalNumbers = calculateJacobsthalNumbers(10);
	std::cout << "sortedArr.size(): " << sortedArr.size() << std::endl;
	std::cout << "Jacobsthal Numbers: ";
    for (std::vector<int>::iterator it = jacobsthalNumbers.begin(); it != jacobsthalNumbers.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    for (size_t i = 0; i < pairs.size(); ++i) {
        if (pairs[i].second != -1) {
            int target = pairs[i].second; // 삽입하려는 값
            int insertPos = -1; // 삽입 위치 초기화
            int stepIndex = jacobsthalNumbers.size() - 1; // 가장 큰 야콥스탈 수부터 시작
            size_t pos = 0; // 탐색 시작 위치

            while (stepIndex >= 0 && pos < sortedArr.size()) {
                int step = jacobsthalNumbers[stepIndex]; // 현재 단계에서의 건너뛸 요소 수
                if (pos + step < sortedArr.size() && sortedArr[pos + step].first <= target) {
                    // 현재 위치 + 건너뛸 요소 수가 삽입할 값보다 작거나 같으면, 거기서부터 다시 탐색 시작
                    pos += step;
                } else {
                    // 삽입할 값보다 큰 요소를 찾았거나, 더 이상 건너뛸 수 없으면 다음 단계로
                    stepIndex--;
                }
            }
            insertPos = pos; // 대략적인 삽입 위치 결정

            // // 선형 탐색을 통해 실제 삽입 위치 결정
            // while (insertPos < sortedArr.size() && sortedArr[insertPos].first < target) {
            //     insertPos++;
            // }

            // 찾은 위치에 삽입
            sortedArr.insert(sortedArr.begin() + insertPos, std::make_pair(target, -1));
        }
    }
}

// 5-1. 야콥 스탈 수 계산
// 각 항이 이전 항과 그 이전 항의 두 배에 1을 더한 값. 0번째 항은 0, 1번째 항은 1
// 0 1 2 5 11 23 ..
std::vector<int> PMergeMe::calculateJacobsthalNumbers(int n) {
    std::vector<int> jacobsthal(n + 1, 0); // n+1개의 항을 갖는 벡터 생성
    jacobsthal[0] = 0; // 첫 번째 항 초기화
    jacobsthal[1] = 1; // 두 번째 항 초기화
    for (int i = 2; i <= n; ++i) { // 세 번째 항부터 시작
        jacobsthal[i] = jacobsthal[i-1] + 2 * jacobsthal[i-2] + 1; // 야콥스탈 수열의 정의에 따라 각 항 계산
    }
    return jacobsthal;
}

// 6. 정렬된 pairs를 원래 배열에 적용
void PMergeMe::applySortedPairs() {
    std::vector<int> tempArr;
    for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
        tempArr.push_back(it->first); // 첫 번째 원소 삽입
        if (it->second != -1) {
            tempArr.push_back(it->second); // 두 번째 원소가 유효하면 삽입
        }
    }

    arr = tempArr; // 임시 배열의 내용을 원래 배열 arr에 복사
}

void PMergeMe::printSequence() const {
	for (std::vector<int>::const_iterator it = arr.begin(); it != arr.end(); ++it) {
            std::cout << *it << " ";
	}
	std::cout << std::endl;
}

PMergeMe::PMergeMe() {};

PMergeMe::PMergeMe(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        int num = std::atoi(argv[i]);
        if (num <= 0) {
            throw std::invalid_argument("Error: Only positive integers are allowed.\n");
        }
        arr.push_back(num);
    }

    if (arr.empty()) {
        throw std::invalid_argument("Error: The sequence cannot be empty.\n");
    }

    if (arr.size() <= 2) {
        throw std::invalid_argument("Error: The sequence must contain more than two elements.\n");
    }
};

PMergeMe::PMergeMe(const PMergeMe& other) {
	if (this != &other) {
		this->arr = other.arr;
	}
}

PMergeMe& PMergeMe::operator=(const PMergeMe& other) {
	if (this != &other) {
		this->arr = other.arr;
	}
	return *this;
}

PMergeMe::~PMergeMe() {};

std::vector<int> PMergeMe::getSequence() const {
	return this->arr;
}

// // 삽입 정렬
// void PMergeMeinsertionSort(vector<int>& arr) {
//     for (int i = 1; i < arr.size(); i++) {
//         int key = ar[i];
//         int j = i - 1;
//         while (j >= 0 && arr[j] > key) {
//             arr[j + 1] = arr[j];
//             j = j - 1;
//         }
//         arr[j + 1] = key;
//     }
// }

// // 이진 탐색
// int PMergeMe::binarySearch(const std::vector<int>& arr, int item, int low, int high) {
//     while (low <= high) {
//         int mid = low + (high - low) / 2;
//         if (arr[mid] == item)
//             return mid;
//         else if (arr[mid] < item)
//             low = mid + 1;
//         else
//             high = mid - 1;
//     }
//     return low;
// }