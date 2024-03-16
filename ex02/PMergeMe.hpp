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

/*
포드 존슨 알고리즘을 구현. 정수 배열을 효율적으로 정렬하는 방법.
기본 아이디어는 '병합 삽입 정렬'을 개선해 비교 횟수를 줄임.

- 양의 정수 배열 -> 모든 원소가 0보다 큰 정수인 배열

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
4. 작은 배열을 큰 배열에 삽입(이진 탐색): 정렬된 작은 배열의 원소들을 이진 탐색을 사용해 큰 배열에 적절한 위치에 삽입해. 이 과정을 통해 전체 배열이 정렬

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
1. 원소 두개를 번갈아가며 vector<pair>로 짝지음 (홀수 개일 경우, 마지막 원소는 pair의 두 번째 원소를 -1로 설정)
2. 배열을 반으로 나누고(재귀), 원소가 2개 이하이면 정렬
3. 정렬된 원소의 index가 짝지어진 큰 배열에 영향이 감. -> 어떻게?
4. 작은 배열 정렬(삽입)
5. 큰 배열 재귀정렬
6. 작은 배열을 큰 배열에 삽입(이진 탐색)
7. 배열의 원소가 2개 이하일 때까지 재귀적으로 반복

0. vector<int> arr 받음
1. 2개씩 vector<pair<int(큰 수), int(작은 수)>> (홀수 개일 경우, 마지막 원소는 pair의 두 번째 원소를 -1로 설정)
2. 2개 이하가 될 때까지 반으로 나눔
3. 2개 일 때, pair의 첫 번째 요소끼리 비교 (v.size() < 2)
4. 2개 일 때의 정렬된 원소와 pair로 연결된 원소의 위치도 정렬 -> 어떻게?
5. 큰 배열에 삽입(이진 탐색) -> 야콥 스타일 수로 계산하여 삽입해서 비교 횟수 줄임 -> 어떻게?

1. vector<pair<int(큰 수), int(작은 수)> >로 짝지음 예) 8 2 4 6 -> 8 2, 4 6 (8과 2를 pair로)
2. pair의 첫 번째 원소들을 삽입 정렬 예) 8 2 4 6 -> 4 2 8 6 (8과 4를 swap)
3. pair의 두 번째 원소들은 첫 번째 원소들의 index에 따라 swap 예) 4 2 8 6 -> 4 6 8 2 (비교 없음. 2와 6을 swap)
4. pair의 두 번째 원소들을 첫 번째 원소들에 삽입(이진 탐색) 예) 4 8, 2 6 -> 2 4 6 8 근데 pair의 두번째 원소인 2는 한 번 비교된 4보다 작아서 4까지만 비교하면 됨.

0. vector<int> arr 받음
1. vector<pair<int(큰 수), int(작은 수)> >로 짝지음 예) 8 2 4 6 -> (8, 2), (4, 6)
2. 2개 이하가 될 때까지 첫 번째 원소들을 짝지음
// 8 2 4 6 1 3 7 5 -> (8, 2), (6, 4), (3, 1), (7, 5) -> [(8, 2), (6, 4)]와 [(3, 1), (7, 5)] 비교 -> [(8, 6),  (7, 3)] -> [(8, 6)]과 [(7, 3)] 이 최종적으로 남음(2개)
2. pair의 첫 번째 원소들을 기준으로 삽입 정렬. 첫 번째 원소가 이동할 때 두 번째 원소도 함께 이동. 예) 8 2 4 6 -> (4, 6), (8, 2)
3. 각 쌍의 두 번째 원소를 첫 번째 원소에 삽입(야콥 스타일 수(Jacobsthal numbers)를 기준으로 이진 탐색). 예) (4, 6), (8, 2)에서 두 번째 원소인 6과 2를 이미 정렬된 첫 번째 원소보다 작은 것까지만 비교하여 삽입. 2 4 6 8
J(n) = J(n-1) + 2J(n-2) + 1 (n > 1)

0. vector<int> arr 받음
1. vector<pair<int(값), int(인덱스)> >로 초기화
	예) 2 1 4 3 -> (2, 0), (1, 1), (4, 2), (3, 3)
2. 첫 번째 원소들을 짝지어 pair(큰 수, 작은 수)의 개수가 2개 이하가 될 때까지 그룹화(재귀)
	// 예) (값) 2 1 4 3 -> (2, 1), (4, 3)
	// 예) (index) 0 1 2 3 -> (0, 1), (2, 3)
	// 예) 8 2 4 6 1 3 7 5 -> (8, 2), (6, 4), (3, 1), (7, 5) -> [(8, 2), (6, 4)]와 [(3, 1), (7, 5)] 비교 -> [(8, 6), (7, 3)] -> 최종적으로 [(8, 6)]과 [(7, 3)]이 남음.
3. 2개 이하일 때, pair의 첫 번째 원소들을 기준으로 삽입 정렬. 첫 번째 원소가 이동할 때, 두 번째 원소도 함께 이동.
	예) (2, 1), (4, 3) -> (4, 3), (2, 1)
4. 각 쌍의 두 번째 원소를 첫 번째 원소에 삽입. 이때, 야콥 스타일 수(Jacobsthal numbers)를 기준으로 이진 탐색을 사용.
	// 예) (4, 3), (2, 1)에서 두 번째 원소인 3과 1을 이미 정렬된 첫 번째 원소보다 작은 것(3 -> 4)(1 -> 2)까지만 비교하여 삽입. 최종 결과: 1 2 3 4
*/

class PMergeMe
{
public:
	PMergeMe();
	PMergeMe(std::vector<int> &arr);
	PMergeMe(PMergeMe const &);
	PMergeMe &operator=(PMergeMe const &);
	~PMergeMe();

	void sort();

    void printSortedSequence() const;

	std::vector<int> getSequence() const;

private:
    std::vector<int> arr; // {2, 1, 4, 3}
	std::vector<std::pair<size_t, size_t> > indexPairs; // <size_t(값), size_t(인덱스)>
	std::vector<std::pair<int, int> > pairs; // <int(큰 수), int(작은 수)>

	// 1. pair<int(값), int(인덱스)>로 초기화 indexPairs
	// 예) 2 1 4 3 -> (2, 0), (1, 1), (4, 2), (3, 3)
	void initIndexPairs();

	// 2. pair<int(큰 수), int(작은 수)> pairs 초기화
	// 예) 2 1 4 3 -> (2, 1), (4, 3)
	void initPairs();

	// 3. 첫 번째 원소들을 짝지어 pair(큰 수, 작은 수)의 개수가 2개 이하가 될 때까지 그룹화(재귀)
	// 홀수일 경우, 마지막 원소는 pair의 두 번째 원소를 -1로 설정
	// 예) (값) 2 1 4 3 -> (2, 1), (4, 3)
	// 예) (index) 0 1 2 3 -> (0, 1), (2, 3)
	// 예) 8 2 4 6 1 3 7 5 -> (8, 2), (6, 4), (3, 1), (7, 5)
	// -> [(8, 2), (6, 4)]와 [(3, 1), (7, 5)] 비교
	// -> 최종적으로 [(8, 6)]과 [(7, 3)]이 남음
	void GroupElements(std::vector<std::pair<int, int> >& currentPairs);

    // 4. 2개 이하일 때, pair의 첫 번째 원소들을 기준으로 삽입 정렬. 첫 번째 원소가 이동할 때, 두 번째 원소도 함께 이동.
    // 예) (2, 1), (4, 3) -> (4, 3), (2, 1)
	void sortPairsByFirstElement(std::vector<std::pair<int, int> >& paired);

	// 5. 각 쌍의 두 번째 원소를 첫 번째 원소에 삽입. 이때, 야콥 스타일 수(Jacobsthal numbers)를 기준으로 이진 탐색을 사용.
    // 예) (4, 3), (2, 1)에서 두 번째 원소인 3과 1을 이미 정렬된 첫 번째 원소보다 작은 것(3은 4)(1은 2)까지만 비교하여 삽입. 최종 결과: 1 2 3 4
	// J(n) = J(n-1) + 2J(n-2) + 1 (n > 1) 
    void insertSecondElement(std::vector<int>& sortedArr);

	void applySortedPairs();
};



#endif // PMERGEME_HPP