#pragma once
#include<vector>
#include <string>

enum Coler
{
	WHITE = 15, GREEN = 10, BLUE = 9, BLACK = 0
};

// 벡터 객체를 저장하고 좌표를 빼주는 연산을 담당하는 클래스
class Vec2D {

public:
	int x;
	int y;


	Vec2D() { x = 2147483640; y = 2147483640; } // 만약 입력되지 않은 값이 들어 갈 경우 int형 최댓값으로 초기화 하여 정렬시 문제가 없도록 한다.
	Vec2D(int x, int y);
	Vec2D operator -(const Vec2D& other) { return Vec2D(this->x - other.x, this->y - other.y); }
	Vec2D operator +(const Vec2D& other) { return Vec2D(this->x + other.x, this->y + other.y); }
	bool operator <(Vec2D& other) { return this->x < other.x; }
	bool operator >(Vec2D& other) { return this->x > other.x; }
	bool operator ==(Vec2D& other) { return this->x == other.x && this->y == other.y; }
	std::ostream& operator <<(const Vec2D& other) { return std::cout << other.x << ", " << other.y; }
};

// Coordinate를 상속받아 입력 받은 좌표를 실제로 계산하는 클래스
class Compare {
private:
	int count; // 지나간 길
	int m_size; // 벡터의 전체 크기
	int m_posSize; // m_pos의 크기 정렬한 좌표값을 읽을때 쓰인다.
	int m_VposSize; // m_Vpos의 좌표값을 읽을때 쓰인다.

	std::vector<std::vector<Vec2D> > m_map; // 맵이 저장될 벡터
	std::vector<Vec2D> m_pos; // 좌표가 저장될 벡터
	std::vector<Vec2D> m_Vpos; // 좌표끼리 뺄셈하여 방향 및 위치를 알려줄 벡터
public:

	Compare(int size);
	~Compare();
	// size, string 값을 받아 정렬하고 count값을 돌려주는 멤버 함수
	int Sort(std::string);

	void SetMap(); // 맵을 만드는 함수
	void Root(int count); // 길 찾는 함수
	void ColorPrint(int); // 색 바꾸는 함수
	void Print(); // 맵 프린트 함수
};