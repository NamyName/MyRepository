#include "catch.hpp"
#include "lib.hpp"
#include "iostream"
#include "string"
#include "vector"

TEST_CASE("Something works") { REQUIRE(1 == 1); }

TEST_CASE("equalimpls")
		{
			int i = 5;
			vector<int>a(5);
			impl x;
			impl y;
			x.n = i;
			y.n = i;
			x.src = a;
			y.src = a;
			REQUIRE(eqimpl(x, y)==1);
		}

		TEST_CASE("unequalimpls")
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
			REQUIRE(eqimpl(x, y)==0);
		}
		
		TEST_CASE("mergeable")
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[3] = 0;
			impl x = merge(a, b);
			REQUIRE(x.src[3] == -1);
			x.src[3] = 1;
			REQUIRE(eqimpl(x, a)==1);
		}


		
		TEST_CASE("unmergeable")
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[3] = 0;
			b.src[1] = 1;
			impl x = merge(a, b);
			REQUIRE(x.marker == -1);
		}
		
		TEST_CASE("equnmergeable")
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl x = merge(a, a);
			REQUIRE(x.marker == -1);
		}
		
		TEST_CASE("consumable")
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[7] = 0;
			REQUIRE(consume(a, b)==1);
		}
		
		TEST_CASE("reverseunconsumable")
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[7] = 0;
			REQUIRE(consume(b, a)==0);
		}
		
		TEST_CASE("unconsumable")
		{
			impl a;
			a.n = 10;
			a.src = { 1, 0, 1, 1, -1, 1, 1, -1, 1, 0 };
			impl b = a;
			b.src[7] = 0;
			b.src[1] = 1;
			REQUIRE(consume(a, b)==0);
		}

		TEST_CASE("selfconsume")
		{
			impl a = inttoimpl(7, 4);
			REQUIRE(consume(a, a)==1);
		}
		
		TEST_CASE("inttoimpltotal")
		{
			int i = 5;
			int n = 3;
			impl t = inttoimpl(i, n);
			REQUIRE(t.marker == 0);
			REQUIRE(t.n == n);
			REQUIRE(t.src[0] == 1);
			REQUIRE(t.src[1] == 0);
			REQUIRE(t.src[2] == 1);
		}
		
		TEST_CASE("inttoimplzero")
		{
			int i = 0;
			int n = 8;
			impl t = inttoimpl(i, n);
			int f;
			for (f = 0; f < n; ++f)
			{
				REQUIRE(t.src[f]== 0);
			}
		}

		TEST_CASE("parse_inputdnf")
		{
			string main = "00001100";
			vector<impl> t = parse_input(main, 1);
			impl a;
			a.src = { 4 };
			t.push_back(a);
			REQUIRE(t[0].src[0] == 1);
			REQUIRE(t[0].src[1] == 0);
			REQUIRE(t[0].src[2] == 0);
			REQUIRE(t[2].marker == -1);
			REQUIRE(t[3].src[0] == 4);
		}

		TEST_CASE("parse_inputknf")
		{
			string main = "11110011";
			vector<impl> t = parse_input(main, 0);
			impl a;
			a.src = { 4 };
			t.push_back(a);
			REQUIRE(t[0].src[0] == 1);
			REQUIRE(t[0].src[1] == 0);
			REQUIRE(t[0].src[2] == 0);
			REQUIRE(t[2].marker == -1);
			REQUIRE(t[3].src[0] == 4);
		}
	    
		TEST_CASE("parse_inputcheckzero")
		{
			string main = "00000000";
			vector<impl>t = parse_input(main, 1);
			vector<impl>a;
			impl b;
			b.marker = -1;
			a.push_back(b);
			REQUIRE(t.size() == a.size());
		}
		
		TEST_CASE("firstfasesmall")
		{
			string main = "01";
			vector<impl>t = parse_input(main, 1);
			vector<impl>a = firstfase(t);
			impl check;
			check.n = 1;
			check.src = { 1 };
			REQUIRE(eqimpl(a[0], check)==1);
			REQUIRE(a[1].marker == -1);
		}
		
		TEST_CASE("firstfasegeneral")
		{
			string main = "0000011110011011";
			vector<impl>t = parse_input(main, 1);
			vector<impl>a = firstfase(t);
			REQUIRE(a[5].marker == -1);
		}

		TEST_CASE("firstfaseempty")
		{
			string main = "0000000000000000";
			vector<impl>t = parse_input(main, 1);
			vector<impl>a = firstfase(t);
			REQUIRE(a[0].marker == -1);
		}

		TEST_CASE("simplesimplify")
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
			REQUIRE(consume(d[2], a)==1);
			REQUIRE(e[0] == 1);
			REQUIRE(e[1] == 0);
			REQUIRE(e[2] == 1);
		}

		TEST_CASE("noncoreempty")
		{
			markedvector a;
			vector<markedvector> b;
			markedvector c = noncorerecursive(a, b);
			REQUIRE(c.body.empty()==1);
		}

		TEST_CASE("noncorefailed")
		{
			markedvector a;
			markedvector c;
			c.body = { 0 };
			vector<markedvector> b = { c };
			markedvector i = noncorerecursive(a, b);
			REQUIRE(i.marker == -100);
		}

		TEST_CASE("noncoresmall")
		{
			markedvector a;
			a.indexes = { 0, 1 };
			markedvector b;
			b.body = { 1, 0 };
			markedvector c = noncorerecursive(a, vector<markedvector> {b, b, b, b, b, b, b, b});
			REQUIRE(c.marker == 1);
			REQUIRE( c.body[0] == 0 );
		}
		
		TEST_CASE("noncoregeneral")
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
			REQUIRE(b.marker == 2);
		}

		TEST_CASE("secondfasesimplebig")
		{
			vector<impl> h = parse_input("1111111111111111111111111111111111111111111111111111111111111111", 1);
			vector<impl>b = firstfase(h);
			vector<impl>a = secondfase(h, b);
			REQUIRE(b[1].marker == -1);
			REQUIRE(b[0].src[1] == -1);
			REQUIRE(a[1].marker == -1);
			REQUIRE(a[0].src[1] == -1);
		}

		TEST_CASE("realfulltest1")
		{
			vector<impl> h = parse_input("0000011110011011", 1);
			vector<impl>b = firstfase(h);
			vector<impl>a = secondfase(h, b);
			REQUIRE(a[4].marker == -1);
		}

		TEST_CASE("realfulltest2")
		{
			vector<impl> h = parse_input("1111011000100111", 1);
			vector<impl>a = firstfase(h);
			vector<impl>b = secondfase(h, a);
			REQUIRE(b[4].marker == -1);
		}

		TEST_CASE("realfulltest3")
		{
			vector<impl> h = parse_input("0000111001111000", 1);
			vector<impl>a = firstfase(h);
			vector<impl>b = secondfase(h, a);
			REQUIRE(b[5].marker == -1);
		}
