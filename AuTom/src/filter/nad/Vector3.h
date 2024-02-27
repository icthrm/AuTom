/*
	Vector3.h
	Class for 3-value vectors
	Author: Bernard Heymann
	Created: 20000501 	Modified: 20120501
*/


#include <iostream>
#include <cmath>

// #include "math_util.h"

using namespace std;

#ifndef _Vector3_
#define _Vector3_
/************************************************************************
@Object: class Vector3
@Description:
	Vector class for 3-value vectors used in 3D space.
@Features:
	The internal variables are an array of 3 numbers.
*************************************************************************/
template <typename Type>
class Vector3 {
private:
    Type	data[3];
public:
    Vector3() {
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
    }
    Vector3(const Vector3& v) {
        data[0] = v.data[0];
        data[1] = v.data[1];
        data[2] = v.data[2];
    }
    Vector3(const Type x, const Type y, const Type z) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }
    Vector3	operator=(const Type d) {
        data[0] = d;
        data[1] = 0;
        data[2] = 0;
        return *this;
    }
    Vector3	operator=(const Vector3& v) {
        data[0] = v.data[0];
        data[1] = v.data[1];
        data[2] = v.data[2];
        return *this;
    }
    Vector3	operator-() {
        Vector3<Type>	vn(-data[0], -data[1], -data[2]);
        return vn;
    }
    Vector3	operator+=(const double d) {
        for ( int i=0; i<3; i++ ) data[i] += Type(d);
        return *this;
    }
    Vector3	operator+(const double d) {
        Vector3<Type>	vn(Type(data[0]+d), Type(data[1]+d), Type(data[2]+d));
        return vn;
    }
    Vector3	operator+=(const Vector3& v) {
        for ( int i=0; i<3; i++ ) data[i] += v.data[i];
        return *this;
    }
    Vector3	operator+(const Vector3& v) {
        Vector3<Type>	vn(data[0]+v.data[0], data[1]+v.data[1], data[2]+v.data[2]);
        return vn;
    }
    Vector3	operator-=(const double d) {
        for ( int i=0; i<3; i++ ) data[i] -= Type(d);
        return *this;
    }
    Vector3	operator-(const double d) {
        Vector3<Type>	vn(Type(data[0]-d), Type(data[1]-d), Type(data[2]-d));
        return vn;
    }
    Vector3	operator-=(const Vector3& v) {
        for ( int i=0; i<3; i++ ) data[i] -= v.data[i];
        return *this;
    }
    Vector3	operator-(const Vector3& v) {
        Vector3<Type>	vn(data[0]-v.data[0], data[1]-v.data[1], data[2]-v.data[2]);
        return vn;
    }
    Vector3	operator*=(const double d) {
        for ( int i=0; i<3; i++ ) data[i] = Type(data[i] * d);
        return *this;
    }
    Vector3	operator*(const double d) {
        Vector3<Type>	vn(Type(data[0] * d), Type(data[1] * d), Type(data[2] * d));
        return vn;
    }
    Vector3	operator*=(const Vector3& v) {	// Note that the order is important for vectors of different types
        for ( int i=0; i<3; i++ ) data[i] *= v.data[i];
        return *this;
    }
    Vector3	operator*(const Vector3& v) {
        Vector3<Type>	vn(data[0]*v.data[0], data[1]*v.data[1], data[2]*v.data[2]);
        return vn;
    }
    Vector3 operator/=(const double d) {
        if ( fabs(d) < 1e-30 ) return *this;
        double	div = 1/d;
        for ( int i=0; i<3; i++ ) data[i] *= div;
        return *this;
    }
    Vector3	operator/(const double d) {
        Vector3<Type>	vn(Type(data[0] / d), Type(data[1] / d), Type(data[2] / d));
        return vn;
    }
    Vector3	operator/=(const Vector3& v) {
        for ( int i=0; i<3; i++ ) data[i] /= v.data[i];
        return *this;
    }
    Vector3	operator/(const Vector3& v) {
        Vector3<Type>	vn(data[0]/v.data[0], data[1]/v.data[1], data[2]/v.data[2]);
        return vn;
    }
    bool	operator==(const Vector3& v) {
        int		i, e = 0;
        for ( i=0; i<3; i++ ) e += ( data[i] == v.data[i] );
        return ( e == 3 );
    }
    bool	operator==(const double d) {
        int		i, e = 0;
        for ( i=0; i<3; i++ ) e += ( data[i] == d );
        return ( e == 3 );
    }
    bool	operator!=(const Vector3& v) {
        int		i, e = 0;
        for ( i=0; i<3; i++ ) e += ( data[i] == v.data[i] );
        return ( e != 3 );
    }
    bool	operator>(const double d) {
        int		i, e = 0;
        for ( i=0; i<3; i++ ) e += ( data[i] > d );
        return ( e == 3 );
    }
    bool	operator<(const double d) {
        int		i, e = 0;
        for ( i=0; i<3; i++ ) e += ( data[i] < d );
        return ( e == 3 );
    }
    Type&	operator[](int i) {
        if ( i < 0 ) i = 0;
        if ( i > 2 ) i = 2;
        return data[i];
    }
    template <typename T2> operator Vector3<T2>() const {
        return Vector3<T2>((T2)data[0], (T2)data[1], (T2)data[2]);
    }
    Type	x() {
        return data[0];
    }
    Type	y() {
        return data[1];
    }
    Type	z() {
        return data[2];
    }
    void	x(const double d) {
        data[0] = (Type)d;
    }
    void	y(const double d) {
        data[1] = (Type)d;
    }
    void	z(const double d) {
        data[2] = (Type)d;
    }
    Vector3	min(const double d) {
        Vector3		vn(*this);
        for ( int i=0; i<3; i++ ) if ( vn.data[i] > d ) vn.data[i] = (Type)d;
        return vn;
    }
    Vector3	max(const double d) {
        Vector3		vn(*this);
        for ( int i=0; i<3; i++ ) if ( vn.data[i] < d ) vn.data[i] = (Type)d;
        return vn;
    }
    Vector3	min(const Vector3& v) {
        Vector3		vn(*this);
        for ( int i=0; i<3; i++ ) if ( vn.data[i] > v.data[i] ) vn.data[i] = v.data[i];
        return vn;
    }
    Vector3	max(const Vector3& v) {
        Vector3		vn(*this);
        for ( int i=0; i<3; i++ ) if ( vn.data[i] < v.data[i] ) vn.data[i] = v.data[i];
        return vn;
    }
    Vector3	abs() {
        Vector3		vn(*this);
        for ( int i=0; i<3; i++ ) if ( data[i] < 0 ) vn.data[i] = -data[i];
        return vn;
    }
    Vector3	floor(int places) {
        Vector3		vn;
        for ( int i=0; i<3; i++ ) vn.data[i] = bfloor(data[i], places);
        return vn;
    }
    Vector3	round(int places) {
        Vector3		vn;
        for ( int i=0; i<3; i++ ) vn.data[i] = bround(data[i], places);
        return vn;
    }
    Vector3	remainder(int divisor) {
        Vector3		vn;
        for ( int i=0; i<3; i++ ) vn.data[i] = fmod(data[i], divisor);
        return vn;
    }
    double	length2() {
        return (double)data[0]*data[0] + (double)data[1]*data[1] + (double)data[2]*data[2];
    }
    double	length() {
        return sqrt(length2());
    }
    double	distance(const Vector3& v) {
        return (*this - v).length();
    }
    double	distance2(const Vector3& v) {
        return (*this - v).length2();
    }
    Vector3	square_root() {
        Vector3		v;
        for ( int i=0; i<3; i++ ) v.data[i] = sqrt(data[i]);
        return v;
    }
    double	sum() {
        return (double)data[0] + (double)data[1] + (double)data[2];
    }
    double	volume() {
        return (double)data[0]*data[1]*data[2];
    }
    double	normalize() {
        double	len = length();
        if ( len < 1e-30 ) {
//			len = 1;
            data[2] = 1;
        } else {
            double	div = 1.0/len;
            for ( int i=0; i<3; i++ ) data[i] *= div;
        }
        return len;
    }
    double	scalar(const Vector3& v) {
        return (double)data[0]*v.data[0] + (double)data[1]*v.data[1] + (double)data[2]*v.data[2];
    }
    Vector3	cross(const Vector3& v) {
        Vector3		vn((double)data[1]*v.data[2] - (double)data[2]*v.data[1],
                       (double)data[2]*v.data[0] - (double)data[0]*v.data[2],
                       (double)data[0]*v.data[1] - (double)data[1]*v.data[0]);
        return vn;
    }
    double	angle(Vector3& v) {
        Vector3		v1(*this);
        Vector3		v2(v);
        v1.normalize();
        v2.normalize();
        double		prod = v1.scalar(v2);
        if ( prod > 1 ) prod = 1;
        if ( prod < -1 ) prod = -1;
        return(acos(prod));
    }
    Vector3	normal(Vector3& v1, Vector3& v2) {
        Vector3 	edge1 = *this - v1;
        Vector3 	edge2 = *this - v2;
        Vector3 	normal = edge1.cross(edge2);
        normal.normalize();
        return(normal);
    }
    bool notfinite() {
        int			e = 0;
        for ( int i=0; i<3; i++ ) e += finite(data[i]);
        return ( e != 3 );
    }
//	friend Vector3<float>	operator=(Vector3<double>& vd);
//	friend Vector3<double>	operator=(Vector3<float>& vf);
} ;

template <typename Type>
ostream& operator<<(ostream& output, Vector3<Type> vec) {
    output.setf(ios::fixed, ios::floatfield);
    output.precision(4);
    output << "{" << vec.x() << "," << vec.y() << "," << vec.z() << "}";
    return output;
}

/************************************************************************
@Function: Vector3::operator/
@Description:
	Divides a value by each of the elements of a 3-value vector.
@Algorithm:
	All elements of the vector must be non-zero.
@Arguments:
	double d			value to be divided.
	Vector3<Type> vec	3-value vector.
@Returns:
	Vector3<Type>		new 3-value vector.
**************************************************************************/
template <typename Type>
inline Vector3<Type>	operator/(double d, Vector3<Type> vec)
{
    if ( vec.x() == 0 || vec.y() == 0 || vec.z() == 0 )
        cerr << "Error: Zero elements in vector " << vec << endl;

    return Vector3<Type>(d/vec.x(), d/vec.y(), d/vec.z());
}

template <typename Type>
inline Vector3<Type>	vector3_scalar_range(Vector3<Type> vec, double min, double max)
{
    if ( max < min ) return(vec);

    if ( vec.x() < min ) vec[0] = min;
    if ( vec.x() > max ) vec[0] = max;
    if ( vec.y() < min ) vec[1] = min;
    if ( vec.y() > max ) vec[1] = max;
    if ( vec.z() < min ) vec[2] = min;
    if ( vec.z() > max ) vec[2] = max;

    return(vec);
}

template <typename Type>
inline Vector3<Type>	vector3_set_PBC(Vector3<Type> coord, Vector3<Type>& box)
{
    while ( coord.x() < 0 ) coord[0] += box.x();
    while ( coord.y() < 0 ) coord[1] += box.y();
    while ( coord.z() < 0 ) coord[2] += box.z();
    while ( coord.x() >= box.x() ) coord[0] -= box.x();
    while ( coord.y() >= box.y() ) coord[1] -= box.y();
    while ( coord.z() >= box.z() ) coord[2] -= box.z();

    return(coord);
}

template <typename Type>
inline Vector3<Type>	vector3_difference_PBC(Vector3<Type>& v1, Vector3<Type>& v2, Vector3<Type>& box)
{
    Vector3<Type>	d = v1 - v2;

    if ( box.x() - d.x() < d.x() ) d[0] -= box.x();
    if ( box.x() + d.x() < -d.x() ) d[0] += box.x();
    if ( box.y() - d.y() < d.y() ) d[1] -= box.y();
    if ( box.y() + d.y() < -d.y() ) d[1] += box.y();
    if ( box.z() - d.z() < d.z() ) d[2] -= box.z();
    if ( box.z() + d.z() < -d.z() ) d[2] += box.z();

    return(d);
}


#endif


