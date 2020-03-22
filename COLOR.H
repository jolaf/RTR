#ifndef __COLOR_H__
#define __COLOR_H__

#include <iostream.h>

class Color
 {
  public:
  int r, g, b ;
  Color( int _r = 0, int _g = 0, int _b = 0 ) ;
  Color( double _r, double _g, double _b ) ;
  int bright( void ) ;
  Color&  norm( void ) ;
  Color& trunc( void ) ;
    
  Color& operator  = ( Color const& a ) ;
  Color& operator += ( Color const& a ) ;
  Color& operator -= ( Color const& a ) ;
  Color& operator *= ( Color const& a ) ;
  Color& operator *= ( double c ) ;
  Color& operator /= ( double c ) ;
  operator int( void ) ;
  
  friend istream& operator >> ( istream& stream, Color& a ) ;
  friend ostream& operator << ( ostream& stream, Color const& a ) ;
 } ;

Color operator + ( Color const& a, Color const& b ) ;
Color operator - ( Color const& a, Color const& b ) ;
Color operator * ( Color const& a, Color const& b ) ;
Color operator * ( Color const& a, double c ) ;
Color operator * ( double c, Color const& a ) ;
Color operator / ( Color const& a, double c ) ;
int operator <  ( Color const& a, Color const& b ) ;
int operator >  ( Color const& a, Color const& b ) ;
int operator <= ( Color const& a, Color const& b ) ;
int operator >= ( Color const& a, Color const& b ) ;
int operator == ( Color const& a, Color const& b ) ;
int operator != ( Color const& a, Color const& b ) ;
int operator == ( Color const& a, double c ) ;
int operator == ( double c, Color const& a ) ;
int operator != ( Color const& a, double c ) ;
int operator != ( double c, Color const& a ) ;
int   bright( Color const& a ) ;
Color&  norm( Color const& a ) ;
Color& trunc( Color const& a ) ;

#endif
