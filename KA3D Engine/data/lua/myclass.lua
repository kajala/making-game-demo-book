testvar1 = "hello";
testvar2 = "world";
testvar3 = 1;
testvar4 = 23;
testvar5 = true;
testvar6 = false;
testvar7 = {1,2,3};

function f( x )

	_G.print( "Testing Lua C++ integration" );
	_G.print( "\nmyclass.lua method f called with parameter x = " .. x );
	
	_G.print( "\n* Calling C++ method add1(x,2), result=5:" );
	_G.print( "add1(x,2) == " .. add1(x,2) );

	_G.print( "\n* Calling C++ method add1(x,other().value()), result=7:" );
	_G.print( "add1(x,other().value()) == " .. add1(x,other().value()) );

	_G.print( "\n* Calling C++ method add2(x,other()), result=11:" );
	_G.print( "add2(x,other()) == " .. add2(x,other()) );

	_G.print( "\n* Calling C++ method sum(1,2,3), result=6:" );
	_G.print( "sum(1,2,3) == " .. sum(1,2,3) );

	-- _G.print( "\n* Calling C++ method add1(x,'Hello'), should fail:" );
	-- _G.print( "add1(x,'Hello') == " .. add1(x,'Hello') );
	
	return 123;
end
