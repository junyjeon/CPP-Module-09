#include "PMergeMe.hpp"

void PMergeMe::printSortedSequence() const {
    std::cout << "After: ";
	for (std::vector<int>::const_iterator it = arr.begin(); it != arr.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
}

void PMergeMe::sort() {
	// 1. pair<int(값), int(인덱스)>로 초기화 indexPairs
	// 예) 2 1 4 3 -> (2, 0), (1, 1), (4, 2), (3, 3)
	initIndexPairs();
	
	// 2. pair<int(큰 수), int(작은 수)> pairs 초기화
	// 예) 2 1 4 3 -> (2, 1), (4, 3)
	initPairs();
	
	// 3. 첫 번째 원소들을 짝지어 pair(큰 수, 작은 수)의 개수가 2개 이하가 될 때까지 그룹화(재귀) pair(큰 수(pair(큰 수(...), 작은 수)), 작은 수)
	// 예) 8 2 4 6 1 3 7 5 -> (8, 2), (6, 4), (3, 1), (7, 5) -> [(8, 2), (6, 4)]와 [(3, 1), (7, 5)] 비교 -> [(8, 6),  (7, 3)] -> [(8, 6)]과 [(7, 3)] 이 최종적으로 남음(2개)
	GroupElements(pairs);

    // 4. 2개 이하일 때, pair의 첫 번째 원소들을 기준으로 삽입 정렬. 첫 번째 원소가 이동할 때, 두 번째 원소도 함께 이동.
	// 예) 8 2 4 6 -> (4, 6), (8, 2)
	sortPairsByFirstElement(pairs);
	
	// 5. pair의 두 번째 원소들을 첫 번째 원소들에 삽입(이진 탐색)
	// insertSecondElement(pairs);

	// 6. 정렬된 pair를 원래 배열에 적용
	applySortedPairs();
	// std::sort(arr.begin(), arr.end());
}

// 1. pair<int(값), int(인덱스)> indexPairs 초기화
void PMergeMe::initIndexPairs() {
	for (size_t i = 0; i < arr.size(); ++i) {
		indexPairs.push_back(std::make_pair(arr[i], i));
	}
}

// 2. pair<int(큰 수), int(작은 수)> pairs 초기화
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

// 3. 첫 번째 원소들을 짝지어 pair(큰 수, 작은 수)의 개수가 2개 이하가 될 때까지 그룹화(재귀)
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

// 4. 2개 이하일 때, pair의 첫 번째 원소들을 기준으로 삽입 정렬. 첫 번째 원소가 이동할 때, 두 번째 원소도 함께 이동.
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

// 5. 각 쌍의 두 번째 원소를 첫 번째 원소에 삽입. 이때, 야콥 스타일 수(Jacobsthal numbers)를 기준으로 이진 탐색을 사용.
// 예) (4, 3), (2, 1)에서 두 번째 원소인 3과 1을 이미 정렬된 첫 번째 원소보다 작은 것(3 -> 4)(1 -> 2)까지만 비교하여 삽입. 최종 결과: 1 2 3 4
// J(n) = J(n-1) + 2J(n-2) + 1 (n > 1) 
void PMergeMe::insertSecondElement(std::vector<int>& sortedArr) {
	(void)sortedArr;
    // for (size_t i = 0; i < pairs.size(); ++i) {
    //     if (pairs[i].second != -1) { // 두 번째 원소가 -1이 아니면
    //         int j = i;
    //         while (j > 0 && pairs[j-1].first > pairs[i].second) {
    //             j--;
    //         }
    //         // j는 이제 pairs[i].second가 삽입될 위치
    //         // 실제 삽입은 배열 재구성 시 수행됨
    //     }
    // }
    // 이 함수는 예제를 단순화하기 위해 실제 삽입 로직을 생략함
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

PMergeMe::PMergeMe() {};

PMergeMe::PMergeMe(std::vector<int>& arr) : arr(arr) {};

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