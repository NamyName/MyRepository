#include "stdafx.h"
#include "iostream"
#include "string"
#include "vector"
using namespace std;

struct impl
{
	int n; //
	vector<int> src; // 
	int marker = 0; //

};

bool eqimpl(impl a, impl b)
{
	if (a.n != b.n)
	{
		return 0;
	}
	int i;
	for (i = 0; i < a.n; ++i)
	{
		if (a.src[i] != b.src[i])
		{
			return 0;
		}
	}
	return 1;
}

impl inttoimpl(int n, int k)//Преобразует число в импликпнту. n - число, k - количество переменных.
{
	impl output;
	output.n = k;
	vector<int>a(k);
	int i = 0;
	while (n > 0)
	{
		if (n % 2 == 1)
		{
			a[k-1-i] = 1;
			--n;
		}
		n = n / 2;
		++i;
	}
	output.src = a;
	return output;
}

vector<impl> parse_input(string main, bool dnf, int l = 0) // Преобразует вектор в массив импликант, которые должны выполняться.
{
	if (l == 0) //Находит l, если она не задана.
	{
		int temp = main.size();
		while (temp > 1)
		{
			temp = temp / 2;
			++l;
		}

	}
	vector<impl> output;
	int i;
	for (i = 0; i < main.size(); ++i) //Проходит по вектору и записывает подходящие импликанты
	{
		if (dnf && main.substr(i, 1) == "1")
		{
			output.push_back(inttoimpl(i, l));
		}
		if (!dnf && main.substr(i, 1) == "0")
		{
			output.push_back(inttoimpl(i, l));
		}
	}
	impl breaker;
	breaker.marker = -1;
	output.push_back(breaker);
	return output;
}

int main()
{
	return 0;
}

