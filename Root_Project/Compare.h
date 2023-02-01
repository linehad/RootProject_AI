#pragma once
#include<vector>
#include <string>

enum Coler
{
	WHITE = 15, GREEN = 10, BLUE = 9, BLACK = 0
};

// ���� ��ü�� �����ϰ� ��ǥ�� ���ִ� ������ ����ϴ� Ŭ����
class Vec2D {

public:
	int x;
	int y;


	Vec2D() { x = 2147483640; y = 2147483640; } // ���� �Էµ��� ���� ���� ��� �� ��� int�� �ִ����� �ʱ�ȭ �Ͽ� ���Ľ� ������ ������ �Ѵ�.
	Vec2D(int x, int y);
	Vec2D operator -(const Vec2D& other) { return Vec2D(this->x - other.x, this->y - other.y); }
	Vec2D operator +(const Vec2D& other) { return Vec2D(this->x + other.x, this->y + other.y); }
	bool operator <(Vec2D& other) { return this->x < other.x; }
	bool operator >(Vec2D& other) { return this->x > other.x; }
	bool operator ==(Vec2D& other) { return this->x == other.x && this->y == other.y; }
	std::ostream& operator <<(const Vec2D& other) { return std::cout << other.x << ", " << other.y; }
};

// Coordinate�� ��ӹ޾� �Է� ���� ��ǥ�� ������ ����ϴ� Ŭ����
class Compare {
private:
	int count; // ������ ��
	int m_size; // ������ ��ü ũ��
	int m_posSize; // m_pos�� ũ�� ������ ��ǥ���� ������ ���δ�.
	int m_VposSize; // m_Vpos�� ��ǥ���� ������ ���δ�.

	std::vector<std::vector<Vec2D> > m_map; // ���� ����� ����
	std::vector<Vec2D> m_pos; // ��ǥ�� ����� ����
	std::vector<Vec2D> m_Vpos; // ��ǥ���� �����Ͽ� ���� �� ��ġ�� �˷��� ����
public:

	Compare(int size);
	~Compare();
	// size, string ���� �޾� �����ϰ� count���� �����ִ� ��� �Լ�
	int Sort(std::string);

	void SetMap(); // ���� ����� �Լ�
	void Root(int count); // �� ã�� �Լ�
	void ColorPrint(int); // �� �ٲٴ� �Լ�
	void Print(); // �� ����Ʈ �Լ�
};