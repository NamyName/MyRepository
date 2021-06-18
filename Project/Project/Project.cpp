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

bool consume(impl consumer, impl consumed)
{
	if (consumer.n != consumed.n)
	{
		return 0;
	}
	int i;
	for (i = 0; i < consumer.n; ++i)
	{
		if (consumer.src[i] != consumed.src[i] && consumer.src[i]!=-1)
		{
			return 0;
		}
	}
	return 1;
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

vector<impl> firstfase(vector<impl> parsed)
{
	impl breaker;
	breaker.marker = -1;
	vector<impl>recursive;
	vector<impl> output;
	if (parsed[0].marker == -1)
	{
		return vector<impl>{breaker};
	}
	else
	{
		int i;
		i = 0;
		int j;
		while (parsed[i].marker != -1)
		{
			j = i + 1;
			while (parsed[j].marker != -1)
			{
				if (eqimpl(parsed[i], parsed[j]))
				{
					parsed.erase(parsed.begin() + j);
					--j;
				}
				else
				{
					impl temp = merge(parsed[i], parsed[j]);
					if (temp.marker != -1)
					{
						parsed[i].marker = 1;
						parsed[j].marker = 1;
						recursive.push_back(temp);
					}
				}
				++j;
			}
			++i;
		}
		recursive.push_back(breaker);
		i = 0;
		while (parsed[i].marker != -1)
		{
			if (parsed[i].marker == 0)
			{
				output.push_back(parsed[i]);
			}
			i++;
		}
		vector<impl> additional = firstfase(recursive);
		i = 0;
		while (additional[i].marker != -1)
		{
			output.push_back(additional[i]);
			i++;
		}
		output.push_back(additional[i]);
		return output;
	}
}


int main()
{
	return 0;
}

