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
namespace Samples3
{

	class CharacterController
	{
	public:
		virtual void SetInput(const float angle, const float move, const float strafe = 0.0, const float jump = 0.0, const bool crouch = false, const float maxacceleration = 1.0, const float maxdeceleration = 0.5, const bool detailed = false, const float maxrotationspeed = 5.0f);
	};

}

#include "Sample3.inl" // Generated file

int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries();
	tolua_Sample3_open(lua);
	int x = 0;
	
	lua.script(R"(
		local keyValue = Key.OpenBracket)
)");
	
	return 0;
}