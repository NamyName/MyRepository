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

	};
}