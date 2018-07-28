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

	Will generate this file :

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
	


## Features

- Supports inherited classes, operators, default arguments, static variable, static const variable

## Features

- override are disabled for the moment.
- the prototype in pkg must be exactly like described in pkg (a modifier is planned)

## Supported Compilers

tosol makes use of C++11 **and** C++17 features. 

- Visual Studio 2015 Community (Visual C++ 14.0)+


## License

Sol is distributed with an MIT License. You can see LICENSE.txt for more info.