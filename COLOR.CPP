#include <math.h>
#include "color.h"

int MAX_PAL = 256 ;

Color::Color( int _r, int _g, int _b )
 {
  r = _r ; g = _g ; b = _b ;
 }

Color::Color( double _r, double _g, double _b )
 {
  r = _r * MAX_PAL ; g = _g * MAX_PAL ; b = _b * MAX_PAL ;
 }

int Color::bright( void )
 {
  return ( int ) ( ( r > g ) ? ( ( r > b ) ? r : b ) : ( ( g > b ) ? g : b ) ) ;
 }

Color& Color::norm( void )
 {
  int max = MAX_PAL ;
  double d ;
  if( r > max ) max = r ;
  if( g > max ) max = g ;
  if( b > max ) max = b ;
  if( r < 0 ) r = 0 ;
  if( g < 0 ) g = 0 ;
  if( b < 0 ) b = 0 ;
  if( max != MAX_PAL )
   {
    d = ( double ) MAX_PAL / max ;
    r = r * d ; g = g * d ; b = b * d ;
   }
  return *this ;
 }

Color& Color::trunc( void )
 {
  if( r > MAX_PAL ) r = MAX_PAL ;
  if( g > MAX_PAL ) g = MAX_PAL ;
  if( b > MAX_PAL ) b = MAX_PAL ;
  if( r < 0 ) r = 0 ;
  if( g < 0 ) g = 0 ;
  if( b < 0 ) b = 0 ;
  return *this ;
 }
  
Color& Color::operator = ( Color const& a )
 {
  r = a.r ;
  g = a.g ;
  b = a.b ;
  return *this ;
 }

Color& Color::operator += ( Color const& a )
 {
  r += a.r ;
  g += a.g ;
  b += a.b ;
  return *this ;
 }

Color& Color::operator -= ( Color const& a )
 {
  r -= a.r ;
  g -= a.g ;
  b -= a.b ;
  return *this ;
 }

Color& Color::operator *= ( Color const& a )
 {
  r = r * a.r / MAX_PAL ;
  g = g * a.g / MAX_PAL ;
  b = b * a.b / MAX_PAL ;
  return *this ;
 }

Color& Color::operator *= ( double c )
 {
  r *= c ;
  g *= c ;
  b *= c ;
  return *this ;
 }

Color& Color::operator /= ( double c )
 {
  c = 1 / c ;
  r *= c ;
  g *= c ;
  b *= c ;
  return *this ;
 }

Color::operator int( void )
 {
  return ( ( int ) sqrt( r * r + g * g + b * b ) ) ;
 }

Color operator + ( Color const& a, Color const& b ) 
 {
  return Color( a.r + b.r, a.g + b.g, a.b + b.b ) ;
 }

Color operator - ( Color const& a, Color const& b ) 
 {
  return Color( a.r - b.r, a.g - b.g, a.b - b.b ) ;
 }

Color operator * ( Color const& a, Color const& b )
 {
  return Color( a.r * b.r / MAX_PAL, a.g * b.g / MAX_PAL, a.b * b.b / MAX_PAL ) ;
 }

Color operator * ( Color const& a, double c )
 {
  return Color( ( int ) ( a.r * c ), ( int ) ( a.g * c ), ( int ) ( a.b * c ) ) ;
 }

Color operator * ( double c, Color const& a )
 {
  return Color( ( int ) ( a.r * c ), ( int ) ( a.g * c ), ( int ) ( a.b * c ) ) ;
 }

Color operator / ( Color const& a, double c )
 {
  c = 1 / c ;
  return Color( ( int ) ( a.r * c ), ( int ) ( a.g * c ), ( int ) ( a.b * c ) ) ;
 }

int operator <  ( Color const& a, Color const& b )
 {
  return ( bright( a ) < bright( b ) ) ;
 }

int operator >  ( Color const& a, Color const& b )
 {
  return ( bright( a ) > bright( b ) ) ;
 }

int operator <= ( Color const& a, Color const& b )
 {
  return ( bright( a ) <= bright( b ) ) ;
 }

int operator >= ( Color const& a, Color const& b )
 {
  return ( bright( a ) >= bright( b ) ) ;
 }

int operator == ( Color const& a, Color const& b )
 {
  return ( a.r == b.r && a.g == b.g && a.b == b.b ) ;
 }

int operator != ( Color const& a, Color const& b )
 {
  return ( a.r != b.r || a.g != b.g || a.b != b.b ) ;
 }

int operator == ( Color const& a, double c )
 {
  return ( bright( a ) == c ) ;
 }

int operator == ( double c, Color const& a )
 {
  return ( bright( a ) == c ) ;
 }

int operator != ( Color const& a, double c )
 {
  return ( bright( a ) != c ) ;
 }

int operator != ( double c, Color const& a )
 {
  return ( bright( a ) != c ) ;
 }

istream& operator >> ( istream& stream, Color& a )
 {
  char c ;
  stream >> c ;
  if( c != '{' )
   {
    stream.clear( stream.rdstate() | ios::failbit) ;
    return stream ;
   }
  stream >> a.r ;
  stream >> c ;
  if( c != ',' )
   {
    stream.clear( stream.rdstate() | ios::failbit) ;
    return stream ;
   }
  stream >> a.g ;
  stream >> c ;
  if( c != ',' )
   {
    stream.clear( stream.rdstate() | ios::failbit) ;
    return stream ;
   }
  stream >> a.b ;
  stream >> c ;
  if( c != '}' ) stream.clear( stream.rdstate() | ios::failbit) ;
  return stream ;
 }

ostream& operator << ( ostream& stream, Color const& a )
 {
  stream << "{ " << a.r << ", " << a.g << ", " << a.b << " }" ;
  return stream ;
 }

int bright( Color const& a )
 {
  return ( int ) ( ( a.r > a.g ) ? ( ( a.r > a.b ) ? a.r : a.b ) : ( ( a.g > a.b ) ? a.g : a.b ) ) ;
 }

Color& norm( Color& a )
 {
  int max = MAX_PAL ;
  double d ;
  if( a.r > max ) max = a.r ;
  if( a.g > max ) max = a.g ;
  if( a.b > max ) max = a.b ;
  if( a.r < 0 ) a.r = 0 ;
  if( a.g < 0 ) a.g = 0 ;
  if( a.b < 0 ) a.b = 0 ;
  if( max != MAX_PAL )
   {
    d = ( double ) MAX_PAL / max ;
    a.r = a.r * d ; a.g = a.g * d ; a.b = a.b * d ;
   }
  return a ;
 }

Color& trunc( Color& a )
 {
  if( a.r > MAX_PAL ) a.r = MAX_PAL ;
  if( a.g > MAX_PAL ) a.g = MAX_PAL ;
  if( a.b > MAX_PAL ) a.b = MAX_PAL ;
  if( a.r < 0 ) a.r = 0 ;
  if( a.g < 0 ) a.g = 0 ;
  if( a.b < 0 ) a.b = 0 ;
  return a ;
 }
