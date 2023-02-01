#include <iostream>
#include "Compare.h"

using namespace std;

int main(void) {


	int size, count;
	string index = " ";

	bool flag = true;
	cout << "범위와 좌표를 입력하세요 엔터를 치면 입력이 종료됩니다. : ";
	cin >> size;

	getline(cin, index);
	
	while (true)
	{
		if (index.empty()) {
			cin.ignore();
			cout << "잘못된 입력입니다." << endl;
			cout << "범위와 좌표를 입력하세요 엔터를 치면 입력이 종료됩니다. : ";
			getline(cin, index);
		}
		else {
			break;
		}
	}

	Compare a(size);

	count = a.Sort(index);
	a.Root(count);

	a.Print();

	
	cout << endl << "아무거나 입력하면 종료합니다." << endl;
	cin.ignore();
	return 0;
}