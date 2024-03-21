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
	std::cout << "GroupElements 이후 pairs 상태:" << std::endl;
	for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
		std::cout << "(" << it->first << ", " << it->second << "), ";
	}
	std::cout << std::endl;

	// 3.5 작은 배열 중 첫 번째 요소는 큰 배열의 첫 번째 요소보다 작으니까 먼저 삽입
	preprocessFirstPair(pairs);
	std::cout << "preprocessFirstPair 이후 pairs 상태:" << std::endl;
	for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
		std::cout << "(" << it->first << ", " << it->second << "), ";
	}
	std::cout << std::endl;

    // 4. 첫 번째 요소(큰 값) 기준 정렬. 그룹이 2개 이하일 때, pair의 첫 번째 요소(큰 값)들을 기준으로 삽입 정렬.
	// 첫 번째 요소가 이동할 때, 두 번째 요소도 함께 이동해야 함 -> 아직 모름.
    // 예) (2, 1), (4, 3) -> (4, 3), (2, 1)
	// 예2) (8, 6), (7, 3) -> (7, 3), (8, 6)
	sortPairsByFirstElement(pairs);
	
	std::cout << "sortPairsByFirstElement 이후 pairs 상태:" << std::endl;
	for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
		std::cout << "(" << it->first << ", " << it->second << "), ";
	}
	std::cout << std::endl;

	// 5. pair의 두 번째 요소들을 첫 번째 요소들에 삽입. 두 번째 요소들을 첫 번째 요소에 삽입하는 과정에서 야콥스탈 수(Jacobsthal numbers)를 기준으로 이진 탐색
    // 예) (4, 3), (2, 1)에서 두 번째 요소인 3과 1을 이미 정렬된 첫 번째 요소보다 작은 것(3은 4)(1은 2)까지만 비교하여 삽입. 최종 결과: 1 2 3 4
	insertSecondElement(pairs);
	std::cout << "insertSecondElement 이후 pairs 상태:" << std::endl;
	for (std::vector<std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
		std::cout << "(" << it->first << ", " << it->second << "), ";
	}
	std::cout << std::endl;
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

// 3.5 작은 배열 중 첫 번째 요소는 큰 배열의 첫 번째 요소보다 작으니까 먼저 삽입
void PMergeMe::preprocessFirstPair(std::vector<std::pair<int, int> >& pairs) {
    if (!pairs.empty() && pairs[0].second != -1) {
        // pair[0].second를 새로운 pair로 만들어 배열의 시작 부분에 삽입
        pairs.insert(pairs.begin(), std::make_pair(pairs[0].second, -1));
        // 원래의 쌍에서 두 번째 값을 제거
        pairs[1].second = -1;
    }
}

// 4. 첫 번째 요소 기준 정렬. 그룹이 2개 이하일 때, pair의 첫 번째 요소들을 기준으로 삽입 정렬.
// 첫 번째 요소가 이동할 때, 두 번째 요소도 함께 이동해야 함 -> 아직 모름.
void PMergeMe::sortPairsByFirstElement(std::vector<std::pair<int, int> >& pairs) {
    mergeSort(pairs, 0, pairs.size() - 1);
}

void PMergeMe::merge(std::vector<std::pair<int, int> >& pairs, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::pair<int, int> > L(n1), R(n2);

    for (i = 0; i < n1; i++)
        L[i] = pairs[left + i];
    for (j = 0; j < n2; j++)
        R[j] = pairs[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i].first <= R[j].first) {
            pairs[k] = L[i];
            i++;
        } else {
            pairs[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        pairs[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        pairs[k] = R[j];
        j++;
        k++;
    }
}

void PMergeMe::mergeSort(std::vector<std::pair<int, int> >& pairs, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(pairs, left, mid);
        mergeSort(pairs, mid + 1, right);

        merge(pairs, left, mid, right);
    }
}

// // 두 번째 요소를 찾는 함수
// std::vector<std::pair<int, int> >::iterator PMergeMe::findSecondElement(int target, std::vector<std::pair<int, int> >& sortedArr) {
//     return std::find(sortedArr.begin(), sortedArr.end(), std::make_pair(target, -1));
// }

// // 두 번째 요소를 적절한 위치에 삽입하는 함수
// void PMergeMe::insertElementInCorrectPosition(std::vector<std::pair<int, int> >::iterator it, int target, std::vector<std::pair<int, int> >& sortedArr) {
//     if (it == sortedArr.end()) {
//         // 삽입할 요소를 sortedArr의 끝에 추가
//         sortedArr.push_back(std::make_pair(target, -1));
//         // 삽입된 요소를 올바른 위치로 이동
// 		// 여기서 야콥스탈 수를 사용해보자.
//         for (int j = sortedArr.size() - 1; j > 0 && sortedArr[j].first < sortedArr[j - 1].first; --j) {
//             std::swap(sortedArr[j], sortedArr[j - 1]);
//         }
//     }
// }

// // 두 번째 요소를 첫 번째 요소에 삽입하는 함수
// void PMergeMe::insertSecondElement(std::vector<std::pair<int, int> >& sortedArr) {
//     // 1. 야콥스탈 수열 생성. 배열의 크기보다 작거나 같은 가장 큰 야콥스탈 수 까지 넣음.
//     std::vector<int> jacobsthalNumbers = calculateJacobsthalNumbers(sortedArr.size());

//     for (size_t i = 0; i < pairs.size(); ++i) {
//         if (pairs[i].first != -1 && pairs[i].second != -1) {
//             int target = pairs[i].second;
//             std::vector<std::pair<int, int> >::iterator it = findSecondElement(target, sortedArr); //target을 찾고
//             insertElementInCorrectPosition(it, target, sortedArr); //target이 없으면 삽입
//         }
//     }
// }

// 두 번째 요소를 적절한 위치에 삽입하는 함수
void PMergeMe::insertElementInCorrectPosition(int target, std::vector<std::pair<int, int> >& sortedArr, std::vector<int>& jacobsthalNumbers) {
    // 야콥스탈 수열을 이용하여 삽입할 위치를 찾음
    int position = binarySearchWithJacobsthal(target, 0, sortedArr.size() - 1, jacobsthalNumbers, sortedArr);
	// 삽입할 위치에 새 요소를 삽입
    sortedArr.insert(sortedArr.begin() + position, std::make_pair(target, -1));
}

#include <thread>
// 두 번째 요소를 첫 번째 요소에 삽입하는 함수
void PMergeMe::insertSecondElement(std::vector<std::pair<int, int> >& sortedArr) {
    // 1. 야콥스탈 수열 생성. 배열의 크기보다 작거나 같은 가장 큰 야콥스탈 수 까지 넣음.
    std::vector<int> jacobsthalNumbers = calculateJacobsthalNumbers(sortedArr.size());

    for (size_t i = 0; i < pairs.size(); ++i) {
        if (pairs[i].first != -1 && pairs[i].second != -1) {
            int target = pairs[i].second;
            // 야콥스탈 수열을 이용하여 삽입할 위치를 찾고 삽입
            insertElementInCorrectPosition(target, sortedArr, jacobsthalNumbers);
			std::cout << "123무한루프 target: " << target << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

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

    return jacobsthalNumbers;
}

// // 이진 탐색
// int PMergeMe::binarySearch(int target, int start, int end) {
//     while (start <= end) {
//         int mid = start + (end - start) / 2;
//         if (sortedArr[mid].first == target) {
//             return mid;
//         } else if (sortedArr[mid].first < target) {
//             start = mid + 1;
//         } else {
//             end = mid - 1;
//         }
//     }
//     return start;
// }

// 야콥스탈 수를 이용하여 이진 탐색
int PMergeMe::binarySearchWithJacobsthal(int target, int start, int end, std::vector<int>& jacobsthalNumbers, std::vector<std::pair<int, int> >& sortedArr) {
    while (start <= end) {
        // 야콥스탈 수열을 사용하여 mid를 계산
        int mid = start + jacobsthalNumbers.back();
        // mid가 범위를 벗어나면 야콥스탈 수열에서 마지막 요소를 제거
        if (mid > end) {
            jacobsthalNumbers.pop_back();
            continue;
        }

        // mid와 target을 비교
        if (sortedArr[mid].first == target) {
            // target을 찾으면 mid를 반환
            return mid;
        } else if (sortedArr[mid].first < target) {
            // mid가 target보다 작으면 시작 인덱스를 mid+1로 업데이트
            start = mid + 1;
        } else {
            // mid가 target보다 크면 끝 인덱스를 mid-1로 업데이트
            end = mid - 1;
        }

        // 야콥스탈 수열에서 마지막 요소를 제거
        jacobsthalNumbers.pop_back();
    }

    // target을 찾지 못하면 -1을 반환
    return -1;
}

// // 5-2. 야콥 스탈 수를 이용한 이진 탐색으로 삽입 위치 찾기
// int PMergeMe::findInsertPosition(const std::vector<std::pair<int, int> >& sortedArr, int target, const std::vector<int>& jacobsthalNumbers) {
//     int pos = sortedArr.size() - 1;
//     for (int stepIndex = jacobsthalNumbers.size() - 1; stepIndex >= 0; --stepIndex) {
//         int newPos = pos - jacobsthalNumbers[stepIndex];
//         while (newPos >= 0 && sortedArr[newPos].first > target) {
//             pos = newPos;
//             // std::cout << "newPos: " << newPos << std::endl;
//             newPos--;
//         }
//         // std::cout << "stepIndex: " << stepIndex << std::endl;
//     }
//     // std::cout << "pos: " << pos << std::endl;
//     return pos + 1; // 삽입할 위치는 찾은 위치의 다음 위치
// }

// 6. 정렬된 pairs를 원래 배열에 적용
// [(3, 0), (1, 1), (2, 2)]
// [(1, 1), (2, 2), (3, 0)]
// [1, 2, 3]
// Index[1, 2, 0]
void PMergeMe::applySortedPairs() {
    for (std::vector<std::pair<int, int> >::iterator it = indexPairs.begin(); it != indexPairs.end(); ++it) {
        arr[it->second] = it->first; // 인덱스에 해당하는 값 변경
    }
}

void PMergeMe::printSequence() const {
	for (std::vector<int>::const_iterator it = arr.begin(); it != arr.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

void PMergeMe::printSortedSequence() const {
    for (std::vector<std::pair<int, int> >::const_iterator it = pairs.begin(); it != pairs.end(); ++it) {
        std::cout << it->first << " ";
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
