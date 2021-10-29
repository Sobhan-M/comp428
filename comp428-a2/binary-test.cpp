#include <iostream>
#include <bitset>
using namespace std;

string IntToBinaryString(int n, int bits) 
{
	string tempOutput = "";
	while(true)
	{
		// 4 = 100
		if (n == 0)
		{
			tempOutput += "0";
			break;
		}
		else if (n == 1)
		{
			tempOutput += "1";
			break;
		}
		else if (n%2 == 0)
		{
			tempOutput += "0";
			n /= 2; 
		}
		else if (n%2 == 1)
		{
			tempOutput += "1";
			n /= 2; 
		}
	}

	// Reversing.
	string output = "";
	for (int i = tempOutput.length() - 1; i >= 0 ; --i)
	{
		output += tempOutput[i];
	}

	while (bits - output.length() > 0)
	{
		output = "0" + output;
	}

	return output;
}

int BinaryStringToInt(string binary)
{
	int output = 0;
	int power = 1;
	for (int i = binary.length() - 1 ; i >= 0 ; --i)
	{
		if (binary[i] == '0')
		{
			output += 0*power;
		}
		else if (binary[i] == '1')
		{
			output += 1*power;
		}
		power = power*2;
	}

	return output;
}

int main()
{
	int n = 16;

	for (int i = 0 ; i <= n ; ++i)
	{
		string binary = IntToBinaryString(i, 5);
		cout << "Bitset: " << bitset<5>(i).to_string() << endl;
		cout << "Binary: " <<  binary << endl;
		cout << "Int: " << BinaryStringToInt(binary) << endl;
	}

	cout << endl;

	for (int i = 0; i < 5 ; ++i)
	{
		cout << bitset<5>(n).to_string()[i] << " ";
	}

	return 0;
}