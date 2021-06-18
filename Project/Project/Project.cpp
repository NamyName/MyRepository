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

impl merge(impl a, impl b)
{
	impl falsemerge;
	falsemerge.marker = -1;
	if (eqimpl(a, b) || a.n!=b.n)
	{
		return falsemerge;
	}
	int i, d;
	d = -1;
	for (i = 0; i < a.n; ++i)
	{
		if (a.src[i] != b.src[i])
		{
			if (d != -1)
			{
				return falsemerge;
			}
			else
			{
				d = i;
			}
		}
	}
	impl output = a;
	output.src[d] = -1;
	return output;
}

impl inttoimpl(int n, int k)//Ïðåîáðàçóåò ÷èñëî â èìïëèêïíòó. n - ÷èñëî, k - êîëè÷åñòâî ïåðåìåííûõ.
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

vector<impl> parse_input(string main, bool dnf, int l = 0) // Ïðåîáðàçóåò âåêòîð â ìàññèâ èìïëèêàíò, êîòîðûå äîëæíû âûïîëíÿòüñÿ.
{
	if (l == 0) //Íàõîäèò l, åñëè îíà íå çàäàíà.
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
	for (i = 0; i < main.size(); ++i) //Ïðîõîäèò ïî âåêòîðó è çàïèñûâàåò ïîäõîäÿùèå èìïëèêàíòû
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

