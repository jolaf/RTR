#include <math.h>
#include "vector3d.h"

Vector::Vector( double _x, double _y, double _z )
 {
  x = _x ; y = _y ; z = _z ;
 }
  
Vector Vector::norm( double c )
 {
  double d = sqrt( c / abs() ) ;
  return Vector( x * d, y * d, z * d ) ;
 }
  
double Vector::abs( void )
 {
  return sqrt( x * x + y * y + z * z ) ;
 }

Vector& Vector::operator = ( Vector const& a )
 {
  x = a.x ;
  y = a.y ;
  z = a.z ;
  return *this ;
 }

Vector& Vector::operator += ( Vector const& a )
 {
  x += a.x ;
  y += a.y ;
  z += a.z ;
  return *this ;
 }

Vector& Vector::operator -= ( Vector const& a )
 {
  x -= a.x ;
  y -= a.y ;
  z -= a.z ;
  return *this ;
 }

Vector& Vector::operator %= ( Vector const& a )
 {
  x = y * a.z - z * a.y ;
  y = z * a.x - x * a.z ;
  z = x * a.y - y * a.x ;
  return *this ;
 }

Vector& Vector::operator *= ( double c )
 {
  x *= c ;
  y *= c ;
  z *= c ;
  return *this ;
 }

Vector& Vector::operator /= ( double c )
 {
  c = 1 / c ;
  x *= c ;
  y *= c ;
  z *= c ;
  return *this ;
 }

Vector& Vector::operator |= ( double c )
 {
  c = sqrt( c / abs() ) ;
  x *= c ;
  y *= c ;
  z *= c ;
  return *this ;
 }

Vector Vector::operator + ( void )
 {
  return *this ;
 }

Vector Vector::operator - ( void )
 {
  return Vector( -x, -y, -z ) ;
 }
 
Vector Vector::operator ! ( void )
 {
  return Vector( -x, -y, -z ) ;
 }

Vector Vector::operator ~ ( void )
 {
  return Vector( -x, -y, -z ) ;
 }

Vector::operator double( void )
 {
  return sqrt( x * x + y * y + z * z ) ;
 }

Vector operator + ( Vector const& a, Vector const& b ) 
 {
  return Vector( a.x + b.x, a.y + b.y, a.z + b.z ) ;
 }

Vector operator - ( Vector const& a, Vector const& b ) 
 {
  return Vector( a.x - b.x, a.y - b.y, a.z - b.z ) ;
 }

Vector operator % ( Vector const& a, Vector const& b ) 
 {
  return Vector( a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x ) ;
 }

Vector operator * ( Vector const& a, double c )
 {
  return Vector( a.x * c, a.y * c ,a.z * c ) ;
 }
 
Vector operator * ( double c, Vector const& a )
 {
  return Vector( a.x * c, a.y * c ,a.z * c ) ;
 }

Vector operator / ( Vector const& a, double c )
 {
  c = 1 / c ;
  return Vector( a.x * c, a.y * c ,a.z * c ) ;
 }

Vector operator | ( Vector const& a, double c )
 {
  c = sqrt( c / vabs( a ) ) ;
  return Vector( a.x * c, a.y * c, a.z * c ) ;
 }
 
double operator * ( Vector const& a, Vector const& b )
 {
  return ( a.x * b.x + a.y * b.y + a.z * b.z ) ;
 }

double operator ^ ( Vector const& a, Vector const& b )
 {
  return acos( ( a * b ) / ( vabs( a ) * vabs( b ) ) ) ;
 }

int operator <  ( Vector const& a, Vector const& b )
 {
  return ( vabs( a ) < vabs( b ) ) ;
 }

int operator >  ( Vector const& a, Vector const& b )
 {
  return ( vabs( a ) > vabs( b ) ) ;
 }

int operator <= ( Vector const& a, Vector const& b )
 {
  return ( vabs( a ) <= vabs( b ) ) ;
 }

int operator >= ( Vector const& a, Vector const& b )
 {
  return ( vabs( a ) >= vabs( b ) ) ;
 }

int operator == ( Vector const& a, Vector const& b )
 {
  return ( a.x == b.x && a.y == b.y && a.z == b.z ) ;
 }

int operator != ( Vector const& a, Vector const& b )
 {
  return ( a.x != b.x || a.y != b.y || a.z != b.z ) ;
 }

int operator || ( Vector const& a, Vector const& b )
 {
  return ( a * b == vabs( a ) * vabs( b ) ) ;
 }

int operator == ( Vector const& a, double c )
 {
  return ( vabs( a ) == c ) ;
 }

int operator == ( double c, Vector const& a )
 {
  return ( vabs( a ) == c ) ;
 }

int operator != ( Vector const& a, double c )
 {
  return ( vabs( a ) != c ) ;
 }

int operator != ( double c, Vector const& a )
 {
  return ( vabs( a ) != c ) ;
 }

istream& operator >> ( istream& stream, Vector& a )
 {
  char c ;
  stream >> c ;
  if( c != '{' )
   {
    stream.clear( stream.rdstate() | ios::failbit) ;
    return stream ;
   }
  stream >> a.x ;
  stream >> c ;
  if( c != ',' )
   {
    stream.clear( stream.rdstate() | ios::failbit) ;
    return stream ;
   }
  stream >> a.y ;
  stream >> c ;
  if( c != ',' )
   {
    stream.clear( stream.rdstate() | ios::failbit) ;
    return stream ;
   }
  stream >> a.z ;
  stream >> c ;
  if( c != '}' ) stream.clear( stream.rdstate() | ios::failbit) ;
  return stream ;
 }

ostream& operator << ( ostream& stream, Vector const& a )
 {
  stream << "{ " << a.x << ", " << a.y << ", " << a.z << " }" ;
  return stream ;
 }

Vector norm( Vector const& a, double c )
 {
  c = sqrt( c / vabs( a ) ) ;
  return Vector( a.x * c, a.y * c, a.z * c ) ;
 }

Vector min( Vector const& a, Vector const& b )
 {
  if( vabs( a ) < vabs( b ) ) return a ;
   else return b ;
 }

Vector max( Vector const& a, Vector const& b )
 {
  if( vabs( a ) > vabs( b ) ) return a ;
   else return b ;
 }

Vector mix( Vector const& a, Vector const& b, Vector const& c )
 {
  return ( a * b * c ) ;
 }

double vabs( Vector const& a )
 {
  return sqrt( a.x * a.x + a.y * a.y + a.z * a.z ) ;
 }

double abs( Vector const& a )
 {
  return sqrt( a.x * a.x + a.y * a.y + a.z * a.z ) ;
 }

double cos( Vector const& a, Vector const& b )
 {
  return ( a * b ) / ( vabs( a ) * vabs( b ) ) ;
 }

double sin( Vector const& a, Vector const& b )
 {
  double c = ( a * b ) / ( vabs( a ) * vabs( b ) ) ;
  return sqrt( 1 - c * c ) ;
 }

double tan( Vector const& a, Vector const& b )
 {
  double c = ( a * b ) / ( vabs( a ) * vabs( b ) ) ;
  return ( sqrt( 1 - c * c ) / c ) ;
 }

double ctg( Vector const& a, Vector const& b )
 {
  double c = ( a * b ) / ( vabs( a ) * vabs( b ) ) ;
  return ( c / sqrt( 1 - c * c ) ) ;
 }
