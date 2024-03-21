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
	// std::cout << "GroupElements 이후 pairs 상태:" << std::endl;
	// for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
	// 	std::cout << "(" << it->first << ", " << it->second << "), ";
	// }
	// std::cout << std::endl;

	// 3.5 작은 배열 중 첫 번째 요소는 큰 배열의 첫 번째 요소보다 작으니까 먼저 삽입
	preprocessFirstPair(pairs);
	// std::cout << "preprocessFirstPair 이후 pairs 상태:" << std::endl;
	// for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
	// 	std::cout << "(" << it->first << ", " << it->second << "), ";
	// }
	// std::cout << std::endl;

    // 4. 첫 번째 요소(큰 값) 기준 정렬. 그룹이 2개 이하일 때, pair의 첫 번째 요소(큰 값)들을 기준으로 삽입 정렬.
	// 첫 번째 요소가 이동할 때, 두 번째 요소도 함께 이동해야 함 -> 아직 모름.
    // 예) (2, 1), (4, 3) -> (4, 3), (2, 1)
	// 예2) (8, 6), (7, 3) -> (7, 3), (8, 6)
	sortPairsByFirstElement(pairs);
	// std::cout << "sortPairsByFirstElement 이후 pairs 상태:" << std::endl;
	// for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
	// 	std::cout << "(" << it->first << ", " << it->second << "), ";
	// }
	// std::cout << std::endl;

	// 5. pair의 두 번째 요소들을 첫 번째 요소들에 삽입. 두 번째 요소들을 첫 번째 요소에 삽입하는 과정에서 야콥스탈 수(Jacobsthal numbers)를 기준으로 이진 탐색
    // 예) (4, 3), (2, 1)에서 두 번째 요소인 3과 1을 이미 정렬된 첫 번째 요소보다 작은 것(3은 4)(1은 2)까지만 비교하여 삽입. 최종 결과: 1 2 3 4
	insertSecondElement(pairs);
	// std::cout << "insertSecondElement 이후 pairs 상태:" << std::endl;
	// for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
	// 	std::cout << "(" << it->first << ", " << it->second << "), ";
	// }
	// std::cout << std::endl;

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
    		// std::cout << "짝" << i / 2 << ": (" << arr[i] << ", " << arr[i + 1] << ")\n";
		} else {
			pairs.push_back(std::make_pair(arr[i], -1)); // 홀수면 마지막 원소 -1로 설정
			// std::cout << "짝" << i / 2 << ": (" << arr[i] << ", -1)\n";
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
		// std::cout << "그룹(재귀): (" << it->first << ", " << it->second << ")\n";
	}

	GroupElements(newPairs);
}

// 3.5 작은 배열 중 첫 번째 요소는 큰 배열의 첫 번째 요소보다 작으니까 먼저 삽입
void PMergeMe::preprocessFirstPair(std::vector<std::pair<int, int> >& pairs) {
    if (pairs.size() > 1 && pairs[0].second < pairs[1].first) {
        std::pair<int, int> firstPair = std::make_pair(pairs[0].second, -1);
        pairs.insert(pairs.begin(), firstPair);
        pairs[1].second = -1;
    }
}

// 4. 첫 번째 요소 기준 정렬. 그룹이 2개 이하일 때, pair의 첫 번째 요소들을 기준으로 삽입 정렬.
// 첫 번째 요소가 이동할 때, 두 번째 요소도 함께 이동해야 함 -> 아직 모름.
void PMergeMe::sortPairsByFirstElement(std::vector<std::pair<int, int> >& pairs) {
    for (size_t i = 1; i < pairs.size(); i++) {
        std::pair<int, int> key = pairs[i];
        int j = i - 1;

        // binarySearch를 사용해 삽입할 위치 찾기
        int loc = binarySearch(pairs, key, 0, j);

        // 찾은 위치까지 모든 요소를 한 칸씩 뒤로 옮기기
        while (j >= loc) {
            pairs[j + 1] = pairs[j];
            j--;
        }
        pairs[j + 1] = key;
    }
}

// 이진 탐색
// 해석: pairs 배열에서 target을 찾아 삽입할 위치를 반환
int PMergeMe::binarySearch(const std::vector<std::pair<int, int> >& pairs, const std::pair<int, int>& target, int start, int end) {
    while (start <= end) {
        int mid = start + (end - start) / 2;
		// 큰 수가 mid와 같으면 작은 수를 비교해서 삽입 위치 찾기
        if (pairs[mid].first == target.first && pairs[mid].second == target.second) {
            return mid;
		// 큰 수가 작거나 같으면 오른쪽으로 이동
        } else if (pairs[mid].first < target.first || (pairs[mid].first == target.first && pairs[mid].second < target.second)) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return start;
}

// 5. pair의 두 번째 요소들을 첫 번째 요소들에 삽입. 두 번째 요소들을 첫 번째 요소에 삽입하는 과정에서 야콥스탈 수(Jacobsthal numbers)를 기준으로 이진 탐색
void PMergeMe::insertSecondElement(std::vector<std::pair<int, int> >& sortedArr) {
    for (size_t i = 0; i < sortedArr.size(); ++i) {
        if (sortedArr[i].second != -1 && sortedArr[i].second < sortedArr[i].first) {
            int target = sortedArr[i].second;
            std::pair<int, int> targetPair = std::make_pair(target, -1);
            int insertPos = binarySearch(sortedArr, targetPair, 0, i);

            // 삽입할 위치 바로 뒤에 새로운 원소를 추가
            sortedArr.insert(sortedArr.begin() + insertPos, targetPair);

            // 원래의 pair에서 두 번째 요소를 -1로 변경
            sortedArr[i + 1].second = -1;
        }
    }
}

// // 5-3. 찾은 위치에 삽입
// void PMergeMe::insertElementInCorrectPosition(int insertPos, int target, std::vector<std::pair<int, int> >& sortedArr) {
//     sortedArr.insert(sortedArr.begin() + insertPos, std::make_pair(target, -1));
// }

// 5-1. 야콥 스탈 수 계산
// 각 항이 이전 항과 그 이전 항의 두 배에 1을 더한 값. 0번째 항은 0, 1번째 항은 1
// 0 1 1 3 5 11 23 ..
std::vector<int> PMergeMe::calculateJacobsthalNumbers(int n) {
    std::vector<int> jacobsthalNumbers;
    int j0 = 0, j1 = 1;
    jacobsthalNumbers.push_back(j0);
    jacobsthalNumbers.push_back(j1);

    // 배열의 크기보다 작거나 같은 가장 큰 야콥스탈 수를 찾을 때까지 계산
    while (true) {
        int next = j1 + 2 * j0;
        if (next > n) break;
        jacobsthalNumbers.push_back(next);
        j0 = j1;
        j1 = next;
    }

	jacobsthalNumbers.erase(jacobsthalNumbers.begin());
    return jacobsthalNumbers;
}

// // 5-2. 야콥 스탈 수를 이용한 이진 탐색으로 삽입 위치 찾기
// int PMergeMe::binarySearchWithJacobsthal(int target, int start, int end, std::vector<int>& jacobsthalNumbers, std::vector<std::pair<int, int> >& sortedArr) {
//     int jacobsthalIndex = jacobsthalNumbers.size() - 1;

//     while (start <= end) {
//         int mid = (start + end) / 2;

//         // mid 위치에 가장 가까운 야콥스탈 수를 더함
//         while (jacobsthalIndex >= 0 && mid + jacobsthalNumbers[jacobsthalIndex] <= end) {
//             mid += jacobsthalNumbers[jacobsthalIndex];
//             jacobsthalIndex--;
//         }

//         // mid와 target을 비교
//         if (sortedArr[mid].first == target) {
//             return mid;
//         } else if (sortedArr[mid].first < target) {
//             start = mid + 1;
//         } else {
//             end = mid - 1;
//         }
//     }

//     // target이 삽입되어야 할 위치를 반환
//     return start;
// }

// 6. 정렬된 pairs를 원래 배열에 적용
// [(3, 0), (1, 1), (2, 2)]
// [(1, 1), (2, 2), (3, 0)]
// [1, 2, 3]
// Index[1, 2, 0]
void PMergeMe::applySortedPairs() {
    arr.clear();
    for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
        arr.push_back(it->first);
    }
}

void PMergeMe::display() const {
	for (std::vector<int>::const_iterator it = arr.begin(); it != arr.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

PMergeMe::PMergeMe() {}

void PMergeMe::initializeArray(int argc, char** argv) {
    std::vector<int> tempArr;
    for (int i = 1; i < argc; ++i) {
        int num = std::atoi(argv[i]);
        if (num <= 0) {
            throw std::invalid_argument("Error: Only positive integers are allowed.\n");
        }
        if (std::find(tempArr.begin(), tempArr.end(), num) != tempArr.end()) {
            throw std::invalid_argument("Error: Duplicate elements are not allowed.\n");
        }
        tempArr.push_back(num);
    }

    arr = tempArr;

    if (arr.size() <= 2) {
        throw std::invalid_argument("Error: The sequence must contain more than two elements.\n");
    }
    
    // jacobsthalNumbers = calculateJacobsthalNumbers(arr.size() + 1);
}

void PMergeMe::initializeArrayDeque(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        int num = std::atoi(argv[i]);
        dequeArr.push_back(num);
    }

    if (dequeArr.size() <= 2) {
        throw std::invalid_argument("Error: The sequence must contain more than two elements.\n");
    }
    
    // jacobsthalNumbersDeque = calculateJacobsthalNumbersDeque(dequeArr.size() + 1);
}

PMergeMe::PMergeMe(int argc, char** argv) {
    initializeArray(argc, argv);
	initializeArrayDeque(argc, argv);
}

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

PMergeMe::~PMergeMe() {}

std::vector<int> PMergeMe::getSequence() const {
	return this->arr;
}

// -----------------------------------------------------Dequeue----------------------------------------------------- //

void PMergeMe::dequeSort() {
    initIndexPairsDeque();
    initPairsDeque();
    GroupElements(dequePairs);
    preprocessFirstPair(dequePairs);
    sortPairsByFirstElement(dequePairs);
    insertSecondElement(dequePairs);
    applySortedPairsDeque();
}

void PMergeMe::initIndexPairsDeque() {
    for (size_t i = 0; i < dequeArr.size(); ++i) {
        dequeIndexPairs.push_back(std::make_pair(dequeArr[i], i));
    }
}

void PMergeMe::initPairsDeque() {
    for (size_t i = 0; i < dequeArr.size(); i += 2) {
        if (i + 1 < dequeArr.size()) {
            dequePairs.push_back(std::make_pair(std::max(dequeArr[i], dequeArr[i + 1]), std::min(dequeArr[i], dequeArr[i + 1])));
        } else {
            dequePairs.push_back(std::make_pair(dequeArr[i], -1));
        }
    }
}

void PMergeMe::GroupElements(std::deque<std::pair<int, int> >& currentPairs) {
    if (currentPairs.size() <= 2) {
        sortPairsByFirstElement(currentPairs);
        return;
    }

    std::deque<std::pair<int, int> > newPairs;
    for (size_t i = 0; i < currentPairs.size(); i += 2) {
        if (i + 1 < currentPairs.size()) {
            newPairs.push_back(std::make_pair(std::max(currentPairs[i].first, currentPairs[i + 1].first), std::min(currentPairs[i].second, currentPairs[i + 1].second)));
        } else {
            newPairs.push_back(currentPairs[i]);
        }
    }

    GroupElements(newPairs);
}

void PMergeMe::preprocessFirstPair(std::deque<std::pair<int, int> >& pairs) {
    if (pairs.size() > 1 && pairs[0].second < pairs[1].first) {
        std::pair<int, int> firstPair = std::make_pair(pairs[0].second, -1);
        pairs.push_front(firstPair);
        pairs[1].second = -1;
    }
}

void PMergeMe::sortPairsByFirstElement(std::deque<std::pair<int, int> >& pairs) {
    for (size_t i = 1; i < pairs.size(); i++) {
        std::pair<int, int> key = pairs[i];
        int j = i - 1;

        // binarySearch를 사용해 삽입할 위치 찾기
        int loc = binarySearch(pairs, key, 0, j);

        // 찾은 위치까지 모든 요소를 한 칸씩 뒤로 옮기기
        while (j >= loc) {
            pairs[j + 1] = pairs[j];
            j--;
        }
        pairs[j + 1] = key;
    }
}
int PMergeMe::binarySearch(const std::deque<std::pair<int, int> >& pairs, const std::pair<int, int>& target, int start, int end) {
    while (start <= end) {
        int mid = start + (end - start) / 2;
        if (pairs[mid].first == target.first && pairs[mid].second == target.second) {
            return mid;
        } else if (pairs[mid].first < target.first || (pairs[mid].first == target.first && pairs[mid].second < target.second)) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return start;
}

void PMergeMe::insertSecondElement(std::deque<std::pair<int, int> >& sortedArr) {
    for (size_t i = 0; i < sortedArr.size(); ++i) {
        if (sortedArr[i].second != -1 && sortedArr[i].second < sortedArr[i].first) {
            int target = sortedArr[i].second;
            std::pair<int, int> targetPair = std::make_pair(target, -1);
            int insertPos = binarySearch(sortedArr, targetPair, 0, i);

            // 삽입할 위치 바로 뒤에 새로운 원소를 추가
            sortedArr.insert(sortedArr.begin() + insertPos, targetPair);

            // 원래의 pair에서 두 번째 요소를 -1로 변경
            sortedArr[i + 1].second = -1;
        }
    }
}

void PMergeMe::applySortedPairsDeque() {
    dequeArr.clear();
    for (std::deque<std::pair<int, int> >::iterator it = dequePairs.begin(); it != dequePairs.end(); ++it) {
        dequeArr.push_back(it->first);
    }
}

void PMergeMe::dequeDisplay() const {
    for (std::deque<int>::const_iterator it = dequeArr.begin(); it != dequeArr.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

std::deque<int> PMergeMe::getSequenceDeque() const {
    return this->dequeArr;
}