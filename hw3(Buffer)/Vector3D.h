/**
 * @file	Vector3D.h
 * @brief	Vector 3D
 * @author	waver
 * @date	2007-07-17
 * 
 * - History :
 * - 2007-07-17		waver		File created.
 *	
 * @todo
 * @bug
 */

#ifndef	WAFI_VECTOR3D_H
#define	WAFI_VECTOR3D_H

namespace MathTool
{

class Vector2D;
class Vector4D;

//Class Declaration
class Vector3D
{
public:
	union
	{
		float entries[3];
		struct
		{
			float x, y, z;
		};
		struct
		{
			float r, g, b;
		};
	};

	// construction
	Vector3D ();
	Vector3D (float fX, float fY, float fZ);
	Vector3D (const float *pfV);
	Vector3D (const Vector3D &rkV);

	// destructor
	~Vector3D ();

	// set
	void Set (float fX, float fY, float fZ);
	void Set (const float *pfV);
	void Set (const Vector3D &rkV);
	void SetX (float fX);
	void SetY (float fY);
	void SetZ (float fZ);

	// get
	float GetX () const;
	float GetY () const;
	float GetZ () const;

	// Load
	void LoadZero ();
	void LoadOne ();

	// access
	float& operator[] (int i);
	float operator[] (int i) const;

	// assignment
	Vector3D& operator= (const Vector3D &rkV);

	// comparison
	bool operator== (const Vector3D &rkV) const;
	bool operator!= (const Vector3D &rkV) const;

	// binary operator
	Vector3D operator+ (const Vector3D& rkV) const;
	Vector3D operator- (const Vector3D& rkV) const;
	Vector3D operator* (float fScalar) const;
	Vector3D operator/ (float fScalar) const;
	Vector3D operator* (const Vector3D& rhs) const;
	Vector3D operator/ (const Vector3D& rhs) const;
	friend Vector3D operator* (float fScalar, const Vector3D &rkV);

	// unary operator
	Vector3D operator- () const;
	Vector3D operator+ () const;

	// update operator
	Vector3D& operator+= (const Vector3D& rkV);
	Vector3D& operator-= (const Vector3D& rkV);
	Vector3D& operator*= (float fScalar);
	Vector3D& operator/= (float fScalar);
	Vector3D& operator*=(const Vector3D& rhs);
	Vector3D& operator/=(const Vector3D& rhs);

	// vector length
	float GetLength () const;
	float GetSquareLength () const;

	// vector operation
	float Dot (const Vector3D &rkV) const;
	Vector3D Cross (const Vector3D& rkV) const;							// right-handed rule
	Vector3D UnitCross (const Vector3D& rkV) const;

	// normal vector
	void Normalize ();
	Vector3D GetNormalize () const;
};

//Type Definition
typedef Vector3D Point3D;

//Global Function Declaration
float Distance(const Point3D &p1, const Point3D &p2);
float DistanceSquared(const Point3D &p1, const Point3D &p2);


//Global Inline Function
inline float Distance(const Point3D &p1, const Point3D &p2)
{
	return (p1 - p2).GetLength();
}
inline float DistanceSquared(const Point3D &p1, const Point3D &p2)
{
	return (p1 - p2).GetSquareLength();
}

}
#endif