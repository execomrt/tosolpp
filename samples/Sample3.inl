/*
** Lua binding: Sample3
** Generated automatically by tosol++0.1.0 
*/
using namespace Samples3;

int tolua_Sample3_open(lua_State* L)
{
 sol::state_view tosol_S(L);
 sol::table _Game = tosol_S.create_named_table("Game"); 
 _Game.new_simple_usertype<CharacterController>("CharacterController", 
 	"SetAnimationFrame", sol::overload(
		sol::resolve<void(const double&, const float, const std::string&)>(&CharacterController::SetAnimationFrame),
		sol::resolve<void(const double&, const float, const std::string&, const bool)>(&CharacterController::SetAnimationFrame)
	)
);


 return 0;
}
 