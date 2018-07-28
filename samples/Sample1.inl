/*
** Lua binding: Sample1
** Generated automatically by tosol++0.0.9 
*/
using namespace Samples1;
inline static b2Vec2 tolua_Sample1_b2Vec2_0(void)
{
 return b2Vec2();
}
inline static b2Vec2 tolua_Sample1_b2Vec2_1(float xIn, float yIn)
{
 return b2Vec2(xIn, yIn);
}
int tolua_Sample1_open(sol::state & tosol_S)
{
 tosol_S.new_usertype<b2Vec2>("Vec2", 
 	sol::constructors<
 		b2Vec2(void),
 		b2Vec2(float xIn, float yIn)>(),
 	"SetZero", &b2Vec2::SetZero,
 	"Set", &b2Vec2::Set,
 	sol::meta_function::unary_minus,sol::resolve<b2Vec2() const>(&b2Vec2::operator-),
 	sol::meta_function::index, &b2Vec2::operator[],
 	sol::meta_function::addition, &b2Vec2::operator+,
 	sol::meta_function::subtraction,sol::resolve<b2Vec2(const b2Vec2&)>(&b2Vec2::operator-),
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
 	"ZERO", sol::property([]() { return b2Vec2::ZERO; }));

tosol_S["Vec2"] =
	sol::overload(
		sol::resolve<b2Vec2(void) >(&tolua_Sample1_b2Vec2_0), 
		sol::resolve<b2Vec2(float xIn, float yIn) >(&tolua_Sample1_b2Vec2_1)
	);
 return 0;
}
 