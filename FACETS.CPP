#include <math.h>
#include "facets.h"

#define S2_3   0.4714045208		// sqrt( 2 ) / 3
#define S2_S3  0.8164965809		// sqrt( 2 / 3 )
#define _1_3   0.3333333333		// 1 / 3
#define MAX_DOUBLE 1.7e308		// Very big number
#define MIN_DOUBLE ( - 1.7e308 )	// Very small number

Facet::Facet( double _x, double _y, double _z, double _k, char _flag )
 {
  k = _k ; flag = _flag ; exist = 0 ;
  n = Vector( _x, _y, _z ) ;
 }
 
double Facet::Cross( Vector const& r, Vector const& d )
 {
  if( flag == 0 ) return MIN_DOUBLE ;
  return ( ( - k - n * r ) / ( n * d ) ) ;
 }

Wall::Wall( void )
 {
  k = 0 ; flag = 1 ; 
  n = Vector( 0, 0, 0 ) ; 
  color = Color( 100, 100, 100 ) ;
 }

Wall::Wall( double _x, double _y, double _z, double _k, Color& _c, char _flag )
 {
  k = _k ; flag = _flag ;
  n = Vector( _x, _y, _z ) ;
  color = _c ;
 }

Tetraeder::Tetraeder( void )
 {
  l = 250.0 ; flag = 1 ; color = Color( 100, 100, 100 ) ;
  facets[ 0 ] = Facet(      0,      1,      0,         0, 1 ) ;
  facets[ 1 ] = Facet(   S2_3,  -_1_3, -S2_S3,         0, 1 ) ;
  facets[ 2 ] = Facet(   S2_3,  -_1_3,  S2_S3,         0, 1 ) ;
  facets[ 3 ] = Facet( -S2_S3,  -_1_3,      0, l * S2_S3, 1 ) ;
  a = Vector( 0, 0, 0 ) ;
  ax = ay = az = 0 ;
 }

Tetraeder::Tetraeder( double _x, double _y, double _z,
                      double _ax, double _ay, double _az,
                      Color const& _c, char _flag, double _l )
 {
  l = _l ; flag =_flag ; color = _c ;
  facets[ 0 ] = Facet(      0,      1,      0,         0, 1 ) ;
  facets[ 1 ] = Facet(   S2_3,  -_1_3, -S2_S3,         0, 1 ) ;
  facets[ 2 ] = Facet(   S2_3,  -_1_3,  S2_S3,         0, 1 ) ;
  facets[ 3 ] = Facet( -S2_S3,  -_1_3,      0, l * S2_S3, 1 ) ;
  a = Vector( 0, 0, 0 ) ;
  ax = ay = az = 0 ;
  TurnX( _ax, 0, 0 ) ;
  TurnY( _ay, 0, 0 ) ;
  TurnZ( _az, 0, 0 ) ;
  Move( Vector( _x, _y, _z ) ) ;
 }

void Tetraeder::TurnX( double fi, double y, double z )
 {
  Vector v ;
  ax += fi ;
  for( int i = 0 ; i < 4 ; i ++ )
   {
    v = Vector( facets[ i ].n.x,
                facets[ i ].n.y * cos( fi ) + facets[ i ].n.z * sin( fi ),
                facets[ i ].n.z * cos( fi ) - facets[ i ].n.y * sin( fi ) ) ;
    facets[ i ].k = facets[ i ].k + ( facets[ i ].n - v ) * Vector( 0, y, z ) ;
    facets[ i ].n = v ;
   }
 }

void Tetraeder::TurnY( double fi, double x, double z )
 {
  Vector v ;
  ay += fi ;
  for( int i = 0 ; i < 4 ; i ++ )
   {
    v = Vector( facets[ i ].n.x * cos( fi ) - facets[ i ].n.z * sin( fi ),
                facets[ i ].n.y,
                facets[ i ].n.z * cos( fi ) + facets[ i ].n.x * sin( fi ) ) ;
    facets[ i ].k = facets[ i ].k + ( facets[ i ].n - v ) * Vector( x, 0, z ) ;
    facets[ i ].n = v ;
   }
 }

void Tetraeder::TurnZ( double fi, double x, double y )
 {
  Vector v ;
  ay += fi ;
  for( int i = 0 ; i < 4 ; i ++ )
   {
    v = Vector( facets[ i ].n.x * cos( fi ) + facets[ i ].n.y * sin( fi ),
                facets[ i ].n.y * cos( fi ) - facets[ i ].n.x * sin( fi ),
                facets[ i ].n.z ) ;
    facets[ i ].k = facets[ i ].k + ( facets[ i ].n - v ) * Vector( x, y, 0 ) ;
    facets[ i ].n = v ;
   }
 }

void Tetraeder::Move ( Vector b )
 {
  a += b ;
  for( int i = 0 ; i < 4 ; i ++ )
   facets[ i ].k -= facets[ i ].n * b ;
 }

double Tetraeder::Cross( Vector const& r, Vector const& d, char* fn )
 {
  double t ;
  double tn = MIN_DOUBLE ;
  double tf = MAX_DOUBLE ;
  char min = 255 ;
  if( flag == 0 ) return MIN_DOUBLE ;
  for( char i = 0 ; i < 4 ; i ++ )
   {
    t = facets[ i ].Cross( r, d ) ;
    if( facets[ i ].n * d > 0 )
     { 
      if( t > tn )
       {
        tn = t ;
        min = i ;
       }
     }
     else 
      if( t < tf ) tf = t ;
    if( tn > tf ) return MIN_DOUBLE ;
   }
  if( min == 255 ) return MIN_DOUBLE ;
  facets[ min ].exist = 1 ;
  if( fn != NULL ) *fn = min ;
  return tn ;
 }
