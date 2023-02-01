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

		// 띄어쓴 개수대로 끊어내는 기능
		stringstream ss(index);
		while (ss >> temp)
		{
			if (flag)
			{
				flag = false;
				x_pos = stol(temp);

				// x좌표 값이 size 보다 클 수 없으므로 contine;
				if (x_pos >= m_size) {
					continue;
				}
			}
			else
			{
				flag = true;
				y_pos = stol(temp);

				// y좌표 값이 size 보다 클 수 없으므로 contine;
				if (y_pos >= m_size || x_pos >= m_size) {
					std::cout << "x 또는 y 범위 초과 : " << x_pos << ", " << y_pos << endl;
					x_pos = y_pos = 0;
					continue;
				}

				// 여기에서 vector에 넣는 이유는 x값만 넣어질 경우 무시하기 위함이다.
				Vec2D vec(x_pos, y_pos);
				m_pos.push_back(vec);
				count++;
			}
		}
			
		// 중복 된 값 처리
		m_pos.erase(unique(m_pos.begin(), m_pos.end()), m_pos.end());
		return count;
	}
}

void Compare::SetMap()
{
	using std::cout;
	using std::endl;

	const int POINT = 2147483647; // POINT : 도달해야 하는 목적지
	const int ROOT = 2147483646; // ROOT : 지나간 길
	const int START = 2147483645; // START : 시작 지점
	const int END = 2147483644; // END : 종료 지점
	const int OVER = 2147483640; // 범위 벗어날 경우 예외

	bool flag = false;
	bool pass = false; // 지나간 곳을 체크 할 bool

	m_posSize = m_pos.size();
	m_VposSize = m_Vpos.size();

	for (int i = 0; i < m_posSize; i++) {
		cout << m_pos[i].x << ", " << m_pos[i].y << endl;
	}
	cout << endl;

	//현재 백터에서 앞에 있는 벡터를 빼서 방향을 찾는다.
	for (int i = 0; i < (m_posSize -1); i++) {
		m_Vpos[i] = m_pos[i + 1] - m_pos[i];
		cout << m_Vpos[i].x << ", " << m_Vpos[i].y << endl;
	}

	cout << endl << endl;

	//각 맵에 좌표값 입력
	for (int y = 0; y < m_size; y++) {
		for (int x = 0; x < m_size; x++) {
			Vec2D vec(x, y);
			m_map[y][x] = vec;
		}
	}

	// 전체 맵을 돌면서 입력 받은 좌표를 POINT로 바꾸는 작업을 한다.
	for (int i = 0; i < (m_size); i++)
	{
		for (int j = 0; j < (m_size); j++)
		{
			for (int k = 0; k < m_posSize; k++)
			{
				// 시작점 정하기
				if (m_pos[0] == m_map[i][j]) {
					m_map[i][j].x = START;
					m_map[i][j].y = START;
					break;
					//flag = true;
				}

				// 목적지와 현재 탐색중인 m_map이 같으면 x,y좌표를 POINT로 바꾼다.
				else if (m_pos[k] == m_map[i][j]) {
					m_map[i][j].x = POINT;
					m_map[i][j].y = POINT;

					pass = true;
					break;
				}

				// 도착점 정하기
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
		// END를 입력하면 반복문 종료
		if (flag)
		{
			flag = false;
			break;
		}

	}
}

// 길을 찾는 멤버 함수
void Compare::Root(int count)
{
	using std::cout;
	using std::endl;
	using std::vector;

	int point_size = 0;
	int tempx, tempy; // 절댓값을 저장하여 우선순위를 비교할 변수
	int vecSize = 0;

	const int POINT = 2147483647; // POINT : 도달해야 하는 목적지
	const int ROOT = 2147483646; // ROOT : 지나간 길
	const int START = 2147483645; // START : 시작 지점
	const int END = 2147483644; // END : 종료 지점
	const int OVER = 2147483640; // 범위 벗어날 경우 예외

	bool endpos = false; // 위치가 끝난 지점이면 종료 반복문
	bool trueEND = false; // m_pos에 저장된 마지막 원소일 경우 true

	bool searchRX = false; // x축 오른쪽 이동
	bool searchLX = false; // x축 왼쪽 이동
	bool searchRY = false; // y축 오른쪽 이동
	bool searchLY = false; // y축 왼쪽 이동

	bool blackRX = false; // X 막힌 길 체크
	bool blackLX = false; // -X 막힌 길 체크
	bool blackRY = false; // Y 막힌 길 체크
	bool blackLY = false; // -Y 막힌 길 체크
	bool blackRoot = false; // 우회 중인 상태

	SetMap();

	// 시작점, 도착점이 정해졌고 가야하는 곳이 POINT가 된 상태이다.
	// 방향 벡터만큼 반복한다.
	for (int i = 0; i < m_VposSize; i++)
	{
		endpos = false;
		if (trueEND)
		{
			break;
		}
		// 맵의 크기만큼 길을 찾아야 할 수 있다. 따라서 맵 전체크기 만큼 반복하되 END를 만나면 break
		for (int j = 0; j < (m_size * m_size); j++)
		{
			if (endpos || trueEND)
			{
				break;
			}
			// 0의 가까운 순서대로 먼저 순회해야 한다 따라서 절댓값으로 만들어준다.
			// x가 0보다 작을 경우 실행한다.
			if (m_Vpos[i].y > 0 && m_Vpos[i].x < 0 && !blackRoot)
			{
				tempx = m_Vpos[i].x * (-1);

				// y가 0보다 클 경우
				tempy = m_Vpos[i].y;

				// x가 음수 y가 양수이고 x가 작을 때 x 좌표먼저 이동
				if (tempx != 0 && (tempx < tempy))
				{
					searchRX = false;
					searchLX = true;
					searchRY = false;
					searchLY = false;
				}

				// x가 음수 y가 양수이고 y가 작을 때 y 좌표먼저 이동
				else if (tempy != 0 && (tempx > tempy))
				{
					searchRX = false;
					searchLX = false;
					searchRY = false;
					searchLY = true;

				}

				// 값이 같다면 왼쪽으로 이동한다.
				else
				{
					searchRX = false;
					searchLX = true;
					searchRY = false;
					searchLY = false;
				}

			}

			// y는 음수 x가 양수일 때
			else if (m_Vpos[i].x > 0 && m_Vpos[i].y < 0 && !blackRoot)
			{
				tempy = m_Vpos[i].y * (-1);

				tempx = m_Vpos[i].x;


				// x가 양수이므로 x +좌표 이동
				if (tempx != 0 && (tempx < tempy))
				{
					searchRX = true;
					searchLX = false;
					searchRY = false;
					searchLY = false;
				}

				// y가 음수 이므로 -좌표 이동
				else if (tempy != 0 && (tempx > tempy))
				{
					searchRX = false;
					searchLX = false;
					searchRY = false;
					searchLY = true;

				}

				// 값이 같다면 오른쪽으로 이동한다.
				else
				{
					searchRX = true;
					searchLX = false;
					searchRY = false;
					searchLY = false;
				}

			}

			// x, y가 음수일 때
			else if (m_Vpos[i].y < 0 && m_Vpos[i].x < 0 && !blackRoot)
			{
				tempx = m_Vpos[i].x * (-1);
				tempy = m_Vpos[i].y * (-1);

				// x, y가 음수이고 x가 y보다 작으므로 x 좌표먼저 이동한다.
				if (tempx != 0 && (tempx < tempy))
				{
					searchRX = false;
					searchLX = true;
					searchRY = false;
					searchLY = false;
				}

				// x, y가 음수이고 y가 x보다 작으므로 y 좌표먼저 이동한다.
				else if (tempy != 0 && (tempx > tempy))
				{
					searchRX = false;
					searchLX = false;
					searchRY = false;
					searchLY = true;

				}

				// 값이 같다면 왼쪽으로 이동한다.
				else
				{
					searchRX = false;
					searchLX = true;
					searchRY = false;
					searchLY = false;
				}
			}

			// 둘다 양수고 x가 작을때
			else if ((m_Vpos[i].y != 0) && (m_Vpos[i].x != 0) && (m_Vpos[i].x < m_Vpos[i].y) && !blackRoot)
			{
				searchRX = true;
				searchLX = false;
				searchRY = false;
				searchLY = false;

			}

			// 둘다 양수고 y가 작을때
			else if ((m_Vpos[i].y != 0) && (m_Vpos[i].x != 0) && (m_Vpos[i].x > m_Vpos[i].y) && !blackRoot)
			{
				searchRX = false;
				searchLX = false;
				searchRY = true;
				searchLY = false;
			}

			// x가 0일때
			else if (m_Vpos[i].x == 0 && !blackRoot)
			{
				// y가 0보다 작을 때
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

			// y가 0일때
			else if (m_Vpos[i].y == 0 && !blackRoot)
			{
				// x가 0보다 작을 때
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

			// 둘다 양수고 같을 떼
			else if(m_Vpos[i].y == m_Vpos[i].x && !blackRoot)
			{
				searchRX = true;
				searchLX = false;
				searchRY = false;
				searchLY = false;

			}

			static int tx = m_pos[point_size].x; // 입력 받은 x값 static으로 선언 했기 때문에 if문 스코프가 종료 되어도 초기화 되지 않음
			static int ty = m_pos[point_size].y; // 입력 받은 y 값 static으로 선언 했기 때문에 if문 스코프가 종료 되어도 초기화 되지 않음

			while (true)
			{
				// 방향 벡터 넘어가면 예외처리
				if (m_Vpos[vecSize].x == OVER) {
					trueEND = true;
					break;
				}
				// END만나면 모든 루프 종료
				else if (m_Vpos[vecSize].x == END) {
					trueEND = true;
					break;
				}

				// m_map의 x값이 START, END, ROOT가 아니고 searchRX가 참일때 실행
				//오른쪽 이동
				if ((searchRX && m_map[tx][ty].x != START) || (searchRX && m_map[tx][ty].x != ROOT) || (searchRX && m_map[tx][ty].x != END) || (searchRX && blackRoot))
				{

					// 탐색하다가 방향벡터가 모두 0이되면 포인트를 만난 것이기 때문에 이를 START로 바꿔준다.
					if (m_Vpos[vecSize].x == 0 && m_Vpos[vecSize].y == 0) {
						m_map[ty][tx].x = START;
						m_map[ty][tx].y = START;
						endpos = true;
						break;
					}

					// x가 목적지에 도달 했을 경우 나간다
					else if (m_Vpos[vecSize].x == 0 && !blackRoot)
					{
						searchRX = false;
						break;
					}

					// tx는 현재 x좌표를 가리키지만 0 또는 전체 맵 크기를 넘을 수 없다. 따라서 예외처리
					else if (tx <= m_size - 2) {
						++tx;
					}

					// tx + 1이 최대값이 아니고 다음 이동할 좌표가 ROOT, START라면
					if (m_map[ty][tx].x == ROOT || m_map[ty][tx].x == START)
					{

						// tx - 1이 -1이 아니고 다음 이동할 좌표가 ROOT, START라면
						if (m_map[ty][tx].x == ROOT || m_map[ty][tx].x == START)
						{
							// 앞으로 갔던 값 되돌아오기
							--tx;
							blackRX = true;
							blackRoot = true;

							// 길이 막혔을 경우 y좌표 먼저 탐색 y가 0보다 클 때
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
								cout << "갈 수 없는 길" << endl;
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
						// 탐색하다가 END를 만나면 입력 지점의 끝 이기 때문에 반복문을 빠져나간다.
						if (m_map[ty][tx].x == END) {
							trueEND = true;
							break;
						}

						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].x -= 1;						
					}
				}
				// 왼쪽 이동
				else if ((searchLX && m_map[tx][ty].x != START) || (searchLX && m_map[tx][ty].x != ROOT) || (searchLX && m_map[tx][ty].x != END) || (searchLX && blackRoot))
				{
					// 탐색하다가 방향벡터가 모두 0이되면 포인트를 만난 것이기 때문에 이를 START로 바꿔준다.
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

					// tx는 현재 좌표를 가리키지만 0 또는 전체 맵 크기를 넘을 수 없다. 따라서 예외처리
					else if (tx >= 1) {
						--tx;
					}

					// tx - 1이 -1이 아니고 다음 이동할 좌표가 ROOT, START라면
					if (m_map[ty][tx].x == ROOT || m_map[ty][tx].x == START)
					{
						// 앞으로 갔던 값 되돌아오기
						++tx;

						blackLX = true;
						blackRoot = true;

						// 다음 벡터 방향 탐색 ty + 1이 최댓값을 넘지 않고 ty의 다음 좌표가 벽 앞에서 1칸 떨어져 있을때 참
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
							cout << "갈 수 없는 길" << endl;
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
						// 탐색하다가 END를 만나면 입력 지점의 끝 이기 때문에 반복문을 빠져나간다.
						if (m_map[ty][tx].x == END) {
							trueEND = true;
							break;
						}

						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].x += 1;
					}
				}

				// 아래로 이동
				else if ((searchRY && m_map[tx][ty].y != START) || (searchRY && m_map[tx][ty].y != ROOT) || (searchRY && m_map[tx][ty].y != END) || (searchRY && blackRoot))
				{
					// 탐색하다가 방향벡터가 모두 0이되면 포인트를 만난 것이기 때문에 이를 START로 바꿔준다.
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

					// ty는 현재 좌표를 가리키지만 0 또는 전체 맵 크기를 넘을 수 없다. 따라서 예외처리
					else if (ty <= m_size - 2) {
						++ty;
					}

					// ty + 1이 최대값이 아니고 다음 이동할 좌표가 ROOT, START라면
					if (m_map[ty][tx].y == ROOT || m_map[ty][tx].y == START)
					{
						// 앞으로 갔던 값 되돌아오기
						--ty;

						blackRY = true;
						blackRoot = true;

						// 다음 벡터 방향 탐색
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
							cout << "갈 수 없는 길" << endl;
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
						// 탐색하다가 END를 만나면 입력 지점의 끝 이기 때문에 반복문을 빠져나간다.
						if (m_map[ty][tx].x == END) {
							trueEND = true;
							break;
						}

						m_map[ty][tx].x = ROOT;
						m_map[ty][tx].y = ROOT;
						m_Vpos[vecSize].y -= 1;
					}
				}
				
				// 위로 이동
				else if ((searchLY && m_map[i][j].y != START) || (searchLY && m_map[i][j].y != ROOT) || (searchLY && m_map[i][j].y != END) || (searchLY && blackRoot))
				{
					// 탐색하다가 방향벡터가 모두 0이되면 포인트를 만난 것이기 때문에 이를 START로 바꿔준다.
					if (m_Vpos[vecSize].x == 0 && m_Vpos[vecSize].y == 0)
					{
						m_map[ty][tx].x = START;
						m_map[ty][tx].y = START;

						endpos = true;
						break;
					}

					// y가 0인 경우 전부 이동 한 것이므로 while문 나감
					else if (m_Vpos[vecSize].y == 0 && !blackRoot)
					{
						searchLY = false;
						break;
					}

					// tx는 현재 좌표를 가리키지만 0 또는 전체 맵 크기를 넘을 수 없다. 따라서 예외처리
					else if (ty >= 1) 
					{
						--ty;
					}

					// ty - 1이 -1이 아니고 다음 이동할 좌표가 ROOT, START라면
					if (m_map[ty][tx].y == ROOT || m_map[ty][tx].y == START)
					{

						// 앞으로 갔던 값 되돌아오기
						++ty;

						blackLY = true;
						blackRoot = true;

						// 다음 벡터 방향 탐색
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
						// 탐색하다가 END를 만나면 입력 지점의 끝 이기 때문에 반복문을 빠져나간다.
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
				cout << "갈 수 없는 길" << endl;
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




// 배경색 변경 멤버 함수
void Compare::ColorPrint(int color)
{
	char mark;
	using std::cout;
	switch (color)
	{
		// 입력 받지 못한 경우
	case WHITE:
		mark = 88;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (color << 4));
		cout << "| " << mark << " ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE | BLACK);
		break;

		// 움직일 길
	case GREEN:
		mark = 42;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (color << 4));
		cout << "| " << mark << " ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE | BLACK);
		break;

		// 지나가야 하는 좌표
	case BLUE:
		mark = 79;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (color << 4));
		cout << "| " << mark << " ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE | BLACK);
		break;

		// 색 정상으로 돌려 놓기
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
	const int POINT = 2147483647; // POINT : 도달해야 하는 목적지
	const int ROOT = 2147483646; // ROOT : 지나간 길
	const int START = 2147483645; // START : 시작 지점
	const int END = 2147483644; // END : 종료 지점

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