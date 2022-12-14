#ifndef MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
#define MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "vec3.hpp"
#include "vec4.hpp"

/** Mat44f: 4x4 matrix with floats
 *
 * See vec2f.hpp for discussion. Similar to the implementation, the Mat44f is
 * intentionally kept simple and somewhat bare bones.
 *
 * The matrix is stored in row-major order (careful when passing it to OpenGL).
 *
 * The overloaded operator () allows access to individual elements. Example:
 *    Mat44f m = ...;
 *    float m12 = m(1,2);
 *    m(0,3) = 3.f;
 *
 * The matrix is arranged as:
 *
 *   ⎛ 0,0  0,1  0,2  0,3 ⎞
 *   ⎜ 1,0  1,1  1,2  1,3 ⎟
 *   ⎜ 2,0  2,1  2,2  2,3 ⎟
 *   ⎝ 3,0  3,1  3,2  3,3 ⎠
 */
struct Mat44f
{
	float v[16];

	constexpr
	float& operator() (std::size_t aI, std::size_t aJ) noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
	constexpr
	float const& operator() (std::size_t aI, std::size_t aJ) const noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
};

// Identity matrix
constexpr Mat44f kIdentity44f = { {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
} };

// Common operators for Mat44f.
// Note that you will need to implement these yourself.

constexpr
Mat44f operator*( Mat44f const& aLeft, Mat44f const& aRight ) noexcept
{
	Mat44f res = {
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f
	};

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				res(i, j) += aLeft(i, k) * aRight(k, j);
			}
		}
	}
	return res;
}

constexpr
Vec4f operator*( Mat44f const& aLeft, Vec4f const& aRight ) noexcept
{
	return Vec4f{
		aRight.x*aLeft(0,0) + aRight.y*aLeft(0,1) +aRight.z*aLeft(0,2) +aRight.w*aLeft(0,3) ,
		aRight.x*aLeft(1,0) + aRight.y*aLeft(1,1) +aRight.z*aLeft(1,2) +aRight.w*aLeft(1,3) ,
		aRight.x*aLeft(2,0) + aRight.y*aLeft(2,1) +aRight.z*aLeft(2,2) +aRight.w*aLeft(2,3) ,
		aRight.x*aLeft(3,0) + aRight.y*aLeft(3,1) +aRight.z*aLeft(3,2) +aRight.w*aLeft(3,3) ,
	};
}

// Functions:

inline
Mat44f make_rotation_x( float aAngle ) noexcept
{
	return Mat44f{
		1, 0, 0, 0,
		0, std::cos(aAngle), -std::sin(aAngle), 0,
		0, std::sin(aAngle), std::cos(aAngle), 0,
		0, 0, 0, 1
	};
}


inline
Mat44f make_rotation_y( float aAngle ) noexcept
{
	return Mat44f{
		std::cos(aAngle), 0, std::sin(aAngle), 0,
		0, 1, 0, 0,
		-std::sin(aAngle), 0, std::cos(aAngle), 0,
		0, 0, 0, 1
	};
}

inline
Mat44f make_rotation_z( float aAngle ) noexcept
{
	return Mat44f{
		std::cos(aAngle), -std::sin(aAngle),  0, 0,
		std::sin(aAngle), std::cos(aAngle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

inline
Mat44f make_translation( Vec3f aTranslation ) noexcept
{
	return Mat44f{
		1, 0, 0, aTranslation.x,
		0, 1, 0, aTranslation.y,
		0, 0, 1, aTranslation.z,
		0, 0, 0, 1
	};
}

inline
Mat44f make_scaling( float aSX, float aSY, float aSZ ) noexcept
{
	return Mat44f{
		aSX, 0, 0, 0,
		0, aSY, 0, 0,
		0, 0, aSZ, 0,
		0, 0, 0, 1
	};
}


inline
Mat44f make_perspective_projection( float aFovInRadians, float aAspect, float aNear, float aFar ) noexcept
{
	float s = 1 / std::tan( aFovInRadians/2 );
	float sx = s / aAspect;

	float a = - ( aFar + aNear ) / ( aFar - aNear );
	float b = -2 * ( aFar * aNear ) / ( aFar - aNear );

	return Mat44f{
		sx, 0, 0, 0,
		0, s, 0, 0,
		0, 0, a, b,
		0, 0, -1, 0
	};
}
inline
Mat44f camMat(Vec3f position, Vec3f target, Vec3f up)
{
	//Have to calculate all 3 new vectors for this
	Vec3f D = normalize(position - target);
	Vec3f R = normalize(cross_product(up, D));
	Vec3f P = cross_product(D, R); //Have the calculate a new up vector instead of one passed through - P for Pitch

	Mat44f aRight = {
		1.f, 0.f, 0.f, -position.x,
		0.f, 1.f, 0.f, -position.y,
		0.f, 0.f, 1.f, -position.z,
		0.f, 0.f, 0.f, 1.f
	};

	Mat44f aLeft = {
		R.x, R.y, R.z, 0.f,
		P.x, P.y, P.z, 0.f,
		D.x, D.y, D.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	return aLeft * aRight;
}



#endif // MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
