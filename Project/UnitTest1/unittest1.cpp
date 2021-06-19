#include "stdafx.h"
#include "CppUnitTest.h"
#include "iostream"
#include "string"
#include "vector"
#include "C:\\Users\\Serg\\Documents\\Visual Studio 2013\\Projects\\Project\\Project\\Project.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(equalimpls)
		{
			int i = 5;
			vector<int>a(5);
			impl x;
			impl y;
			x.n = i;
			y.n = i;
			x.src = a;
			y.src = a;
			Assert::IsTrue(eqimpl(x, y));
		}

		TEST_METHOD(unequalimpls)
		{
			int i = 5;
			vector<int>a(5);
			impl x;
			impl y;
			x.n = i;
			y.n = i;
			x.src = a;
			a[2] = 6;
			y.src = a;
			Assert::IsFalse(eqimpl(x, y));
		}
		
		TEST_METHOD(mergeable)
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[3] = 0;
			impl x = merge(a, b);
			Assert::AreEqual(x.src[3], -1);
			x.src[3] = 1;
			Assert::IsTrue(eqimpl(x, a));
		}


		
		TEST_METHOD(unmergeable)
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[3] = 0;
			b.src[1] = 1;
			impl x = merge(a, b);
			Assert::AreEqual(x.marker, -1);
		}
		
		TEST_METHOD(equnmergeable)
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl x = merge(a, a);
			Assert::AreEqual(x.marker, -1);
		}
		
		TEST_METHOD(consumable)
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[7] = 0;
			Assert::IsTrue(consume(a, b));
		}
		
		TEST_METHOD(reverseunconsumable)
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[7] = 0;
			Assert::IsFalse(consume(b, a));
		}
		
		TEST_METHOD(unconsumable)
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[7] = 0;
			b.src[1] = 1;
			Assert::IsFalse(consume(a, b));
		}

		TEST_METHOD(selfconsume)
		{
			impl a = inttoimpl(7, 4);
			Assert::IsTrue(consume(a, a));
		}
		
		TEST_METHOD(inttoimpltotal)
		{
			int i = 5;
			int n = 3;
			impl t = inttoimpl(i, n);
			Assert::AreEqual(t.marker, 0);
			Assert::AreEqual(t.n, n);
			Assert::AreEqual(t.src[0], 1);
			Assert::AreEqual(t.src[1], 0);
			Assert::AreEqual(t.src[2], 1);
		}
		
		TEST_METHOD(inttoimplzero)
		{
			int i = 0;
			int n = 8;
			impl t = inttoimpl(i, n);
			int f;
			for (f = 0; f < n; ++f)
			{
				Assert::AreEqual(t.src[f], 0);
			}
		}

		TEST_METHOD(parse_inputdnf)
		{
			string main = "00001100";
			vector<impl> t = parse_input(main, 1);
			impl a;
			a.src = { 4 };
			t.push_back(a);
			Assert::AreEqual(t[0].src[0], 1);
			Assert::AreEqual(t[0].src[1], 0);
			Assert::AreEqual(t[0].src[2], 0);
			Assert::AreEqual(t[2].marker, -1);
			Assert::AreEqual(t[3].src[0], 4);
		}

		TEST_METHOD(parse_inputknf)
		{
			string main = "11110011";
			vector<impl> t = parse_input(main, 0);
			impl a;
			a.src = { 4 };
			t.push_back(a);
			Assert::AreEqual(t[0].src[0], 1);
			Assert::AreEqual(t[0].src[1], 0);
			Assert::AreEqual(t[0].src[2], 0);
			Assert::AreEqual(t[2].marker, -1);
			Assert::AreEqual(t[3].src[0], 4);
		}
	    
		TEST_METHOD(parse_inputcheckzero)
		{
			string main = "00000000";
			vector<impl>t = parse_input(main, 1);
			vector<impl>a;
			impl b;
			b.marker = -1;
			a.push_back(b);
			Assert::AreEqual(t.size(), a.size());
		}
		
		TEST_METHOD(firstfasesmall)
		{
			string main = "01";
			vector<impl>t = parse_input(main, 1);
			vector<impl>a = firstfase(t);
			impl check;
			check.n = 1;
			check.src = { 1 };
			Assert::IsTrue(eqimpl(a[0], check));
			Assert::AreEqual(a[1].marker, -1);
		}
		
		TEST_METHOD(firstfasegeneral)
		{
			string main = "0000011110011011";
			vector<impl>t = parse_input(main, 1);
			vector<impl>a = firstfase(t);
			Assert::AreEqual(a[5].marker, -1);
		}

		TEST_METHOD(firstfaseempty)
		{
			string main = "0000000000000000";
			vector<impl>t = parse_input(main, 1);
			vector<impl>a = firstfase(t);
			Assert::AreEqual(a[0].marker, -1);
		}

		TEST_METHOD(simplesimplify)
		{
			impl a = inttoimpl(7, 4);
			impl b = a;
			b.src[1] = -1;
			impl c = a;
			c.src[0] = 1;
			impl breaker;
			breaker.marker = -1;
			vector<impl>d = { b, c, a, breaker };
			vector<int>e = simplify(d, a);
			Assert::IsTrue(consume(d[2], a));
			Assert::AreEqual(e[0], 1);
			Assert::AreEqual(e[1], 0);
			Assert::AreEqual(e[2], 1);
		}

		TEST_METHOD(noncoreempty)
		{
			markedvector a;
			vector<markedvector> b;
			markedvector c = noncorerecursive(a, b);
			Assert::IsTrue(c.body.empty());
		}

		TEST_METHOD(noncorefailed)
		{
			markedvector a;
			markedvector c;
			c.body = { 0 };
			vector<markedvector> b = { c };
			markedvector i = noncorerecursive(a, b);
			Assert::AreEqual(i.marker, -100);
		}

		TEST_METHOD(noncoresmall)
		{
			markedvector a;
			a.indexes = { 0, 1 };
			markedvector b;
			b.body = { 1, 0 };
			markedvector c = noncorerecursive(a, vector<markedvector> {b, b, b, b, b, b, b, b});
			Assert::AreEqual(c.marker, 1);
			Assert::AreEqual( c.body[0], 0 );
		}
		
		TEST_METHOD(noncoregeneral)
		{
			markedvector a;
			a.indexes = { 0, 1, 2, 3};
			markedvector c;
			c.body = { 0, 0, 1, 1 };
			markedvector d;
			d.body = { 0, 1, 0, 1 };
			markedvector e;
			e.body = { 1, 1, 0, 0 };
			vector<markedvector> j = { c, d, e };
			markedvector b = noncorerecursive(a, j);
			Assert::AreEqual(b.marker, 2);
		}

		TEST_METHOD(secondfasesimplebig)
		{
			vector<impl> h = parse_input("1111111111111111111111111111111111111111111111111111111111111111", 1);
			vector<impl>b = firstfase(h);
			vector<impl>a = secondfase(h, b);
			Assert::AreEqual(b[1].marker, -1);
			Assert::AreEqual(b[0].src[1], -1);
			Assert::AreEqual(a[1].marker, -1);
			Assert::AreEqual(a[0].src[1], -1);
		}

		TEST_METHOD(realfulltest1)
		{
			vector<impl> h = parse_input("0000011110011011", 1);
			vector<impl>b = firstfase(h);
			vector<impl>a = secondfase(h, b);
			Assert::AreEqual(a[4].marker, -1);
		}

		TEST_METHOD(realfulltest2)
		{
			vector<impl> h = parse_input("1111011000100111", 1);
			vector<impl>a = firstfase(h);
			vector<impl>b = secondfase(h, a);
			Assert::AreEqual(b[4].marker, -1);
		}

		TEST_METHOD(realfulltest3)
		{
			vector<impl> h = parse_input("0000111001111000", 1);
			vector<impl>a = firstfase(h);
			vector<impl>b = secondfase(h, a);
			Assert::AreEqual(b[5].marker, -1);
		}

	};
}
