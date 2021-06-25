#include "lib.hpp"
#include "iostream"
#include "string"
#include "vector"
using namespace std;
/**
\brief Функция преобразует импликанту в строку для вывода. Работает только для днф.
\param a Преобразуемая импликанта.
\return Возвращает строку, соответствующую импликанте.
*/
string impltostringdnf(impl a)
{
	string output = "(";
	int i;
	char counter = 'a';
	for (i = 0; i < a.n; ++i)
	{
		if (a.src[i] == 1)
		{
			output = output + counter + "*";
		}
		if (a.src[i] == 0)
		{
			output = output + "!" + counter + "*";
		}
		++counter;
	}
	output.erase(output.end()-1);
	output = output + ")";
	if (output == ")")
	{
		output = "1";
	}
	return output;
}

/**
\brief Функция преобразует импликанту в строку для вывода. Работает только для кнф.
\param a Преобразуемая импликанта.
\return Возвращает строку, соответствующую импликанте.
*/
string impltostringknf(impl a)
{
	string output = "(";
	int i;
	char counter = 'a';
	for (i = 0; i < a.n; ++i)
	{
		if (a.src[i] == 0)
		{
			output = output + counter + "+";
		}
		if (a.src[i] == 1)
		{
			output = output + "!" + counter + "+";
		}
		++counter;
	}
	output.erase(output.end() - 1);
	output = output + ")";
	if (output == ")")
	{
		output = "0";
	}
	return output;
}

/**
\brief Функция преобразует массив минимизированной днф в строку для вывода.
\param a Преобразуемый массив.
\return Возвращает строку, соответствующую массиву импликант.
*/
string makestringdnf(vector<impl> source)
{
	string output = "";
	int i;
	for (i = 0; source[i].marker != -1; ++i)
	{
		output = output + impltostringdnf(source[i]) + "+";
	}
	if (!output.empty())
	{
		output.erase(output.end() - 1);
	}
	return output;
}

/**
\brief Функция преобразует массив минимизированной кнф в строку для вывода.
\param a Преобразуемый массив.
\return Возвращает строку, соответствующую массиву импликант.
*/
string makestringknf(vector<impl> source)
{
	string output = "";
	int i;
	for (i = 0; source[i].marker != -1; ++i)
	{
		output = output + impltostringknf(source[i]) + "*";
	}
	if (!output.empty())
	{
		output.erase(output.end() - 1);
	}
	return output;
}


/// Функция main. Запускает консоль для взаимодействия с пользователем. 
int main()
{
	int i;
	int j;
	string a;
	while (1)
	{
		cout << "Press 1 to type in a vector." << endl;
		cout << "Or press 2 to quit." << endl;
		cin >> i;
		cout << endl;
		if (i == 2)
		{
			break;
		}
		if (i == 1)
		{
			cout << "Please type the vector in." << endl; 
			cin >> a;
			cout << "Press 1 to minimise dnf." << endl;
			cout << "Or press 2 to minimise knf." << endl;
			cin >> j;
			cout << endl;
			if (j == 1)
			{
				vector<impl> h = parse_input(a,1);
				vector<impl> f = firstfase(h);
				vector<impl> g = secondfase(h, f);
				cout << makestringdnf(g) << endl;
			}
			if (j == 2)
			{
				vector<impl> h = parse_input(a, 0);
				vector<impl> f = firstfase(h);
				vector<impl> g = secondfase(h, f);
				cout << makestringknf(g) << endl;
			}
		}
	}
	return 0;
}
