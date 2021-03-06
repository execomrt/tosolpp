/*
** Lua binding: Sample1
** Generated automatically by tosol++0.1.0 
*/
using namespace Samples1;
inline static b2Vec2 tolua_Sample1_b2Vec2_0()
{
 return b2Vec2();
}
inline static b2Vec2 tolua_Sample1_b2Vec2_1(const float xIn, const float yIn)
{
 return b2Vec2(xIn, yIn);
}
inline static const b2Vec2 tolua_Sample1_b2Vec2_ZERO_get(void)
{
 return b2Vec2::ZERO; 
}

int tolua_Sample1_open(lua_State* L)
{
 sol::state_view tosol_S(L);
 tosol_S.new_usertype<b2Vec2>("Vec2", 
 	sol::constructors<
 		b2Vec2(),
 		b2Vec2(const float xIn, const float yIn)>(),
 	"SetZero", &b2Vec2::SetZero,
 	"Set", &b2Vec2::Set,
 	sol::meta_function::subtraction, sol::overload(
		sol::resolve<b2Vec2() const>(&b2Vec2::operator-),
		sol::resolve<b2Vec2(const b2Vec2&)>(&b2Vec2::operator-)
	),
 	sol::meta_function::index, &b2Vec2::operator[],
 	sol::meta_function::addition, &b2Vec2::operator+,
 	sol::meta_function::multiplication, sol::overload(
		sol::resolve<void(const b2Vec2&)>(&b2Vec2::operator*),
		sol::resolve<void(float)>(&b2Vec2::operator*)
	),
 	"Length", &b2Vec2::Length,
 	"LengthSquared", &b2Vec2::LengthSquared,
 	"Normalize", &b2Vec2::Normalize,
 	"IsValid", &b2Vec2::IsValid,
 	"Skew", &b2Vec2::Skew,
 	"ToString", &b2Vec2::ToString,
 	"x", &b2Vec2::x,
 	"y", &b2Vec2::y,
 	"ZERO", sol::property(&tolua_Sample1_b2Vec2_ZERO_get)
);


 return 0;
}
