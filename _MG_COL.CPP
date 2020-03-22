#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <conio.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>
#include <dos.h>
#include "Vector3d.h"

#define VIDEO_MODE _MRES256COLOR
#define MAX_PAL 6
#define LIGHT 4
#define SHADOW 0
#define MAX_PAL2 36
#define MAX_COLOR 256
#define MAXX 160
#define MAXY 100
#define MINX ( -160 )
#define MINY ( -100 )
#define SCREEN ( ( 0xA000 ) << 4 )
#define EPS ( 1.0e-5 )
#define MAX_DOUBLE ( 1.7e38 )
#define MAX_WALL 9
#define BACK_R 2
#define BACK_G 2
#define BACK_B 4
#define ASPECT ( 1.2 )
#define S2_3   ( 0.4714045208 )
#define S2_S3  ( 0.8164965809 )
#define _1_3   ( 0.3333333333 )
#define _1_S3  ( 0.5773502692 )
#define S3     ( 1.7320508080 )
#define AC_1_3 ( 1.2309594170 )
#define DANG   ( 0.0250269186 )
#define DEFAULT_LEN ( 50.0 )
#define DEFAULT_R 4
#define DEFAULT_G 3
#define DEFAULT_B 2
#define TETC ( MAXX * 0.75 )
#define pi ( 3.1415926535 )

class Facet
 {
  public:
  Vector n ;
  double k ;
  char flag ;
  char exist ;
  
  Facet( void ) { }
  Facet( double x, double y, double z, double _k, char _flag = 1 )
   {
    k = _k ; flag = _flag ; exist = 0 ;
    n = Vector( x, y, z ) ;
   }
  double Cross( Vector const& r, Vector const& d )
   {
    return ( ( - k - n * r ) / ( n * d ) ) ;
   }
 } ;

class Wall : public Facet
 {
  public:
  char r, g, b ;
  
  Wall( void ) { }
  Wall( double x, double y, double z, double _k, char _r, char _g, char _b, char _flag = 1 )
   {
    k = _k ; r = _r ; g = _g ; b = _b ; flag = _flag ;
    n = Vector( x, y, z ) ;
   }
 } ;

class Tetraeder
 {
  public:
  Facet facets[ 4 ] ;
  Vector a ;
  double ax, ay, az ;
  char r, g, b ;
  double l ;
  char flag ;
  
  Tetraeder( char _r = DEFAULT_R, char _g = DEFAULT_G , char _b = DEFAULT_B,
             double x = 0, double y = 0, double z = 0,
             double _ax = 0, double _ay = 0, double _az = 0, char _flag = 1,
             double _l = DEFAULT_LEN ) ;
  void TurnX( double fi, double y = 0, double z = 0 ) ;
  void TurnY( double fi, double x = 0, double z = 0 ) ;
  void TurnZ( double fi, double x = 0, double y = 0 ) ;
  void Move ( Vector b ) ;
  double Cross( Vector const& r, Vector const& d, short* fn = NULL ) ;
 } ;

Tetraeder::Tetraeder( char _r, char _g, char _b, double x, double y, double z,
                      double _ax, double _ay, double _az, char _flag, double _l )
 {
  l = _l ; r = _r ; g = _g ; b = _b ; flag =_flag ;
  facets[ 0 ] = Facet(      0,      1,      0,         0, 1 ) ;
  facets[ 1 ] = Facet(   S2_3,  -_1_3, -S2_S3,         0, 1 ) ;
  facets[ 2 ] = Facet(   S2_3,  -_1_3,  S2_S3,         0, 1 ) ;
  facets[ 3 ] = Facet( -S2_S3,  -_1_3,      0, l * S2_S3, 1 ) ;
  a = Vector( 0, 0, 0 ) ;
  ax = ay = az = 0 ;
  TurnX( _ax, 0, 0 ) ;
  TurnY( _ay, 0, 0 ) ;
  TurnZ( _az, 0, 0 ) ;
  Move( Vector( x, y, z ) ) ;
 }

inline void Tetraeder::TurnX( double fi, double y, double z )
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

inline void Tetraeder::TurnY( double fi, double x, double z )
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

inline void Tetraeder::TurnZ( double fi, double x, double y )
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

inline void Tetraeder::Move ( Vector b )
 {
  a += b ;
  for( int i = 0 ; i < 4 ; i ++ )
   facets[ i ].k -= facets[ i ].n * b ;
 }

inline double Tetraeder::Cross( Vector const& r, Vector const& d, short* fn )
 {
  double t ;
  double tn = - MAX_DOUBLE ;
  double tf = MAX_DOUBLE ;
  short min = -1 ;
  
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
    if( tn > tf ) return ( - MAX_DOUBLE ) ;
   }
  facets[ min ].exist = 1 ;
  if( fn != NULL ) *fn = min ;
  min = -1 ;
  for( i = 0 ; i < 4 ; i ++ )
   if( facets[ i ].exist == 0 )
    if( min != -1 ) return tn ;
     else min = i ;
  facets[ min ].flag = 0 ;
  return tn ;
 }

short bright [ MAX_PAL ] = { 0, 12, 25, 38, 51, 63 } ;
Wall walls[ MAX_WALL ] ;
Tetraeder icos[ 20 ] ;
long pal[ MAX_COLOR ] ;
Vector light ;
int shot_count = 0 ;
int f ;

inline void InitWalls( void )
 {
  walls[ 0 ] = Wall(  0,  0, -1, MAXX * 1.5, 1, 1, 1 ) ;
  walls[ 1 ] = Wall(  1,  0,  0, MAXX      , 0, 0, 1 ) ;
  walls[ 2 ] = Wall( -1,  0,  0, MAXX      , 0, 0, 1 ) ;
  walls[ 3 ] = Wall(  0, -1,  0, MAXX      , 0, 1, 0 ) ;
  walls[ 4 ] = Wall(  0,  1,  0, MAXX      , 0, 1, 0 ) ;
  walls[ 5 ] = Wall(  _1_S3, -_1_S3, -_1_S3, MAXX * 1.8, 0, 1, 1 ) ;
  walls[ 6 ] = Wall( -_1_S3, -_1_S3, -_1_S3, MAXX * 1.8, 0, 1, 1 ) ;
  walls[ 7 ] = Wall( -_1_S3,  _1_S3, -_1_S3, MAXX * 1.8, 0, 1, 1 ) ;
  walls[ 8 ] = Wall(  _1_S3,  _1_S3, -_1_S3, MAXX * 1.8, 0, 1, 1 ) ;
 }

inline void InitIcos1( void )
 { 
  icos[ 0 ]  = Tetraeder( 1, 1, 0, 0, 0, TETC,        0,        0,    - DANG ) ;
  icos[ 1 ]  = Tetraeder( 1, 1, 0, 0, 0, TETC,        0,        0, pi - DANG ) ;
  icos[ 3 ]  = Tetraeder( 1, 1, 0, 0, 0, TETC,   DANG - pi / 2, 0,    pi / 2 ) ;
  icos[ 4 ]  = Tetraeder( 1, 1, 0, 0, 0, TETC, - DANG - pi / 2, 0,  - pi / 2 ) ;
  icos[ 6 ]  = Tetraeder( 1, 1, 0, 0, 0, TETC,   pi / 2, - DANG + pi / 2,  0 ) ;
  icos[ 8 ]  = Tetraeder( 1, 1, 0, 0, 0, TETC,       pi,        0,      DANG ) ;
  icos[ 9 ]  = Tetraeder( 1, 1, 0, 0, 0, TETC, - pi / 2,   DANG + pi / 2,  0 ) ;
  icos[ 12 ] = Tetraeder( 1, 1, 0, 0, 0, TETC, - pi / 2,   DANG - pi / 2,  0 ) ;
  icos[ 13 ] = Tetraeder( 1, 1, 0, 0, 0, TETC,        0,       pi,      DANG ) ;
  icos[ 15 ] = Tetraeder( 1, 1, 0, 0, 0, TETC,   pi / 2, - DANG - pi / 2,  0 ) ;
  icos[ 17 ] = Tetraeder( 1, 1, 0, 0, 0, TETC, - DANG + pi / 2, 0,    pi / 2 ) ;
  icos[ 18 ] = Tetraeder( 1, 1, 0, 0, 0, TETC,   DANG + pi / 2, 0,  - pi / 2 ) ;
 }
 
inline void InitIcos3( void )
 {
  icos[  2 ] = Tetraeder( 0, 1, 1, 0, 0, 0,  0,     pi / 6, 0 ) ;
  icos[  2 ].TurnX(   AC_1_3 ) ;
  icos[  2 ].TurnY( - pi / 6 ) ;
  icos[  2 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[  5 ] = Tetraeder( 0, 1, 1, 0, 0, 0,  0, 5 * pi / 6, 0 ) ;
  icos[  5 ].TurnX(   AC_1_3 ) ;
  icos[  5 ].TurnY(   pi / 6 ) ;
  icos[  5 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[  7 ] = Tetraeder( 0, 1, 1, 0, 0, 0, pi, 5 * pi / 6, 0 ) ;
  icos[  7 ].TurnX( - AC_1_3 ) ;
  icos[  7 ].TurnY(   pi / 6 ) ;
  icos[  7 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 10 ] = Tetraeder( 0, 1, 1, 0, 0, 0, pi,     pi / 6, 0 ) ;
  icos[ 10 ].TurnX( - AC_1_3 ) ;
  icos[ 10 ].TurnY( - pi / 6 ) ;
  icos[ 10 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 11 ] = Tetraeder( 0, 1, 1, 0, 0, 0,  0, 7 * pi / 6, 0 ) ;
  icos[ 11 ].TurnX( - AC_1_3 ) ;
  icos[ 11 ].TurnY( - pi / 6 ) ;
  icos[ 11 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 14 ] = Tetraeder( 0, 1, 1, 0, 0, 0,  0,   - pi / 6, 0 ) ;
  icos[ 14 ].TurnX( - AC_1_3 ) ;
  icos[ 14 ].TurnY(   pi / 6 ) ;
  icos[ 14 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 16 ] = Tetraeder( 0, 1, 1, 0, 0, 0, pi,   - pi / 6, 0 ) ;
  icos[ 16 ].TurnX(   AC_1_3 ) ;
  icos[ 16 ].TurnY(   pi / 6 ) ;
  icos[ 16 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 19 ] = Tetraeder( 0, 1, 1, 0, 0, 0, pi, 7 * pi / 6, 0 ) ;
  icos[ 19 ].TurnX(   AC_1_3 ) ;
  icos[ 19 ].TurnY( - pi / 6 ) ;
  icos[ 19 ].Move( Vector( 0, 0, TETC ) ) ;
 }

inline int setvideomode( void )
 {
  return ( _setvideomode( VIDEO_MODE ) == 0 ) ;
 }

inline void setpixel( int x, int y, int r, int g, int b )
 {
  *( char* ) ( SCREEN + ( MAXY - y ) * ( MAXX + MAXX ) + x + MAXX )
          =  b * MAX_PAL2 + g * MAX_PAL + r + 16 ;
  return ;
 }
 
inline void putpixel( int x, int y, int color )
 {
  *( char* ) ( SCREEN + ( MAXY - y ) * ( MAXX + MAXX ) + x + MAXX ) = color ;
  return ;
 }
 
inline void setpalette( void )
 {
  unsigned short r, g, b, c ;
 
  pal[0x0] = _BLACK ;      pal[0x1] = _BLUE ;
  pal[0x2] = _GREEN ;      pal[0x3] = _CYAN ;
  pal[0x4] = _RED ;        pal[0x5] = _MAGENTA ;
  pal[0x6] = _BROWN ;      pal[0x7] = _WHITE ;
  pal[0x8] = _GRAY ;       pal[0x9] = _LIGHTBLUE ;
  pal[0xA] = _LIGHTGREEN ; pal[0xB] = _LIGHTCYAN ;
  pal[0xC] = _LIGHTRED ;   pal[0xD] = _LIGHTMAGENTA ;
  pal[0xE] = _YELLOW ;     pal[0xF] = _BRIGHTWHITE ;
  
  c = 0x10 ;
  for( b = 0 ; b < MAX_PAL ; b ++ )
   for( g = 0 ; g < MAX_PAL ; g ++ )
    for( r = 0 ; r < MAX_PAL ; r ++ )
     pal[ c ++ ] = ( ( ( bright [ b ] << 8 ) + bright [ g ] ) << 8 ) + bright [ r ] ;
  for( ; c < MAX_COLOR ; c ++ )
  pal[ c ] = 0 ;
  _remapallpalette( pal ) ;
 }
 
inline Vector RayBase( short x, short y )
 {
  return Vector( x, y * ASPECT, 0 ) ;
 }
 
inline Vector RayDir( short x, short y )
 {
  return Vector( x, y * ASPECT - 1.0e-6, MAXX ) ;
 }
 
inline int Bright( int l, Vector const& v, Vector const& n )
 {
  Vector w = v - light ;
  return ( l + ( int )( fabs( cos( w, n ) ) * LIGHT ) ) ;
 }
 
inline int Dim( int l, Vector const& v, Vector const& n )
 {
  Vector w = v - light ;
  return ( l + ( int )( fabs( cos( w, n ) ) * SHADOW ) ) ;
 }
 
inline void ShadowCast( short xs, short ys, Vector const& v,
                        Tetraeder& t, Facet const& f )
 {
  Vector r = light ;
  Vector d = v - light ;
  
  if( ( d * f.n ) <= 0 )
   {
    setpixel( xs, ys, Dim( t.r, v, f.n ),
                      Dim( t.g, v, f.n ),
                      Dim( t.b, v, f.n ) ) ;
    return ;
   }
  double owndist = t.Cross( r, d ) ;
  double dist ;
  for( int i = 0 ; i < 20 ; i ++ )
   if( &icos[ i ] != &t && icos[ i ].flag != 0 )
    {
     dist = icos[ i ].Cross( r, d ) ;
     if( dist < owndist && dist >= 0 )
      {
       setpixel( xs, ys, Dim( t.r, v, f.n ),
                         Dim( t.g, v, f.n ),
                         Dim( t.b, v, f.n ) ) ;
       return ;
      }
    }
  setpixel( xs, ys, Bright( t.r, v, f.n ),
                    Bright( t.g, v, f.n ),
                    Bright( t.b, v, f.n ) ) ;
  return ;
 }

inline void ShadowCast( short xs, short ys, Vector const& v, Wall const& w )
 {
  Vector r = light ;
  Vector d = v - light ;
  for( int i = 0 ; i < 20 ; i ++ )
   if( icos[ i ].Cross( r, d ) > 0 )
    {
     setpixel( xs, ys, Dim( w.r, v, w.n ),
                       Dim( w.g, v, w.n ),
                       Dim( w.b, v, w.n ) ) ;
     return ;
    }
  setpixel( xs, ys, Bright( w.r, v, w.n ),
                    Bright( w.g, v, w.n ),
                    Bright( w.b, v, w.n ) ) ;
  return ;
 }

inline void Trace( int x, int y )
 {
  Vector r = RayBase( x, y ) ;
  Vector d = RayDir( x, y ) ;
  short i, fn, mf ;
  double dist ;
  double mind = MAX_DOUBLE ;
  short min = -1 ;
  
  for( i = 0 ; i < 20 ; i ++ )		// i < 20 !!!
   {
    if( icos[ i ].flag == 0 ) continue ;
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
    if( walls[ i ].flag == 0 ) continue ;
    dist = walls[ i ].Cross( r, d ) ;
    if( dist < mind && dist >= 0 )
     {
      mind = dist ;
      min = i ;
     }
   }
  if( x == 0 )
   switch( min )
    {
     case 0 : walls[ 4 ].flag = 0 ; break ;
     case 3 : walls[ 0 ].flag = 0 ; break ;
     default: break ;
    }
  if( min == -1 ) setpixel( x, y, BACK_R, BACK_G, BACK_B ) ;
   else ShadowCast( x, y, r + d * mind, walls[ min ] ) ;
  return ;
 }

inline void Shot( int last = 0 )
 {
  short x, y ;
  walls[ 0 ].flag = 1 ;
  walls[ 3 ].flag = 1 ;
  for( x = 0 ; x < 20 ; x ++ )
   for( y = 0 ; y < 4 ; y ++ ) 
    {
     icos[ x ].facets[ y ].flag = 1 ;
     icos[ x ].facets[ y ].exist = 0 ;
    }
  for( y = 1 - MAXY ; y < MAXY ; y ++ )
   for( x = 1 - MAXX ; x < MAXX ; x ++ )
    Trace( x, y ) ;
  if( write( f, ( void* ) SCREEN, 64000 ) != 64000 )
   {
    _setvideomode( _TEXTC80 ) ;
    puts( "Can't write file.\n" ) ;
    exit( 2 ) ;
   }
  if( last || ( kbhit() && ( getch() == 27 ) ) )
   {
    shot_count ++ ;
    lseek( f, 0, 0 ) ;
    write( f, &shot_count, sizeof( int ) ) ;
    close( f ) ;
   }
  return ;
 }

int main ( void )
 {
  InitWalls() ;
  InitIcos1() ;
  InitIcos3() ;
  light = Vector( MAXX * 0.9, 0 /*MAXX * 0.9 */, MAXX * 0.8 ) ;
  f = open( "mg.sht", O_WRONLY | O_BINARY | O_CREAT ) ;
  if( f < 0 )
   {
    puts( "Can't create file.\n" ) ;
    return 1 ;
   }
  if( write( f, &shot_count, sizeof( int ) ) != sizeof( int ) )
   {
    puts( "Can't write file.\n" ) ;
    return 2 ;
   }
  if( setvideomode() )
   {
    puts( "Can't setup the video mode.\n" ) ;
    return 3 ;
   }
  setpalette() ;
  if( write( f, pal, 256 * sizeof( long ) ) != 256 * sizeof( long ) )
   {
    puts( "Can't write file.\n" ) ;
    return 2 ;
   }
   
  Shot( 1 ) ;
  _setvideomode( _TEXTC80 ) ;
  return 0 ;
 }
