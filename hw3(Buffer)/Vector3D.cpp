/**
 * @file	Vector3D.cpp
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

#include "Vector3D.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdio>
using namespace MathTool;

//------------------------------------------------------------------------------------------------
Vector3D::Vector3D()
{
	// uninitialized for performance in array construction
}
//------------------------------------------------------------------------------------------------
Vector3D::Vector3D(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}
//------------------------------------------------------------------------------------------------
Vector3D::Vector3D(const float *pfV)
{
	x = pfV[0];
	y = pfV[1];
	z = pfV[2];
}
//------------------------------------------------------------------------------------------------
Vector3D::Vector3D(const Vector3D &rkV)
:
x(rkV.x),
y(rkV.y),
z(rkV.z)
{
}
//------------------------------------------------------------------------------------------------
Vector3D::~Vector3D()
{
	// empty
}
//------------------------------------------------------------------------------------------------
void Vector3D::Set(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}
//------------------------------------------------------------------------------------------------
void Vector3D::Set(const float *pfV)
{
	x = pfV[0];
	y = pfV[1];
	z = pfV[2];
}
//------------------------------------------------------------------------------------------------
void Vector3D::Set(const Vector3D &rkV)
{
	x = rkV.x;
	y = rkV.y;
	z = rkV.z;
}
//------------------------------------------------------------------------------------------------
void Vector3D::SetX(float fX)
{
	x = fX;
}
//------------------------------------------------------------------------------------------------
void Vector3D::SetY(float fY)
{
	y = fY;
}
//------------------------------------------------------------------------------------------------
void Vector3D::SetZ(float fZ)
{
	z = fZ;
}
//------------------------------------------------------------------------------------------------
float Vector3D::GetX() const
{
	return x;
}
//------------------------------------------------------------------------------------------------
float Vector3D::GetY() const
{
	return y;
}
//------------------------------------------------------------------------------------------------
float Vector3D::GetZ() const
{
	return z;
}
//------------------------------------------------------------------------------------------------
void Vector3D::LoadZero()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}
//------------------------------------------------------------------------------------------------
void Vector3D::LoadOne()
{
	x = 1.0;
	y = 1.0;
	z = 1.0;
}
//------------------------------------------------------------------------------------------------
float& Vector3D::operator[] (int i)
{
	assert( 0 <= i && i <= 2 );
	return entries[i];
}
//------------------------------------------------------------------------------------------------
float Vector3D::operator[] (int i) const
{
	assert( 0 <= i && i <= 2 );
	return entries[i];
}
//------------------------------------------------------------------------------------------------
Vector3D& Vector3D::operator=(const Vector3D &rkV)
{
	x = rkV.x;
	y = rkV.y;
	z = rkV.z;
	return *this;
}
//------------------------------------------------------------------------------------------------
bool Vector3D::operator== (const Vector3D &rkV) const
{
	if( x==rkV.x && y==rkV.y && z==rkV.z)
		return true;

	return false;
}
//------------------------------------------------------------------------------------------------
bool Vector3D::operator!= (const Vector3D &rkV) const
{
	return !((*this)==rkV);
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::operator+ (const Vector3D& rkV) const
{
	return Vector3D(
		x + rkV.x, 
		y + rkV.y,
		z + rkV.z);
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::operator- (const Vector3D& rkV) const
{
	return Vector3D(
		x - rkV.x, 
		y - rkV.y,
		z - rkV.z);
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::operator* (float fScalar) const
{
	return Vector3D(
		x * fScalar, 
		y * fScalar,
		z * fScalar);
}
//------------------------------------------------------------------------------------------------
Vector3D MathTool::operator* (float fScalar, const Vector3D &rkV)
{
	return Vector3D(
		rkV.x * fScalar, 
		rkV.y * fScalar,
		rkV.z * fScalar);
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::operator/ (float fScalar) const
{
	assert(fScalar!=0.0);

	Vector3D kOut;
	if( fScalar != 0.0){
		kOut.x = x/fScalar;
		kOut.y = y/fScalar;
		kOut.z = z/fScalar;
	}
	else{
		kOut.x = 0.0;
		kOut.y = 0.0;
		kOut.z = 0.0;
	}
	return kOut;
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::operator*(const Vector3D& rhs) const
{
	return Vector3D(
		x * rhs.x,
		y * rhs.y,
		z * rhs.z);
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::operator/(const Vector3D& rhs) const
{
	assert(rhs.x!=0.0);
	assert(rhs.y!=0.0);
	assert(rhs.z!=0.0);

	return Vector3D(
		x / rhs.x,
		y / rhs.y,
		z / rhs.z);
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::operator- () const
{
	return Vector3D( -x, -y, -z );
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::operator+ () const
{
	return *this;
}
//------------------------------------------------------------------------------------------------
Vector3D& Vector3D::operator+= (const Vector3D& rkV)
{
	x += rkV.x;
	y += rkV.y;
	z += rkV.z;
	return *this;
}
//------------------------------------------------------------------------------------------------
Vector3D& Vector3D::operator-= (const Vector3D& rkV)
{
	x -= rkV.x;
	y -= rkV.y;
	z -= rkV.z;
	return *this;
}
//------------------------------------------------------------------------------------------------
Vector3D& Vector3D::operator*= (float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	return *this;
}
//------------------------------------------------------------------------------------------------
Vector3D& Vector3D::operator/= (float fScalar)
{
	assert(fScalar!=0.0);

	if( fScalar != 0.0){
		x = x / fScalar;
		y = y / fScalar;
		z = z / fScalar;
	}
	else{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	return *this;
}
//------------------------------------------------------------------------------------------------
Vector3D& Vector3D::operator*=(const Vector3D& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}
//------------------------------------------------------------------------------------------------
Vector3D& Vector3D::operator/=(const Vector3D& rhs)
{
	assert(rhs.x!=0.0);
	assert(rhs.y!=0.0);
	assert(rhs.z!=0.0);
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	return *this;
}
//------------------------------------------------------------------------------------------------
/*std::ostream& MathTool::operator<< (std::ostream& out, Vector3D &rkV)
{
	printf("%10.4f\t%10.4f\t%10.4f", rkV.x, rkV.y, rkV.z);
	return out;
}*/
//------------------------------------------------------------------------------------------------
float Vector3D::GetLength () const
{
	return sqrt( x * x + y * y + z * z );
}
//------------------------------------------------------------------------------------------------
float Vector3D::GetSquareLength () const
{
	return x * x + y * y + z * z;
}
//------------------------------------------------------------------------------------------------
float Vector3D::Dot(const Vector3D &rkV) const
{
	return x * rkV.x + y * rkV.y + z * rkV.z;
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::Cross (const Vector3D& rkV) const
{
	return Vector3D(
		y * rkV.z - z * rkV.y,
		z * rkV.x - x * rkV.z,
		x * rkV.y - y * rkV.x);
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::UnitCross (const Vector3D& rkV) const
{
	Vector3D kCross(
		y * rkV.z - z * rkV.y,
		z * rkV.x - x * rkV.z,
		x * rkV.y - y * rkV.x);
	kCross.Normalize();
	return kCross;	
}
//------------------------------------------------------------------------------------------------
void Vector3D::Normalize()
{
	float fLength = GetLength();
	assert(fLength!=0.0);

	if( fLength!= 0.0 ){
		float fInvertLength = 1.0f / fLength;
		x *= fInvertLength;
		y *= fInvertLength;
		z *= fInvertLength;
	}
	else{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
}
//------------------------------------------------------------------------------------------------
Vector3D Vector3D::GetNormalize () const
{
	Vector3D kV(*this);
	kV.Normalize();
	return kV;
}
