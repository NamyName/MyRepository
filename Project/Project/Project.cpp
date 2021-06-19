#include "stdafx.h"
#include "iostream"
#include "string"
#include "vector"
using namespace std;

/**
\brief Класс, описывающий логическую импликанту.
 
\param n Количество логических переменных в составе импликанты. Задаётся натуральным числом.
\param src Зависимость импликанты от значения переменных. Задаётся массивом натуральных чисел соответствующих переменным. 
1 - импликанта выполняется, когда соответствующая переменная верна;0 - наоборот; -1 - импликанта не зависит от переменной.
\param marker Специальный натуральный маркер для произвольных целей; -1 задаёт конец массивов импликант в данном файле.
Значение по умолчанию = 0.
*/
struct impl
{
	int n; 
	vector<int> src;  
	int marker = 0; 

};

/**
\brief Класс, содержащий пару массивов и  маркер. Используется для удобства.

\param body,indexes - массивы натуральных чисел.
\param marker Маркер, задающийся натуральным числом.
*/
struct markedvector
{
	int marker = 0;
	vector<int> body;
	vector<int> indexes;
};

/**
\brief Функция, проверяющая импликанты на равенство. Не учитывает маркер.

\param a,b Сравниваемые импликанты.
\return Возвращает 1, если импликанты равны и 0 в другом случае.
/*
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

/**
\brief Функция, склеивающая две импликанты.

Функция, склеивающая импликанты или возвращающая маркированную импликанту, если это невозможно.
\param a,b Собственно склеиваемые импликанты.
\return Возвращает импликанту, являющуюся склейкой аргументов. При невозможности склейки возвращает импликанту с маркером -1.
\warning Данная функция не рассчитан на работу с импликантами с маркером = -1. 
*/
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

/**
\brief Функция, проверяющая поглощаемость одной импликанты другой.

\param consumer Поглощающая импликанта.
\param consumed Поглощаемая импликанта
\return Возвращает 1, если поглощение возможно и 0 в ином случае.
\warning Функция не симметрична!
*/
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

/**
\brief Функция, преобразующая число в импликанту.

Функция, преобразующая число в импликанту аналогично переводу в двоичную систему счисления.
\param n Преобразуемое число.
\param k Количество переменых, от которых зависит искомая импликанту.
\return Возвращает импликанту.
*/
impl inttoimpl(int n, int k)
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

/**
\brief Функция преобразующая вектор логической функции в массив импликант.

Функция преобразующая вектор логической функции в массив импликант, соответствующийй совершенной форме.
\param main Собственно строка, являющаяся преобразуемым вектором.
\param dnf Bool параметр, определяющий ведется поиск дизъюнктивной или конъюнктивной формы. 1 для дизъюнктивной.
\param l Количество переменных вектора. Является необязательным параметром и может быть вычислен самой функцией.
\return Возвращает массив импликант, соответствующий выбранной форме.
*/
vector<impl> parse_input(string main, bool dnf, int l = 0) 
{
	if (l == 0) 
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

/**
\brief Выполняет первую часть алгоритма Квайна.

Выполняет первую часть алгоритма Квайна, склеивая элементы совершенной формы. 
\param parsed Массив импликант, которые требуется склеить.
\return Возвращает массив склеенных импликант.
*/
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

/**
Преобразует исходную импликанту в массив индексов импликант, получившихся после склейки, которые её поглощают. 
Упрощает минимизацию на втором этапе алгоритма Квайна.
\param minimiser Массив импликант.
\param minimised Импликанта, которую требуется преобразовать в массив.
\return Возвращает массив длиной равный minimiser. Элементы массива принимают значения 0 и 1.
При значении 1 на некой позиции импликанта поглощается элементом minimiser на той же позиции. При 0 - нет.
*/
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

/**
Функция минимизирующая количество импликант вне ядра Тупиковой Нормальной Формы.
Работает рекурсивно.
\param search Элемент класса markedvector.
В параметре indexeses содержатся индексы возможных импликант в тупиковой нормальной форме.
В параметре body содержится последовательность индексов из прошлых вызовов в рекурсии. Пуст при изначальном вызове.
В параметре marcer содержится длина последовательности индексов.
\param goals Список импликант исходной формы функции, не поглощенных ядром.
Параметры indexeses и marker не значимы внутри данной функции.
В параметре body содержится импликанта в формате описанном в return функции simplify.
\return Возвращает массив аналогичный массиву search, но содержащийй в body минимальную последовательность элементов из search, поглощающую
весь массив goals.
\todo Мемоизация и вычисление всех тупиковых форм.
*/
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

/**
\brief Функция выполняет второй этап алгоритма Квайна.

Функция выполняет второй этап алгоритма Квайна, проводя поглощение исходных импликант совершенной формы импликантами, полученными после первого этапа. 
\param default Маассив исходных импликант. Вычисляется в parse_input.
\param parsed Массив импликант, полученных после первого этапа. Вычисляется в firstfase.
\return Возврощает массив импликант Минимальной или Тупиковой формы.
*/
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


// Функция main. Запускает консоль для взаимодействия с пользователем. 
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
