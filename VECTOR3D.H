#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include <iostream.h>

class Vector
 {
  public:
  double x, y, z ;
  Vector( double _x = 0.0, double _y = 0.0, double _z = 0.0 ) ;
  Vector norm( double c = 1 ) ;
  double abs( void ) ;
    
  Vector& operator  = ( Vector const& a ) ;
  Vector& operator += ( Vector const& a ) ;
  Vector& operator -= ( Vector const& a ) ;
  Vector& operator %= ( Vector const& a ) ;
  Vector& operator *= ( double c ) ;
  Vector& operator /= ( double c ) ;
  Vector& operator |= ( double c ) ;
  Vector  operator  + ( void ) ;
  Vector  operator  - ( void ) ;
  Vector  operator  ! ( void ) ;
  Vector  operator  ~ ( void ) ;
  operator double( void ) ;
  
  friend istream& operator >> ( istream& stream, Vector& a ) ;
  friend ostream& operator << ( ostream& stream, Vector const& a ) ;
 } ;

Vector operator + ( Vector const& a, Vector const& b ) ;
Vector operator - ( Vector const& a, Vector const& b ) ;
Vector operator % ( Vector const& a, Vector const& b ) ;// Vector multiplication
Vector operator * ( Vector const& a, double c ) ;	// Vector mul number
Vector operator * ( double c, Vector const& a ) ;	// Vector mul number
Vector operator / ( Vector const& a, double c ) ;	// Vector div number
Vector operator | ( Vector const& a, double c ) ;	// Norming
double operator * ( Vector const& a, Vector const& b ) ;// Scalar multiplication
double operator ^ ( Vector const& a, Vector const& b ) ;// Angle between vectors
int operator <  ( Vector const& a, Vector const& b ) ;
int operator >  ( Vector const& a, Vector const& b ) ;
int operator <= ( Vector const& a, Vector const& b ) ;
int operator >= ( Vector const& a, Vector const& b ) ;
int operator == ( Vector const& a, Vector const& b ) ;
int operator != ( Vector const& a, Vector const& b ) ;
int operator || ( Vector const& a, Vector const& b ) ;	// Collinearity
int operator == ( Vector const& a, double c ) ;
int operator == ( double c, Vector const& a ) ;
int operator != ( Vector const& a, double c ) ;
int operator != ( double c, Vector const& a ) ;
Vector norm( Vector const& a, double c = 1 ) ;
Vector min( Vector const& a, Vector const& b ) ;
Vector max( Vector const& a, Vector const& b ) ;
Vector mix( Vector const& a, Vector const& b, Vector const& c ) ; // Mixed mult.
double vabs( Vector const& a ) ;
double abs( Vector const& a ) ;
double cos( Vector const& a, Vector const& b ) ;
double sin( Vector const& a, Vector const& b ) ;
double tan( Vector const& a, Vector const& b ) ;
double ctg( Vector const& a, Vector const& b ) ;
 
#endif
