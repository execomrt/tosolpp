## tosol 0.0.9

tosol is a command line application that convert tolua style (pkg) to Sol Lua API.

## Documentation


## Sneak Peek

tosol -o b2Vec2.inl b2Vec2.pkg


/// A 2D column vector.
struct b2Vec2
{
	/// Default constructor does nothing (for performance).
	b2Vec2();

	/// Construct using coordinates.
	b2Vec2(float32 xIn, float32 yIn) : x(xIn), y(yIn);

	/// Set this vector to all zeros.
	void SetZero();

	/// Set this vector to some specified coordinates.
	void Set(float32 x_, float32 y_);

	/// Negate this vector.
	b2Vec2 operator -() const { b2Vec2 v; v.Set(-x, -y); return v; }
	
	/// Read from and indexed element.
	float32 operator () (int32 i) const;

	/// Write to an indexed element.
	float32& operator () (int32 i);

	/// Add a vector to this vector.
	void operator += (const b2Vec2& v);

	/// Subtract a vector from this vector.
	void operator -= (const b2Vec2& v);

	/// Multiply this vector by a scalar.
	void operator *= (float32 a);

	/// Get the length of this vector (the norm).
	float32 Length() const;

	/// Get the length squared. For performance, use this instead of
	/// b2Vec2::Length (if possible).
	float32 LengthSquared() const;

	/// Convert this vector into a unit vector. Returns the length.
	float32 Normalize();

	/// Does this vector contain finite coordinates?
	bool IsValid() const;

	/// Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
	b2Vec2 Skew() const;

	float32 x, y;
};


/*
** Lua binding: b2Vec2
** Generated automatically by tosol++0.0.9 
*/
int tolua_b2Vec2_open(Lua_State* L)
{
 sol::state_view tosol_S(L);
 tosol_S.new_usertype<b2Vec2>("b2Vec2", 
 	sol::constructors<
 		b2Vec2(),
 		b2Vec2(float32 xIn, float32 yIn)>(),
 	"SetZero", &b2Vec2::SetZero,
 	"Set", &b2Vec2::Set,
 	"-", &b2Vec2::-,
 	"operator", sol::overload(
		sol::resolve<float32())>(&b2Vec2::operator),
		sol::resolve<float32())>(&b2Vec2::operator),
	);
 	"+=", &b2Vec2::+=,
 	"-=", &b2Vec2::-=,
 	"*=", &b2Vec2::*=,
 	"Length", &b2Vec2::Length,
 	"LengthSquared", &b2Vec2::LengthSquared,
 	"Normalize", &b2Vec2::Normalize,
 	"IsValid", &b2Vec2::IsValid,
 	"Skew", &b2Vec2::Skew,
 	"x", &b2Vec2::x,
 	"y", &b2Vec2::y);

 return 0;
}





## Features

-  Supports inherited classes, operators, default arguments, static variable, static const variable

## Supported Compilers

tosol makes use of C++11 **and** C++17 features. 

- Visual Studio 2015 Community (Visual C++ 14.0)+


## License

Sol is distributed with an MIT License. You can see LICENSE.txt for more info.