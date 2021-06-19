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

struct markedvector
{
	int marker = 0;
	vector<int> body;
	vector<int> indexes;
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
	output.marker = 0;
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
	vector<impl> cleared;
	vector<impl> additional;
	vector<impl> output;
	int i;
	int j;
	if (parsed[0].marker == -1)
	{
		return vector<impl>{breaker};
	}
	for (i = 0; parsed[i].marker != -1; ++i)
	{
		for (j = i - 1; j >= 0; --j)
		{
			if (eqimpl(parsed[i], parsed[j]))
			{
				parsed[i].marker = -2;
			}
		}
		if (parsed[i].marker != -2)
		{
			cleared.push_back(parsed[i]);
		}
	}
	cleared.push_back(breaker);
	for (i = 0; cleared[i].marker!=-1; ++i)
	{
		for (j = i + 1;cleared[j].marker!=-1; ++j)
		{
			impl temp = merge(cleared[i], cleared[j]);
			if (temp.marker != -1)
			{
				cleared[i].marker = 1;
				cleared[j].marker = 1;
				additional.push_back(temp);
			}
		}
	}
	additional.push_back(breaker);
	additional = firstfase(additional);
	for (i = 0; cleared.begin() + i != cleared.end(); ++i)
	{
		if (cleared[i].marker == 0)
		{
			output.push_back(cleared[i]);
		}
	}
	for (i = 0; additional[i].marker != -1; ++i)
	{
		output.push_back(additional[i]);
	}
	output.push_back(breaker);
	return output;
}

vector<int> simplify(vector<impl> minimiser, impl minimised)
{
	int i;
	i = 0;
	vector<int> output;
	while (minimiser[i].marker != -1)
	{
		if (consume(minimiser[i], minimised))
		{
			output.push_back(1);
		}
		else
		{
			output.push_back(0);
		}
		++i;
	}
	return output;
}

markedvector noncorerecursive(markedvector search, vector<markedvector> goals)
{
	markedvector failedbranch;
	failedbranch.marker = -100;
	if (goals.empty())
	{
		return search;
	}
	if (search.indexes.empty())
	{
		return failedbranch;
	}
	int n = search.indexes[0];
	search.indexes.erase(search.indexes.begin());
	markedvector variant1 = noncorerecursive(search, goals);
	int i;
	for (i = 0; goals.begin() + i != goals.end(); ++i)
	{
		if (goals[i].body[n] == 1)
		{
			goals.erase(goals.begin() + i);
			--i;
		}
	}
	markedvector variant2 = noncorerecursive(search, goals);
	++variant2.marker;
	variant2.body.push_back(n);
	int a = variant1.marker;
	int b = variant2.marker;
	if (a < 0 && b < 0)
	{
		return failedbranch;
	}
	if (a < 0)
	{
		return variant2;
	}
	if (b < 0)
	{
		return variant1;
	}
	if (a>b)
	{
		return variant2;
	}
	return variant1;
}

vector<impl> secondfase(vector<impl> default, vector<impl> parsed)
{
	int i;
	int j;
	vector<int> indexes;
	markedvector noncore;
	vector<markedvector> simpled;
	vector<markedvector> noncored;
	for (i = 0; default[i].marker != -1; ++i)
	{
		vector<int> temp = simplify(parsed, default[i]);
		int count = -1;
		for (j = 0; temp.begin() + j != temp.end(); ++j)
		{
			if (temp[j] == 1)
			{
				if (count == -1)
				{
					count = j;
				}
				else
				{
					count = -4;
				}
			}
		}
		if (count >= 0)
		{
			parsed[count].marker = 1;
		}
		markedvector temp1;
		temp1.body = temp;
		simpled.push_back(temp1);
	}
	for (i = 0; parsed[i].marker != -1; ++i)
	{
		if (parsed[i].marker == 1)
		{
			indexes.push_back(i);
			for (j = 0; simpled.begin() + j != simpled.end(); ++j)
			{
				if (simpled[j].body[i] == 1)
				{
					simpled[j].marker = 1;
				}
			}
		}
		else
		{
			noncore.indexes.push_back(i);
		}
	}
	for (i = 0; simpled.begin() + i != simpled.end(); ++i)
	{
		if (simpled[i].marker == 0)
		{
			noncored.push_back(simpled[i]);
		}
	}
	markedvector additional = noncorerecursive(noncore, noncored);
	for (i = 0; i < additional.marker; ++i)
	{
		indexes.push_back(additional.body[i]);
	}
	vector<impl> output;
	for (i = 0; indexes.begin() + i != indexes.end(); ++i)
	{
		output.push_back(parsed[indexes[i]]);
	}
	impl breaker;
	breaker.marker = -1;
	output.push_back(breaker);
	return output;
}

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


int main()
{
	vector<impl> h = parse_input("0110", 1);
	vector<impl> f = firstfase(h);
	vector<impl> g = secondfase(h, f);
	cout << makestringdnf(g) << endl;
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
