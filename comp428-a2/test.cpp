#include <iostream>
#include <bitset>
using namespace std;

int main()
{
	int n = 16;

	for (int i = 0 ; i <= n ; ++i)
	{
		cout << bitset<4>(i).to_string() << " ";
	}

	cout << endl;

	for (int i = 0; i < 4 ; ++i)
	{
		cout << bitset<4>(n).to_string()[i] << " ";
	}

	return 0;
}