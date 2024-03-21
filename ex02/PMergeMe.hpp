#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <ctime> // clock() 시간 측정
#include <vector> // std::vector
#include <set> // std::set
#include <iomanip> // std::fixed, std::setprecision
#include <cstdlib> // std::atoi
#include <algorithm> // std::swap
#include <utility> // std::pair
#include <deque> // std::deque

/*
포드 존슨 알고리즘을 구현. 정수 배열을 효율적으로 정렬하는 방법.
기본 아이디어는 '병합 삽입 정렬'을 개선해 비교 횟수를 줄임.

- 양의 정수 배열 -> 모든 요소가 0보다 큰 정수인 배열

Ford-Johnson
1. 배열을 2개의 부분 배열로 나눔 예) 5 3 2 1 4 6 7 8 -> 5 3 2 1, 4 6 7 8
2. 나눈 배열을 각각 정렬 예) 5 3 2 1 -> 1 2 3 5, 4 6 7 8 -> 4 6 7 8
3. 나눈 배열을 병합 예) 1 2 3 5, 4 6 7 8 -> 1 2 3 4 5 6 7 8

1. 배열을 2개의 작은, 큰 배열로 나눔 예) 5 3 4 1 2 8 7 6 -> 5 3 4 1, 2 8 7 6
2. 작은 배열 정렬(삽입 정렬) 예) 5 3 4 1 -> 1 3 4 5
3. 큰 배열 정렬(포드-존슨 알고리즘(재귀)) 예) 2 8 7 6 -> 2 6 7 8
4. 정렬된 작은 배열 -> 큰 배열에 삽입(이진 탐색) 예) 1 3 4 5, 2 6 7 8 -> 1 2 3 4 5 6 7 8

반으로 나눔 -> 작은 배열 정렬(삽입) -> 큰 배열 재귀정렬 -> 작은 배열을 큰 배열에 삽입(이진 탐색)

1. 반으로 나눔: 주어진 배열을 반으로 나눠서, 하나는 작은 배열, 다른 하나는 큰 배열로 설정해.
2. 작은 배열 정렬(삽입): 작은 배열을 삽입 정렬과 같은 간단한 정렬 방법으로 정렬해.
3. 큰 배열 재귀정렬: 큰 배열에 대해서는 포드-존슨 알고리즘을 재귀적으로 적용해서 정렬해. 이 과정에서 배열을 계속 나누고, 각 부분을 정렬한 다음, 병합하는 과정이 반복돼.
4. 작은 배열을 큰 배열에 삽입(이진 탐색): 정렬된 작은 배열의 요소들을 이진 탐색을 사용해 큰 배열에 적절한 위치에 삽입해. 이 과정을 통해 전체 배열이 정렬

에러처리:
시퀀스에는 양의 정수만 포함(음수, 문자열, 0 입력 에러 처리)
시퀀스가 비어있을 떄
시퀀스가 2개 이하일 때

try catch 사용
vector, set 사용

템플릿 사용을 피하기 위해, vector, set에 따라 알고리즘을 별도로 구현.

출력 형식:
Before: 정렬되지 않은 배열
After: 정렬된 배열
Time to process a range of n elements with std::[첫 번째 컨테이너] : 0.00000 us
Time to process a range of n elements with std::[두 번째 컨테이너] : 0.00000 us

main.cpp 구현:
1. argc가 2보다 작으면 사용법 출력 후 종료
2. 양의 정수 시퀀스 준비 (argv 사용) vector<int> sequence에 push_back
3. Before: 정렬되지 않은 배열
4. 시간 측정 ctime clock()
5. 포드존슨 알고리즘
6. 시간 측정 종료
7. After: 정렬된 배열
8. Time to process a range of n elements with std::[첫 번째 컨테이너] : 0.00000 us
9. Time to process a range of n elements with std::[두 번째 컨테이너] : 0.00000 us
10. return 0

포드-존슨 알고리즘 구현:
1. 요소 두개를 번갈아가며 vector<pair>로 짝지음 (홀수 개일 경우, 마지막 요소는 pair의 두 번째 요소를 -1로 설정)
2. 배열을 반으로 나누고(재귀), 요소가 2개 이하이면 정렬
3. 정렬된 요소의 index가 짝지어진 큰 배열에 영향이 감. -> 어떻게?
4. 작은 배열 정렬(삽입)
5. 큰 배열 재귀정렬
6. 작은 배열을 큰 배열에 삽입(이진 탐색)
7. 배열의 요소가 2개 이하일 때까지 재귀적으로 반복

0. vector<int> arr 받음
1. 2개씩 vector<pair<int(큰 수), int(작은 수)>> (홀수 개일 경우, 마지막 요소는 pair의 두 번째 요소를 -1로 설정)
2. 2개 이하가 될 때까지 반으로 나눔
3. 2개 일 때, pair의 첫 번째 요소끼리 비교 (v.size() < 2)
4. 2개 일 때의 정렬된 요소와 pair로 연결된 요소의 위치도 정렬 -> 어떻게?
5. 큰 배열에 삽입(이진 탐색) -> 야콥스탈 수로 계산하여 삽입해서 비교 횟수 줄임 -> 어떻게?

1. vector<pair<int(큰 수), int(작은 수)> >로 짝지음 예) 8 2 4 6 -> 8 2, 4 6 (8과 2를 pair로)
2. pair의 첫 번째 요소들을 삽입 정렬 예) 8 2 4 6 -> 4 2 8 6 (8과 4를 swap)
3. pair의 두 번째 요소들은 첫 번째 요소들의 index에 따라 swap 예) 4 2 8 6 -> 4 6 8 2 (비교 없음. 2와 6을 swap)
4. pair의 두 번째 요소들을 첫 번째 요소들에 삽입(이진 탐색) 예) 4 8, 2 6 -> 2 4 6 8 근데 pair의 두번째 요소인 2는 한 번 비교된 4보다 작아서 4까지만 비교하면 됨.

0. vector<int> arr 받음
1. vector<pair<int(큰 수), int(작은 수)> >로 짝지음 예) 8 2 4 6 -> (8, 2), (4, 6)
2. 2개 이하가 될 때까지 첫 번째 요소들을 짝지음
// 8 2 4 6 1 3 7 5 -> (8, 2), (6, 4), (3, 1), (7, 5) -> [(8, 2), (6, 4)]와 [(3, 1), (7, 5)] 비교 -> [(8, 6),  (7, 3)] -> [(8, 6)]과 [(7, 3)] 이 최종적으로 남음(2개)
2. pair의 첫 번째 요소들을 기준으로 삽입 정렬. 첫 번째 요소가 이동할 때 두 번째 요소도 함께 이동. 예) 8 2 4 6 -> (4, 6), (8, 2)
3. 각 쌍의 두 번째 요소를 첫 번째 요소에 삽입(야콥스탈 수(Jacobsthal numbers)를 기준으로 이진 탐색). 예) (4, 6), (8, 2)에서 두 번째 요소인 6과 2를 이미 정렬된 첫 번째 요소보다 작은 것까지만 비교하여 삽입. 2 4 6 8
J(n) = J(n-1) + 2J(n-2) + 1 (n > 1)

0. vector<int> arr 받음
1. vector<pair<int(값), int(인덱스)> >로 초기화
	예) 2 1 4 3 -> (2, 0), (1, 1), (4, 2), (3, 3)
2. 첫 번째 요소들을 짝지어 pair(큰 수, 작은 수)의 개수가 2개 이하가 될 때까지 그룹화(재귀)
	// 예) (값) 2 1 4 3 -> (2, 1), (4, 3)
	// 예) (index) 0 1 2 3 -> (0, 1), (2, 3)
	// 예) 8 2 4 6 1 3 7 5 -> (8, 2), (6, 4), (3, 1), (7, 5) -> [(8, 2), (6, 4)]와 [(3, 1), (7, 5)] 비교 -> [(8, 6), (7, 3)] -> 최종적으로 [(8, 6)]과 [(7, 3)]이 남음.
3. 2개 이하일 때, pair의 첫 번째 요소들을 기준으로 삽입 정렬. 첫 번째 요소가 이동할 때, 두 번째 요소도 함께 이동.
	예) (2, 1), (4, 3) -> (4, 3), (2, 1)
4. 각 쌍의 두 번째 요소를 첫 번째 요소에 삽입. 이때, 야콥스탈 수(Jacobsthal numbers)를 기준으로 이진 탐색을 사용.
	// 예) (4, 3), (2, 1)에서 두 번째 요소인 3과 1을 이미 정렬된 첫 번째 요소보다 작은 것(3 -> 4)(1 -> 2)까지만 비교하여 삽입. 최종 결과: 1 2 3 4

1. indexPairs 초기화. pair<int(값), int(인덱스)>
2. pairs 초기화. pair<int(큰 수), int(작은 수)>
3. 요소 그룹화. 첫 번째 요소들을 짝지어 pair(큰 수, 작은 수)의 개수가 2개 이하가 될 때까지 그룹화(재귀)
4. 첫 번째 요소 기준 정렬. 그룹이 2개 이하일 때, pair의 첫 번째 요소들을 기준으로 삽입 정렬.
5. 두 번째 요소들을 첫 번째 요소에 삽입. 이때, 야콥스탈 수(Jacobsthal numbers)를 기준으로 뒤에서부터(모름. 야콥스탈 수 기준 뒤에서부터 찾아야 함.) 이진 탐색을 사용.
포드-존슨 알고리즘(Ford-Johnson algorithm)은 정렬 알고리즘 중 하나로, 비교를 통한 정렬에서 최소한의 비교 횟수를 목표로 한다. 이 알고리즘은 "The Art of Computer Programming"에 자세히 설명되어 있으며, 비교 기반 정렬 알고리즘의 이론적 하한선인 O(n log n)에 가까운 성능을 보이는 것으로 알려져 있다.

포드존슨 알고리즘의 핵심 아이디어는 다음과 같다:

1. **원소 짝 지어 비교**: 배열의 원소를 2개씩 짝지어 비교하고, 각 쌍에서 큰 값과 작은 값을 구분한다.
2. **큰 값들로 배열 정렬**: 짝지어진 원소들 중 큰 값들만 모아 새로운 배열을 만들고, 이 배열을 정렬한다.
3. **작은 값들 삽입**: 정렬된 큰 값들의 배열에 작은 값들을 적절한 순서로 삽입한다. 이때 삽입 순서는 특정 패턴(예: 1, 3, 2, 5, 4, 11, ...)을 따른다.
4. **홀수 개 원소 처리**: 원소의 개수가 홀수일 경우, 남은 마지막 원소를 이진 탐색을 통해 적절한 위치에 삽입한다.

포드-존슨 알고리즘의 특징은 원소들을 짝지어 비교하고, 큰 값들로만 이루어진 배열을 먼저 정렬한 뒤, 작은 값들을 특정 순서에 따라 삽입하는 과정에서 최소한의 비교로 정렬을 완성한다는 점이다. 이 과정에서 사용되는 삽입 순서는 야콥스탈 수(Jacobsthal numbers)와 관련이 있으며, 이 순서를 따름으로써 비교 횟수를 최소화한다.

이 알고리즘은 이론적으로 비교 기반 정렬 알고리즘 중에서 매우 효율적인 편에 속하지만, 구현의 복잡성과 실제 컴퓨터에서의 캐시 효율성 등을 고려할 때 모든 상황에서 최선의 선택은 아닐 수 있다. 그럼에도 불구하고, 정렬 알고리즘의 이론적 한계와 가능성을 탐구하는 데 있어 중요한 알고리즘이다.

야콥스탈 수열이 [0, 1, 1, 3, 5, 11, ...] 이런 식으로 진행된다고 친다. 이 수열을 이용해 삽입할 위치를 결정할 때, 큰 수열의 인덱스에서 시작해서 작은 수열의 인덱스로 이동하며 적절한 위치를 찾는다.

//챗봇 정리본
예를 들어, 16개의 요소가 이미 정렬된 배열이 있고, 여기에 새로운 요소를 삽입하려고 한다고 해보자. 일반적인 이진 탐색을 사용하면, 삽입할 위치를 찾기 위해 최대 4번의 비교가 필요해. 왜냐하면 16 = 2^4이기 때문에, 이진 탐색의 각 단계에서 배열을 반으로 나누면서 탐색 범위를 줄여나가기 때문이야.

하지만, 야콥스탈 수열을 사용하면 이 과정을 더 효율적으로 만들 수 있어. 야콥스탈 수열의 몇몇 초기 값은 0, 1, 2, 5, 11, ... 이렇게 되어있어. 이 수열을 이용하면, 삽입할 위치를 찾기 위한 접근 방식을 다음과 같이 바꿀 수 있어:

1. 배열의 크기(16)보다 작거나 같은 가장 큰 야콥스탈 수를 찾아. 이 경우, 11이야.
2. 배열의 시작점에서 11번째 위치로 바로 이동해. 여기서 삽입할 요소와 비교해.
3. 만약 삽입할 요소가 11번째 위치의 요소보다 크면, 다음 야콥스탈 수(5)를 이용해 11 + 5 = 16번째 위치로 이동하고 비교해. (이 경우 배열의 크기를 넘어서므로 실제로는 이 단계는 수행되지 않아.)
4. 만약 삽입할 요소가 11번째 위치의 요소보다 작으면, 11번째 위치 이전으로 돌아가서 다음으로 작은 야콥스탈 수(5)를 이용해. 이번에는 11 - 5 = 6번째 위치로 이동해 비교해.
5. 이 과정을 반복하면서 삽입할 위치를 좁혀나가.

이 방식을 사용하면, 삽입할 위치를 찾기 위해 필요한 비교 횟수를 줄일 수 있어. 특히, 삽입할 요소가 배열의 양 끝에 가까운 경우에 이점을 더 크게 느낄 수 있어. 이진 탐색에 비해 각 단계에서 더 큰 간격으로 이동할 수 있기 때문에, 전체적으로 더 적은 비교로 원하는 위치를 찾을 수 있게 되는 거야.

이진 탐색과 야콥스탈 수열의 시간 복잡도는 같은 O(log n)이다.
이진 탐색은 항상 중앙값을 기준으로 탐색 범위를 나누기 때문에, 삽입할 위치가 배열의 양 끝에 가까운 경우에도 여러 단계를 거쳐야 하지만
야콥스탈 수열은 탐색할 위치가 배열의 양 끝에 가까운 경우나 특정 구간에 집중되어 있는 경우에 더 적은 비교로 원하는 위치를 찾을 수 있는 장점.
*/

class PMergeMe
{
public:
	PMergeMe();
	PMergeMe(int argc, char** argv);
	PMergeMe(PMergeMe const &);
	PMergeMe &operator=(PMergeMe const &);
	~PMergeMe();

    void display() const;
	void dequeDisplay() const;
	void sort();
	void dequeSort();

	std::vector<int> getSequence() const;
	std::deque<int> getSequenceDeque() const;

private:
    std::vector<int> arr; // {2, 1, 4, 3}
	std::vector<std::pair<int, int> > indexPairs; // <int(값), int(인덱스)>
	std::vector<std::pair<int, int> > pairs; // <int(큰 수), int(작은 수)>
	std::vector<int> jacobsthalNumbers; // 야콥스탈 수열

	void initializeArray(int argc, char** argv);

	// 1. indexPairs 초기화. pair<int(값), int(인덱스)>
	// 예) 2 1 4 3 -> (2, 0), (1, 1), (4, 2), (3, 3)
	void initIndexPairs();

	// 2. pairs 초기화. pair<int(큰 수), int(작은 수)>
	// 예) 2 1 4 3 -> (2, 1), (4, 3)
	void initPairs();

	// 3. 요소 그룹화. 첫 번째 요소들을 짝지어 pair(큰 수, 작은 수)의 개수가 2개 이하가 될 때까지 그룹화(재귀)
	// 홀수일 경우, 마지막 요소는 pair의 두 번째 요소를 -1로 설정
	// 예) (값) 2 1 4 3 -> (2, 1), (4, 3)
	// 예) (index) 0 1 2 3 -> (0, 1), (2, 3)
	// 예) 8 2 4 6 1 3 7 5 -> (8, 2), (6, 4), (3, 1), (7, 5)
	// -> 최종적으로 [(8, 6)]과 [(7, 3)]이 남음
	void GroupElements(std::vector<std::pair<int, int> >& currentPairs);

	// 3.5 작은 배열 중 첫 번째 요소는 큰 배열의 첫 번째 요소보다 작으니까 먼저 삽입
	void preprocessFirstPair(std::vector<std::pair<int, int> >& pairs);

    // 4. 첫 번째 요소 기준 정렬. 그룹이 2개 이하일 때, pair의 첫 번째 요소들을 기준으로 삽입 정렬.
	// 첫 번째 요소가 이동할 때, 두 번째 요소도 함께 이동해야 함 -> 아직 모름.
    // 예) (2, 1), (4, 3) -> (4, 3), (2, 1)
	void sortPairsByFirstElement(std::vector<std::pair<int, int> >& paired);
	int binarySearch(const std::vector<std::pair<int, int> >& pairs, const std::pair<int, int>& target, int start, int end);

	// 5. pair의 두 번째 요소들을 첫 번째 요소들에 삽입. 두 번째 요소들을 첫 번째 요소에 삽입하는 과정에서 야콥스탈 수(Jacobsthal numbers)를 기준으로 이진 탐색
    // 예) (4, 3), (2, 1)에서 두 번째 요소인 3과 1을 이미 정렬된 첫 번째 요소보다 작은 것(3은 4)(1은 2)까지만 비교하여 삽입. 최종 결과: 1 2 3 4
	void insertSecondElement(std::vector<std::pair<int, int> >& sortedArr);
	// int binarySearchWithJacobsthal(int target, int start, int end, std::vector<int>& jacobsthalNumbers, std::vector<std::pair<int, int> >& sortedArr);

	// 5-1. 야콥스탈 수 계산
	// J(n) = J(n-1) + 2J(n-2) (n > 1) 
	std::vector<int> calculateJacobsthalNumbers(int n);

	// 5-2. 야콥 스탈 수를 이용한 이진 탐색으로 삽입 위치 찾기
	// 5-3. 찾은 위치에 삽입
	// void insertElementInCorrectPosition(int insertPos, int target, std::vector<std::pair<int, int> >& sortedArr);

	// 6. 정렬된 pairs를 원래 배열에 적용
	void applySortedPairs();
	
	// ----------Deque---------- //

	std::deque<std::pair<int, int> > dequeIndexPairs;
	std::deque<std::pair<int, int> > dequePairs;
	std::deque<int> dequeArr;
	std::deque<int> jacobsthalNumbersDeque;
	
	void initializeArrayDeque(int argc, char** argv);
	void initIndexPairsDeque();
	void initPairsDeque();

	void GroupElements(std::deque<std::pair<int, int> >& currentPairs);
	void preprocessFirstPair(std::deque<std::pair<int, int> >& pairs);
	
	void sortPairsByFirstElement(std::deque<std::pair<int, int> >& paired);
	int binarySearch(const std::deque<std::pair<int, int> >& pairs, const std::pair<int, int>& target, int start, int end);

	void insertSecondElement(std::deque<std::pair<int, int> >& sortedArr);
	// void insertElementInCorrectPosition(std::deque<std::pair<int, int> >::iterator it, int target, std::deque<std::pair<int, int> >& sortedArr);
	// int binarySearchWithJacobsthal(int target, int start, int end, std::deque<int>& jacobsthalNumbers, std::deque<std::pair<int, int> >& sortedArr);
	// std::deque<int> calculateJacobsthalNumbersDeque(int n);
	void applySortedPairsDeque();
};

#endif // PMERGEME_HPP