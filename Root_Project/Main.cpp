#include <iostream>
#include "Compare.h"

using namespace std;

int main(void) {


	int size, count;
	string index = " ";

	bool flag = true;
	cout << "������ ��ǥ�� �Է��ϼ��� ���͸� ġ�� �Է��� ����˴ϴ�. : ";
	cin >> size;

	getline(cin, index);
	
	while (true)
	{
		if (index.empty()) {
			cin.ignore();
			cout << "�߸��� �Է��Դϴ�." << endl;
			cout << "������ ��ǥ�� �Է��ϼ��� ���͸� ġ�� �Է��� ����˴ϴ�. : ";
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

	
	cout << endl << "�ƹ��ų� �Է��ϸ� �����մϴ�." << endl;
	cin.ignore();
	return 0;
}