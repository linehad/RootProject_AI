#include <iostream>
#include <Windows.h>
#include <queue>
#include<algorithm>
#include <sstream>
#include "Compare.h"


Vec2D::Vec2D(int x, int y)
{
	this->x = x;
	this->y = y;
}


Compare::Compare(int size)
{
	count = 0;
	m_posSize = 0;
	m_VposSize = 0;
	m_size = size;
	m_Vpos.resize(size * size);
	m_map.resize(size, std::vector<Vec2D>(size));
	//visit = new int* [size];
}

Compare::~Compare()
{
	/*
	for (int i = 0; i < m_size; i++) {
		delete[]visit[i];
	}*/
}

int Compare::Sort(std::string index)
{
	{
		using namespace std;

		int x_pos, y_pos, count = 0;
		string temp;

		bool flag = true;

		// �� ������� ����� ���
		stringstream ss(index);
		while (ss >> temp)
		{
			if (flag)
			{
				flag = false;
				x_pos = stol(temp);

				// x��ǥ ���� size ���� Ŭ �� �����Ƿ� contine;
				if (x_pos >= m_size) {
					continue;
				}
			}
			else
			{
				flag = true;
				y_pos = stol(temp);

				// y��ǥ ���� size ���� Ŭ �� �����Ƿ� contine;
				if (y_pos >= m_size || x_pos >= m_size) {
					std::cout << "x �Ǵ� y ���� �ʰ� : " << x_pos << ", " << y_pos << endl;
					x_pos = y_pos = 0;
					continue;
				}

				// ���⿡�� vector�� �ִ� ������ x���� �־��� ��� �����ϱ� �����̴�.
				Vec2D vec(x_pos, y_pos);
				m_pos.push_back(vec);
				count++;
			}
		}
			
		// �ߺ� �� �� ó��
		m_pos.erase(unique(m_pos.begin(), m_pos.end()), m_pos.end());
		return count;
	}
}

void Compare::SetMap()
{
	using std::cout;
	using std::endl;

	const int POINT = 2147483647; // POINT : �����ؾ� �ϴ� ������
	const int ROOT = 2147483646; // ROOT : ������ ��
	const int START = 2147483645; // START : ���� ����
	const int END = 2147483644; // END : ���� ����
	const int OVER = 2147483640; // ���� ��� ��� ����

	bool flag = false;
	bool pass = false; // ������ ���� üũ �� bool

	m_posSize = m_pos.size();
	m_VposSize = m_Vpos.size();

	for (int i = 0; i < m_posSize; i++) {
		cout << m_pos[i].x << ", " << m_pos[i].y << endl;
	}
	cout << endl;

	//���� ���Ϳ��� �տ� �ִ� ���͸� ���� ������ ã�´�.
	for (int i = 0; i < (m_posSize -1); i++) {
		m_Vpos[i] = m_pos[i + 1] - m_pos[i];
		cout << m_Vpos[i].x << ", " << m_Vpos[i].y << endl;
	}

	cout << endl << endl;

	//�� �ʿ� ��ǥ�� �Է�
	for (int y = 0; y < m_size; y++) {
		for (int x = 0; x < m_size; x++) {
			Vec2D vec(x, y);
			m_map[y][x] = vec;
		}
	}

	// ��ü ���� ���鼭 �Է� ���� ��ǥ�� POINT�� �ٲٴ� �۾��� �Ѵ�.
	for (int i = 0; i < (m_size); i++)
	{
		for (int j = 0; j < (m_size); j++)
		{
			for (int k = 0; k < m_posSize; k++)
			{
				// ������ ���ϱ�
				if (m_pos[0] == m_map[i][j]) {
					m_map[i][j].x = START;
					m_map[i][j].y = START;
					break;
					//flag = true;
				}

				// �������� ���� Ž������ m_map�� ������ x,y��ǥ�� POINT�� �ٲ۴�.
				else if (m_pos[k] == m_map[i][j]) {
					m_map[i][j].x = POINT;
					m_map[i][j].y = POINT;

					pass = true;
					break;
				}

				// ������ ���ϱ�
				else if (m_pos[m_posSize - 1] == m_map[i][j]) {
					m_map[i][j].x = END;
					m_map[i][j].y = END;
					break;
					flag = true;
				}
			}
			if (pass || flag)
			{
				pass = false;
				break;
			}

		}
		// END�� �Է��ϸ� �ݺ��� ����
		if (flag)
		{
			flag = false;
			break;
		}

	}
}

// ���� ã�� ��� �Լ�
void Compare::Root(int count)
{
	using std::cout;
	using std::endl;
	using std::vector;

	int point_size = 0;
	int tempx, tempy; // ������ �����Ͽ� �켱������ ���� ����
	int vecSize = 0;

	const int POINT = 2147483647; // POINT : �����ؾ� �ϴ� ������
	const int ROOT = 2147483646; // ROOT : ������ ��
	const int START = 2147483645; // START : ���� ����
	const int END = 2147483644; // END : ���� ����
	const int OVER = 2147483640; // ���� ��� ��� ����

	bool endpos = false; // ��ġ�� ���� �����̸� ���� �ݺ���
	bool trueEND = false; // m_pos�� ����� ������ ������ ��� true

	bool searchRX = false; // x�� ������ �̵�
	bool searchLX = false; // x�� ���� �̵�
	bool searchRY = false; // y�� ������ �̵�
	bool searchLY = false; // y�� ���� �̵�

	bool blackRX = false; // X ���� �� üũ
	bool blackLX = false; // -X ���� �� üũ
	bool blackRY = false; // Y ���� �� üũ
	bool blackLY = false; // -Y ���� �� üũ
	bool blackRoot = false; // ��ȸ ���� ����

	SetMap();

	// ������, �������� �������� �����ϴ� ���� POINT�� �� �����̴�.
	// ���� ���͸�ŭ �ݺ��Ѵ�.
	for (int i = 0; i < m_VposSize; i++)
	{
		endpos = false;
		if (trueEND)
		{
			break;
		}
		// ���� ũ�⸸ŭ ���� ã�ƾ� �� �� �ִ�. ���� �� ��üũ�� ��ŭ �ݺ��ϵ� END�� ������ break
		for (int j = 0; j < (m_size * m_size); j++)
		{
			if (endpos || trueEND)
			{
				break;
			}
			// 0�� ����� ������� ���� ��ȸ�ؾ� �Ѵ� ���� �������� ������ش�.
			// x�� 0���� ���� ��� �����Ѵ�.
			if (m_Vpos[i].y > 0 && m_Vpos[i].x < 0 && !blackRoot)
			{
				tempx = m_Vpos[i].x * (-1);

				// y�� 0���� Ŭ ���
				tempy = m_Vpos[i].y;

				// x�� ���� y�� ����̰� x�� ���� �� x ��ǥ���� �̵�
				if (tempx != 0 && (tempx < tempy))
				{
					searchRX = false;
					searchLX = true;
					searchRY = false;
					searchLY = false;
				}

				// x�� ���� y�� ����̰� y�� ���� �� y ��ǥ���� �̵�
				else if (tempy != 0 && (tempx > tempy))
				{
					searchRX = false;
					searchLX = false;
					searchRY = false;
					searchLY = true;

				}

				// ���� ���ٸ� �������� �̵��Ѵ�.
				else
				{
					searchRX = false;
					searchLX = true;
					searchRY = false;
					searchLY = false;
				}

			}

			// y�� ���� x�� ����� ��
			else if (m_Vpos[i].x > 0 && m_Vpos[i].y < 0 && !blackRoot)
			{
				tempy = m_Vpos[i].y * (-1);

				tempx = m_Vpos[i].x;


				// x�� ����̹Ƿ� x +��ǥ �̵�
				if (tempx != 0 && (tempx < tempy))
				{
					searchRX = true;
					searchLX = false;
					searchRY = false;
					searchLY = false;
				}

				// y�� ���� �̹Ƿ� -��ǥ �̵�
				else if (tempy != 0 && (tempx > tempy))
				{
					searchRX = false;
					searchLX = false;
					searchRY = false;
					searchLY = true;

				}

				// ���� ���ٸ� ���������� �̵��Ѵ�.
				else
				{
					searchRX = true;
					searchLX = false;
					searchRY = false;
					searchLY = false;
				}

			}

			// x, y�� ������ ��
			else if (m_Vpos[i].y < 0 && m_Vpos[i].x < 0 && !blackRoot)
			{
				tempx = m_Vpos[i].x * (-1);
				tempy = m_Vpos[i].y * (-1);

				// x, y�� �����̰� x�� y���� �����Ƿ� x ��ǥ���� �̵��Ѵ�.
				if (tempx != 0 && (tempx < tempy))
				{
					searchRX = false;
					searchLX = true;
					searchRY = false;
					searchLY = false;
				}

				// x, y�� �����̰� y�� x���� �����Ƿ� y ��ǥ���� �̵��Ѵ�.
				else if (tempy != 0 && (tempx > tempy))
				{
					searchRX = false;
					searchLX = false;
					searchRY = false;
					searchLY = true;

				}

				// ���� ���ٸ� �������� �̵��Ѵ�.
				else
				{
					searchRX = false;
					searchLX = true;
					searchRY = false;
					searchLY = false;
				}
			}

			// �Ѵ� ����� x�� ������
			else if ((m_Vpos[i].y != 0) && (m_Vpos[i].x != 0) && (m_Vpos[i].x < m_Vpos[i].y) && !blackRoot)
			{
				searchRX = true;
				searchLX = false;
				searchRY = false;
				searchLY = false;

			}

			// �Ѵ� ����� y�� ������
			else if ((m_Vpos[i].y != 0) && (m_Vpos[i].x != 0) && (m_Vpos[i].x > m_Vpos[i].y) && !blackRoot)
			{
				searchRX = false;
				searchLX = false;
				searchRY = true;
				searchLY = false;
			}

			// x�� 0�϶�
			else if (m_Vpos[i].x == 0 && !blackRoot)
			{
				// y�� 0���� ���� ��
				if (m_Vpos[i].y < 0)
				{
					searchRX = false;
					searchLX = false;
					searchRY = false;
					searchLY = true;
				}
				else
				{
					searchRX = false;
					searchLX = false;
					searchRY = true;
					searchLY = false;
				}
			}

			// y�� 0�϶�
			else if (m_Vpos[i].y == 0 && !blackRoot)
			{
				// x�� 0���� ���� ��
				if (m_Vpos[i].x < 0)
				{
					searchRX = false;
					searchLX = true;
					searchRY = false;
					searchLY = false;
				}
				else
				{
					searchRX = true;
					searchLX = false;
					searchRY = false;
					searchLY = false;
				}
			}

			// �Ѵ� ����� ���� ��
			else if(m_Vpos[i].y == m_Vpos[i].x && !blackRoot)
			{
				searchRX = true;
				searchLX = false;
				searchRY = false;
				searchLY = false;

			}

			static int tx = m_pos[point_size].x; // �Է� ���� x�� static���� ���� �߱� ������ if�� �������� ���� �Ǿ �ʱ�ȭ ���� ����
			static int ty = m_pos[point_size].y; // �Է� ���� y �� static���� ���� �߱� ������ if�� �������� ���� �Ǿ �ʱ�ȭ ���� ����

			while (true)
			{
				// ���� ���� �Ѿ�� ����ó��
				if (m_Vpos[vecSize].x == OVER) {
					trueEND = true;
					break;
				}
				// END������ ��� ���� ����
				else if (m_Vpos[vecSize].x == END) {
					trueEND = true;
					break;
				}

				// m_map�� x���� START, END, ROOT�� �ƴϰ� searchRX�� ���϶� ����
				//������ �̵�
				if ((searchRX && m_map[tx][ty].x != START) || (searchRX && m_map[tx][ty].x != ROOT) || (searchRX && m_map[tx][ty].x != END) || (searchRX && blackRoot))
				{

					// Ž���ϴٰ� ���⺤�Ͱ� ��� 0�̵Ǹ� ����Ʈ�� ���� ���̱� ������ �̸� START�� �ٲ��ش�.
					if (m_Vpos[vecSize].x == 0 && m_Vpos[vecSize].y == 0) {
						m_map[ty][tx].x = START;
						m_map[ty][tx].y = START;
						endpos = true;
						break;
					}

					// x�� �������� ���� ���� ��� ������
					else if (m_Vpos[vecSize].x == 0 && !blackRoot)
					{
						searchRX = false;
						break;
					}

					// tx�� ���� x��ǥ�� ����Ű���� 0 �Ǵ� ��ü �� ũ�⸦ ���� �� ����. ���� ����ó��
					else if (tx <= m_size - 2) {
						++tx;
					}

					// tx + 1�� �ִ밪�� �ƴϰ� ���� �̵��� ��ǥ�� ROOT, START���
					if (m_map[ty][tx].x == ROOT || m_map[ty][tx].x == START)
					{

						// tx - 1�� -1�� �ƴϰ� ���� �̵��� ��ǥ�� ROOT, START���
						if (m_map[ty][tx].x == ROOT || m_map[ty][tx].x == START)
						{
							// ������ ���� �� �ǵ��ƿ���
							--tx;
							blackRX = true;
							blackRoot = true;

							// ���� ������ ��� y��ǥ ���� Ž�� y�� 0���� Ŭ ��
							if (m_Vpos[vecSize].y > 0 && ((ty + 1) <= (m_size - 1)) && (!blackRY && (m_map[ty + 1][tx].y <= m_size - 2)))
							{
								searchRX = false;
								searchLX = false;
								searchRY = true;
								searchLY = false;
							}

							else if (m_Vpos[vecSize].y < 0 && ((ty - 1) > 0) && (!blackLY && m_map[ty - 1][tx].y >= 1))
							{
								searchRX = false;
								searchLX = false;
								searchRY = false;
								searchLY = true;
							}

							else if (m_Vpos[vecSize].x > 0 && ((tx - 1) > 0) && (!blackLX && m_map[ty][tx - 1].x >= 1))
							{
								searchRX = false;
								searchLX = true;
								searchRY = false;
								searchLY = false;
							}
							else
							{
								cout << "�� �� ���� ��" << endl;
								trueEND = true;
							}
							break;
						}
					}

					else if (blackRoot) {
						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].x -= 1;

						blackRoot = false;
						blackRX = false;
						blackLX = false;
						blackRY = false;
						blackLY = false;
						break;
					}

					else
					{
						// Ž���ϴٰ� END�� ������ �Է� ������ �� �̱� ������ �ݺ����� ����������.
						if (m_map[ty][tx].x == END) {
							trueEND = true;
							break;
						}

						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].x -= 1;						
					}
				}
				// ���� �̵�
				else if ((searchLX && m_map[tx][ty].x != START) || (searchLX && m_map[tx][ty].x != ROOT) || (searchLX && m_map[tx][ty].x != END) || (searchLX && blackRoot))
				{
					// Ž���ϴٰ� ���⺤�Ͱ� ��� 0�̵Ǹ� ����Ʈ�� ���� ���̱� ������ �̸� START�� �ٲ��ش�.
					if (m_Vpos[vecSize].x == 0 && m_Vpos[vecSize].y == 0)
					{
						m_map[ty][tx].x = START;
						m_map[ty][tx].y = START;

						endpos = true;
						break;
					}

					else if (m_Vpos[vecSize].x == 0 && !blackRoot)
					{
						searchLX = false;
						break;
					}

					// tx�� ���� ��ǥ�� ����Ű���� 0 �Ǵ� ��ü �� ũ�⸦ ���� �� ����. ���� ����ó��
					else if (tx >= 1) {
						--tx;
					}

					// tx - 1�� -1�� �ƴϰ� ���� �̵��� ��ǥ�� ROOT, START���
					if (m_map[ty][tx].x == ROOT || m_map[ty][tx].x == START)
					{
						// ������ ���� �� �ǵ��ƿ���
						++tx;

						blackLX = true;
						blackRoot = true;

						// ���� ���� ���� Ž�� ty + 1�� �ִ��� ���� �ʰ� ty�� ���� ��ǥ�� �� �տ��� 1ĭ ������ ������ ��
						if (m_Vpos[vecSize].y > 0 && ((ty + 1) <= (m_size - 1)) && (!blackRY && (m_map[ty + 1][tx].y <= m_size - 2)))
						{
							searchRX = false;
							searchLX = false;
							searchRY = true;
							searchLY = false;
						}

						else if (m_Vpos[vecSize].y < 0 && ((ty - 1) > 0) && (!blackLY && m_map[ty - 1][tx].y >= 1))
						{
							searchRX = false;
							searchLX = false;
							searchRY = false;
							searchLY = true;
						}

						else if (m_Vpos[vecSize].x > 0 && ((tx + 1) <= (m_size - 1)) && (!blackRX && (m_map[ty][tx + 1].x <= m_size - 2)))
						{
							searchRX = true;
							searchLX = false;
							searchRY = false;
							searchLY = false;
						}
						else
						{
							cout << "�� �� ���� ��" << endl;
							trueEND = true;
						}
						break;
					}

					else if (blackRoot)
					{
						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].x += 1;

						blackRoot = false;
						blackRX = false;
						blackLX = false;
						blackRY = false;
						blackLY = false;
						break;
					}

					else
					{
						// Ž���ϴٰ� END�� ������ �Է� ������ �� �̱� ������ �ݺ����� ����������.
						if (m_map[ty][tx].x == END) {
							trueEND = true;
							break;
						}

						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].x += 1;
					}
				}

				// �Ʒ��� �̵�
				else if ((searchRY && m_map[tx][ty].y != START) || (searchRY && m_map[tx][ty].y != ROOT) || (searchRY && m_map[tx][ty].y != END) || (searchRY && blackRoot))
				{
					// Ž���ϴٰ� ���⺤�Ͱ� ��� 0�̵Ǹ� ����Ʈ�� ���� ���̱� ������ �̸� START�� �ٲ��ش�.
					if (m_Vpos[vecSize].x == 0 && m_Vpos[vecSize].y == 0)
					{
						m_map[ty][tx].x = START;
						m_map[ty][tx].y = START;

						endpos = true;
						break;
					}

					else if (m_Vpos[vecSize].y == 0 && !blackRoot)
					{
						searchRY = false;
						break;
					}

					// ty�� ���� ��ǥ�� ����Ű���� 0 �Ǵ� ��ü �� ũ�⸦ ���� �� ����. ���� ����ó��
					else if (ty <= m_size - 2) {
						++ty;
					}

					// ty + 1�� �ִ밪�� �ƴϰ� ���� �̵��� ��ǥ�� ROOT, START���
					if (m_map[ty][tx].y == ROOT || m_map[ty][tx].y == START)
					{
						// ������ ���� �� �ǵ��ƿ���
						--ty;

						blackRY = true;
						blackRoot = true;

						// ���� ���� ���� Ž��
						if (m_Vpos[vecSize].x > 0 && ((tx + 1) <= (m_size - 1)) && (!blackRX && (m_map[ty][tx + 1].x <= m_size - 2)))
						{
							searchRX = true;
							searchLX = false;
							searchRY = false;
							searchLY = false;
						}

						else if (m_Vpos[vecSize].x < 0 && ((tx - 1) > 0) && (!blackLX && m_map[ty][tx - 1].x >= 1))
						{
							searchRX = false;
							searchLX = true;
							searchRY = false;
							searchLY = false;
						}

						else if (m_Vpos[vecSize].y > 0 && ((ty - 1) < 0) && (!blackLY && m_map[ty - 1][tx].y >= 1))
						{
							searchRX = false;
							searchLX = false;
							searchRY = false;
							searchLY = true;
						}
						else
						{
							cout << "�� �� ���� ��" << endl;
							trueEND = true;
						}
						break;
					}

					else if (blackRoot)
					{
						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].y -= 1;

						blackRoot = false;
						blackRX = false;
						blackLX = false;
						blackRY = false;
						blackLY = false;
						break;
					}

					else
					{
						// Ž���ϴٰ� END�� ������ �Է� ������ �� �̱� ������ �ݺ����� ����������.
						if (m_map[ty][tx].x == END) {
							trueEND = true;
							break;
						}

						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].y -= 1;
					}
				}
				
				// ���� �̵�
				else if ((searchLY && m_map[i][j].y != START) || (searchLY && m_map[i][j].y != ROOT) || (searchLY && m_map[i][j].y != END) || (searchLY && blackRoot))
				{
					// Ž���ϴٰ� ���⺤�Ͱ� ��� 0�̵Ǹ� ����Ʈ�� ���� ���̱� ������ �̸� START�� �ٲ��ش�.
					if (m_Vpos[vecSize].x == 0 && m_Vpos[vecSize].y == 0)
					{
						m_map[ty][tx].x = START;
						m_map[ty][tx].y = START;

						endpos = true;
						break;
					}

					// y�� 0�� ��� ���� �̵� �� ���̹Ƿ� while�� ����
					else if (m_Vpos[vecSize].y == 0 && !blackRoot)
					{
						searchLY = false;
						break;
					}

					// tx�� ���� ��ǥ�� ����Ű���� 0 �Ǵ� ��ü �� ũ�⸦ ���� �� ����. ���� ����ó��
					else if (ty >= 1) 
					{
						--ty;
					}

					// ty - 1�� -1�� �ƴϰ� ���� �̵��� ��ǥ�� ROOT, START���
					if (m_map[ty][tx].y == ROOT || m_map[ty][tx].y == START)
					{

						// ������ ���� �� �ǵ��ƿ���
						++ty;

						blackLY = true;
						blackRoot = true;

						// ���� ���� ���� Ž��
						if (m_Vpos[vecSize].x > 0 && ((tx + 1) <= (m_size - 1)) && (!blackRX && (m_map[ty][tx + 1].x <= m_size - 2)))
						{
							searchRX = true;
							searchLX = false;
							searchRY = false;
							searchLY = false;
						}

						else if (m_Vpos[vecSize].x < 0 && ((tx - 1) > 0) && (!blackLX && m_map[ty][tx - 1].x >= 1))
						{
							searchRX = false;
							searchLX = true;
							searchRY = false;
							searchLY = false;
						}

						else if (m_Vpos[vecSize].y > 0 && ((ty + 1) <= (m_size - 1)) && (!blackRY && (m_map[ty + 1][tx].y <= m_size - 2)))
						{
							searchRX = false;
							searchLX = false;
							searchRY = true;
							searchLY = false;
						}
					}

					else if (blackRoot)
					{
						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].y += 1;

						blackRoot = false;
						blackRX = false;
						blackLX = false;
						blackRY = false;
						blackLY = false;
						break;
					}

					else
					{
						// Ž���ϴٰ� END�� ������ �Է� ������ �� �̱� ������ �ݺ����� ����������.
						if (m_map[ty][tx].y == END) 
						{
							trueEND = true;
							break;
						}

						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].y += 1;
					}
				}				

				else 
				{
				cout << "�� �� ���� ��" << endl;
				trueEND = true;
				}

					
				if ((m_Vpos[point_size].x == 0) && (m_Vpos[point_size].y == 0)) 
				{
					m_map[ty][tx].x = START;
					m_map[ty][tx].y = START;

					point_size++;
					vecSize++;
					endpos = true;
					break;
				}
			}
		}
	}
}




// ���� ���� ��� �Լ�
void Compare::ColorPrint(int color)
{
	char mark;
	using std::cout;
	switch (color)
	{
		// �Է� ���� ���� ���
	case WHITE:
		mark = 88;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (color << 4));
		cout << "| " << mark << " ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE | BLACK);
		break;

		// ������ ��
	case GREEN:
		mark = 42;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (color << 4));
		cout << "| " << mark << " ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE | BLACK);
		break;

		// �������� �ϴ� ��ǥ
	case BLUE:
		mark = 79;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (color << 4));
		cout << "| " << mark << " ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE | BLACK);
		break;

		// �� �������� ���� ����
	case BLACK:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE | BLACK);
		break;

	default:
		break;
	}
}

void Compare::Print()
{
	using std::cout;
	using std::endl;
	const int POINT = 2147483647; // POINT : �����ؾ� �ϴ� ������
	const int ROOT = 2147483646; // ROOT : ������ ��
	const int START = 2147483645; // START : ���� ����
	const int END = 2147483644; // END : ���� ����

	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			if (m_map[i][j].x == START || m_map[i][j].x == POINT || m_map[i][j].x == END) {
				ColorPrint(BLUE);
			}
			else if (m_map[i][j].x == ROOT)
			{
				ColorPrint(GREEN);
			}

			else
			{
				ColorPrint(WHITE);
			}
		}
		cout << endl;
	}
	ColorPrint(BLACK);
}