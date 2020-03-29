#include <stdlib.h>
#include <math.h>
#include <iostream.h>
#include "vector3d.h"
#include "color.h"
#include "mglib.h"
#include "facets.h"
#include "video.h"

#define MAX_DOUBLE 1.7e308		// Very big number
#define MIN_DOUBLE ( - 1.7e308 )	// Very small number
#define _1_S3  0.5773502692		// 1 / sqrt( 3 )
#define AC_1_3 1.2309594170		// acos( 1 / 3 )
#define pi     3.1415926535
#define DANG   0.0250269186		// Diff. betw. tetr. and icos. double angle

extern int MAXX ;
extern int MAXY ;
extern double TETC ;
extern int MAX_WALL ;
extern double ASPECT ;
extern Wall walls[] ;
extern Tetraeder icos[] ;
extern Vector light ;
extern Color lightcolor ;
extern Color backcolor ;
extern long& count ;
extern rccoord text_pos ;

extern void setpixel( int x, int y, Color const& c ) ;
extern void InitShot( void ) ;
extern void WriteShot( void ) ;

void InitWalls( void )
 {
  Color front = Color( 45, 45, 45 ) ;
  Color wall  = Color( 20, 40, 10 ) ;
  Color floor = Color( 30, 30,  0 ) ;
  Color slash = Color( 37, 30, 10 ) ;
  walls[ 0 ] = Wall(  0,  0, -1, MAXX * 1.5, front ) ;
  walls[ 1 ] = Wall(  1,  0,  0, MAXX      , wall ) ;
  walls[ 2 ] = Wall( -1,  0,  0, MAXX      , wall ) ;
  walls[ 3 ] = Wall(  0, -1,  0, MAXX      , floor ) ;
  walls[ 4 ] = Wall(  0,  1,  0, MAXX      , floor ) ;
  walls[ 5 ] = Wall(  _1_S3, -_1_S3, -_1_S3, MAXX * 1.8, slash ) ;
  walls[ 6 ] = Wall( -_1_S3, -_1_S3, -_1_S3, MAXX * 1.8, slash ) ;
  walls[ 7 ] = Wall( -_1_S3,  _1_S3, -_1_S3, MAXX * 1.8, slash ) ;
  walls[ 8 ] = Wall(  _1_S3,  _1_S3, -_1_S3, MAXX * 1.8, slash ) ;
 }

void InitIcos( double mv )
 { 
  Color cube = Color( 40,  0, 40 ) ;
  Color slash = Color( 45, 45, 0 ) ;
  icos[  0 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[  1 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[  2 ] = Tetraeder( mv, 0, 0, 0, 0, 0, slash ) ;
  icos[  3 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[  4 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[  5 ] = Tetraeder( mv, 0, 0, 0, 0, 0, slash ) ;
  icos[  6 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[  7 ] = Tetraeder( mv, 0, 0, 0, 0, 0, slash ) ;
  icos[  8 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[  9 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[ 10 ] = Tetraeder( mv, 0, 0, 0, 0, 0, slash ) ;
  icos[ 11 ] = Tetraeder( mv, 0, 0, 0, 0, 0, slash ) ;
  icos[ 12 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[ 13 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[ 14 ] = Tetraeder( mv, 0, 0, 0, 0, 0, slash ) ;
  icos[ 15 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[ 16 ] = Tetraeder( mv, 0, 0, 0, 0, 0, slash ) ;
  icos[ 17 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[ 18 ] = Tetraeder( mv, 0, 0, 0, 0, 0, cube ) ;
  icos[ 19 ] = Tetraeder( mv, 0, 0, 0, 0, 0, slash ) ;
 }

void TurnIcos( double fi )
 {
  int i ;
  Vector move = Vector( 0, 0, TETC ) ;
  icos[  0 ].TurnZ( - DANG ) ;
  icos[  1 ].TurnZ( pi - DANG ) ;
  icos[  2 ].TurnY( pi / 6 ) ; icos[  2 ].TurnX( AC_1_3 ) ; icos[  2 ].TurnY( - pi / 6 ) ;
  icos[  3 ].TurnX( DANG - pi / 2 ) ; icos[  3 ].TurnZ( pi / 2 ) ;
  icos[  4 ].TurnX( - DANG - pi / 2 ) ; icos[  4 ].TurnZ( - pi / 2 ) ;
  icos[  5 ].TurnY( 5 * pi / 6 ) ; icos[  5 ].TurnX( AC_1_3 ) ; icos[  5 ].TurnY( pi / 6 ) ;
  icos[  6 ].TurnX( pi / 2 ) ; icos[  6 ].TurnY( - DANG + pi / 2 ) ;
  icos[  7 ].TurnX( pi ) ; icos[  7 ].TurnY( 5 * pi / 6 ) ; icos[  7 ].TurnX( - AC_1_3 ) ; icos[  7 ].TurnY( pi / 6 ) ;
  icos[  8 ].TurnX( pi ) ; icos[  8 ].TurnZ( DANG ) ;
  icos[  9 ].TurnX( - pi / 2 ) ; icos[  9 ].TurnY( DANG + pi / 2 ) ;
  icos[ 10 ].TurnX( pi ) ; icos[ 10 ].TurnY( pi / 6 ) ; icos[ 10 ].TurnX( - AC_1_3 ) ; icos[ 10 ].TurnY( - pi / 6 ) ;
  icos[ 11 ].TurnY( 7 * pi / 6 ) ; icos[ 11 ].TurnX( - AC_1_3 ) ; icos[ 11 ].TurnY( - pi / 6 ) ;
  icos[ 12 ].TurnX( - pi / 2 ) ; icos[ 12 ].TurnY( DANG - pi / 2 ) ;
  icos[ 13 ].TurnY( pi ) ; icos[ 13 ].TurnZ( DANG ) ;
  icos[ 14 ].TurnY( - pi / 6 ) ; icos[ 14 ].TurnX( - AC_1_3 ) ; icos[ 14 ].TurnY( pi / 6 ) ;
  icos[ 15 ].TurnX( pi / 2 ) ; icos[ 15 ].TurnY( - DANG - pi / 2 ) ;
  icos[ 16 ].TurnX( pi ) ; icos[ 16 ].TurnY( - pi / 6 ) ; icos[ 16 ].TurnX( AC_1_3 ) ; icos[ 16 ].TurnY( pi / 6 ) ;
  icos[ 17 ].TurnX( - DANG + pi / 2 ) ; icos[ 17 ].TurnZ( pi / 2 ) ;
  icos[ 18 ].TurnX( DANG + pi / 2 ) ; icos[ 18 ].TurnZ( - pi / 2 ) ;
  icos[ 19 ].TurnX( pi ) ; icos[ 19 ].TurnY( 7 * pi / 6 ) ; icos[ 19 ].TurnX( AC_1_3 ) ; icos[ 19 ].TurnY( - pi / 6 ) ;
  for( i = 0 ; i < 20 ; i ++ )
   {
    icos[ i ].TurnY( fi ) ;
    icos[ i ].Move( move ) ;
   }
 }

inline Vector RayBase( int x, int y )
 {
  return Vector( x, y * ASPECT, 0 ) ;
 }
 
inline Vector RayDir( int x, int y )
 {
  return Vector( x, y * ASPECT - 1.0e-6, MAXX ) ;
 }
 
inline void Bright( int x, int y, Color const& c, Vector const& v, Vector const& n )
 {
  setpixel( x, y, c + fabs( cos( v - light, n ) ) * lightcolor ) ;
 }
 
inline void Dim( int x, int y, Color const& c, Vector const& v, Vector const& n )
 {
  if( v.x == n.x ) ;
  setpixel( x, y, c ) ;
 }
 
inline void ShadowCast( int x, int y, Vector const& v, Tetraeder& t, Facet const& f )
 {
  Vector r = light ;
  Vector d = v - light ;
  
  if( ( d * f.n ) <= 0 )
   {
    Dim( x, y, t.color, v, f.n ) ;
    return ;
   }
  double owndist = t.Cross( r, d ) ;
  double dist ;
  for( int i = 0 ; i < 20 ; i ++ )
   if( &icos[ i ] != &t )
    {
     dist = icos[ i ].Cross( r, d ) ;
     if( dist < owndist && dist >= 0 )
      {
       Dim( x, y, t.color, v, f.n ) ;
       return ;
      }
    }
  Bright( x, y, t.color, v, f.n ) ;
 }

inline void ShadowCast( int x, int y, Vector const& v, Wall const& w )
 {
  Vector r = light ;
  Vector d = v - light ;
  for( int i = 0 ; i < 20 ; i ++ )
   if( icos[ i ].Cross( r, d ) > 0 )
    {
     Dim( x, y, w.color, v, w.n ) ;
     return ;
    }
  Bright( x, y, w.color, v, w.n ) ;
 }

inline void Trace( int x, int y )
 {
  Vector r = RayBase( x, y ) ;
  Vector d = RayDir( x, y ) ;
  short i, fn ;
  char mf ;
  double dist ;
  double mind = MAX_DOUBLE ;
  short min = -1 ;
  
  for( i = 0 ; i < 20 ; i ++ )
   {
    dist = icos[ i ].Cross( r, d, &mf ) ;
    if( dist < mind && dist >= 0 )
     {
      mind = dist ;
      min = i ;
      fn = mf ;
     }
   }
  if( min != -1 )
   {
    ShadowCast( x, y, r + d * mind, icos[ min ], icos[ min ].facets[ fn ] ) ;
    return ;
   }
  for( i = 0 ; i < MAX_WALL ; i ++ )
   {
    dist = walls[ i ].Cross( r, d ) ;
    if( dist < mind && dist >= 0 )
     {
      mind = dist ;
      min = i ;
     }
   }
  if( min == -1 ) setpixel( x, y, backcolor ) ;
   else ShadowCast( x, y, r + d * mind, walls[ min ] ) ;
 }

void Shot( void )
 {
  int x, y ;
  for( x = 0 ; x < MAX_WALL ; x ++ )
   {
    walls[ x ].flag = 1 ;
    walls[ x ].exist = 0 ;
   }
  for( x = 0 ; x < 20 ; x ++ )
   for( y = 0 ; y < 4 ; y ++ ) 
    {
     icos[ x ].facets[ y ].flag = 1 ;
     icos[ x ].facets[ y ].exist = 0 ;
    }
  InitShot() ;
  Number() ;
  for( y = MAXY ; y > -MAXY ; y -- )
   {
    CheckKbd() ;
    for( x = -MAXX ; x < MAXX ; x ++ )
     Trace( x, y ) ;
    text_pos = GetTextPosition() ;
    text_pos.col = 20 ;
    SetTextPosition( text_pos ) ;
    cout << ( MAXY - y + 1 ) ;
   }
  WriteShot() ;
 }
