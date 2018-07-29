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
namespace Samples1
{
	struct b2Vec2
	{
		/// Default constructor does nothing (for performance).
		b2Vec2() : x(0), y(0) {}

		/// Construct using coordinates.
		b2Vec2(const float xIn, const float yIn) : x(xIn), y(yIn) {}

		/// Set this vector to all zeros.
		void SetZero() {}

		/// Set this vector to some specified coordinates.
		void Set(float x_, float y_) { x = x_; y = y_; }

		/// Negate this vector.
		b2Vec2 operator -() const { return b2Vec2(-x, -y); }

		/// Read/Write to an indexed element.
		float& operator [] (int i) { return i ? y : x; }

		/// Add a vector to this vector.
		void operator += (const b2Vec2& v) { x += v.x; y += v.y; }

		/// Add a vector to this vector.
		b2Vec2 operator + (const b2Vec2& a) { return b2Vec2(a.x + x, a.y + y); }

		/// Subtract a vector to this vector.
		b2Vec2 operator - (const b2Vec2& a) { return b2Vec2(x-a.x, y-a.y); }

		/// Subtract a vector from this vector.
		void operator -= (const b2Vec2& v) { x -= v.x; y -= v.y; }

		/// Multiply this vector by a scalar.
		void operator * (const b2Vec2& a) { *this; }
		void operator * (float s) { x *= s; y*=s; }

		/// Get the length of this vector (the norm).
		float Length() const { return sqrtf(LengthSquared()); }

		/// Get the length squared. For performance, use this instead of
		/// b2Vec2::Length (if possible).
		float LengthSquared() const { return x*x+y*y; }

		/// Convert this vector into a unit vector. Returns the length.
		float Normalize() { float ret = Length(); x /= ret; y /= ret; return ret; }

		/// Does this vector contain finite coordinates?
		bool IsValid() const { return true; }

		/// Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
		b2Vec2 Skew() const { return *this; }

		float x, y;

		std::string ToString() { return std::to_string(x) + ", " + std::to_string(y); }

		static const b2Vec2 ZERO;
	};

	const b2Vec2 b2Vec2::ZERO;
};



#include "Sample1.inl" // Generated file



int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries();
	tolua_Sample1_open(lua);
	int x = 0;
	
	lua.script(R"(
		local pos = Vec2(1, 2)
		pos:Set(3, 4)
		pos = pos + Vec2(2.0, 3.0)
		print(pos:Length())
)");
	
	return 0;
}