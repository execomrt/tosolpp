/*==============================================================================
$Id$
samples1.cpp
tosol++ is an extension of Sol, a tool to integrate C/CPP code with Lua
Uses the same format(PKG) than tolua++ (https://github.com/LuaDist/toluapp)
Sol2 https://github.com/ThePhD/sol2
==============================================================================*/
#define SOL_CHECK_ARGUMENTS 1
#include <array>
#include "sol/sol.hpp"

/// A 2D column vector.
namespace Samples2
{


	class Key
	{
	public:

		static const int Alt, BackSpace, CapsLock, ControlKey, Control, Delete, Down, End, Enter, Escape, Home, Insert, Left;
		static const int NumLock, PageDown, PageUp, RControlKey, Right, Shift, Space, Subtract, Tab, Up;
		static const int Tilde, Equals, OpenBracket, CloseBracket, Backslash, Semicolon, Quotes, Comma, Period, Slash, WindowsKey;
		static const int LButton, MButton, RButton, XButton1, XButton2;
		static const int F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;
		static const int A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
		static const int D0, D1, D2, D3, D4, D5, D6, D7, D8, D9;
		static const int NumPad0, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9, NumPadPeriod, NumPadDivide, NumPadMultiply, NumPadSubtract, NumPadAddition;

	};
}

#include "Sample2.inl" // Generated file

int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries();
	tolua_Sample2_open(lua);
	int x = 0;
	
	lua.script(R"(
		local keyValue = Key.OpenBracket)
)");
	
	return 0;
}