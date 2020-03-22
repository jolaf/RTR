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
#define MAX_PAL 256
#define LIGHT_R 128
#define LIGHT_G 128
#define LIGHT_B 128
#define BLICK 30
#define SHADOW 0
#define MAX_PAL2 65536
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
#define DEFAULT_R 30
#define DEFAULT_G 30
#define DEFAULT_B 30
#define TETC ( MAXX * 0.75 )
#define pi ( 3.1415926535 )
#define N_SHOTS 12
#define MAX_LIST 96

double max_dist ;

class Color
 {
  public:
  char r, g, b ;
  Color( int _r = 0, int _g = 0, int _b = 0 ) ;
 } ;
 
Color::Color( int _r, int _g, int _b )
 {
  r = _r ; g =_g ; b = _b ;
 }

Color operator + ( Color const& a, Color const& b )
 {
  return Color( a.r + b.r, a.g + b.g, a.b + b.b ) ;
 }

Color operator * ( Color const& a, Color const& b )
 {
  return Color( ( ( unsigned ) a.r * ( unsigned ) b.r ) >> 8,
                ( ( unsigned ) a.g * ( unsigned ) b.g ) >> 8,
                ( ( unsigned ) a.b * ( unsigned ) b.b ) >> 8 ) ;
 }

Color operator * ( Color const& a, double d )
 {
  return Color( a.r * d, a.g * d, a.b * d ) ;
 }
 
Color operator * ( double d, Color const& a )
 {
  return Color( a.r * d, a.g * d, a.b * d ) ;
 }


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

struct FList
 {
  Facet* f ;
  double dist ;
  Color color ;
  double trp ;
 } ;

class Wall : public Facet
 {
  public:
  Color color ;
  double trp ;
  
  Wall( void ) { }
  Wall( double x, double y, double z, double _k, char _r, char _g, char _b, double _trp, char _flag = 1 )
   {
    k = _k ; flag = _flag ; trp = _trp ;
    color = Color( _r, _g, _b ) ;
    n = Vector( x, y, z ) ;
   }
  int Cross( Vector const& r, Vector const& d, FList fl[], int& count )
   {
    double dist = ( ( - k - n * r ) / ( n * d ) ) ;
    if( dist < 0 || dist >= max_dist ) return 0 ;
    fl[ count ].f = this ;
    fl[ count ].dist = dist ;
    fl[ count ].color = color ;
    fl[ count ].trp = trp ;
    count ++ ;
    if( trp == 0 ) max_dist = dist ;
    return 1 ;
   }
 } ;

class Tetraeder
 {
  public:
  Facet facets[ 4 ] ;
  Vector a ;
  double ax, ay, az ;
  Color color ;
  double l ;
  double trp ;
  char flag ;
  
  Tetraeder( char _r = DEFAULT_R, char _g = DEFAULT_G , char _b = DEFAULT_B,
             double _trp = 0, double x = 0, double y = 0, double z = 0,
             double _ax = 0, double _ay = 0, double _az = 0, char _flag = 1,
             double _l = DEFAULT_LEN ) ;
  void TurnX( double fi, double y = 0, double z = 0 ) ;
  void TurnY( double fi, double x = 0, double z = 0 ) ;
  void TurnZ( double fi, double x = 0, double y = 0 ) ;
  void Move ( Vector b ) ;
  int  Cross( Vector const& r, Vector const& d, FList fl[], int& count ) ;
 } ;

Tetraeder::Tetraeder( char _r, char _g, char _b, double _trp, double x, double y, double z,
                      double _ax, double _ay, double _az, char _flag, double _l )
 {
  l = _l ; flag =_flag ; trp =_trp ;
  color = Color( _r, _g, _b ) ;
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

inline int Tetraeder::Cross( Vector const& r, Vector const& d, FList fl[], int& count )
 {
  double t ;
  double tn = - MAX_DOUBLE ;
  double tf = MAX_DOUBLE ;
  short min = -1 ;
  short max = -1 ;
  
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
      if( t < tf )
       {
        tf = t ;
        max = i ;
       }
    if( tn > tf ) return 0 ;
   }
  if( tn < max_dist && tn > 0 )
   {
    fl[ count ].f = & facets[ min ] ;
    fl[ count ].dist = tn ;
    fl[ count ].color = color ;
    fl[ count ].trp = trp ;
    count ++ ;
    if( trp == 0 ) max_dist = tn ;
   }
  if( tf < max_dist && tf > 0 )
   {
    fl[ count ].f = & facets[ max ] ;
    fl[ count ].dist = tf ;
    fl[ count ].color = color ;
    fl[ count ].trp = trp ;
    count ++ ;
    if( trp == 0 ) max_dist = tf ;
   }
  return 1 ;
 }

short bright [ MAX_PAL ] = { 0, 12, 25, 38, 51, 63 } ;
Wall walls[ MAX_WALL ] ;
Tetraeder icos[ 20 ] ;
long pal[ MAX_COLOR ] ;
FList tlist[ MAX_LIST ], llist [ MAX_LIST ] ;
int tlc, llc ;
Vector light ;
int shot_count = 0 ;
int f ;

inline void InitWalls( void )
 {
  walls[ 0 ] = Wall(  0,  0, -1, MAXX * 1.5, 40, 0, 0, 0.5 ) ;
  walls[ 1 ] = Wall(  1,  0,  0, MAXX      , 40, 0, 0, 0 ) ;
  walls[ 2 ] = Wall( -1,  0,  0, MAXX      , 40, 0, 0, 0 ) ;
  walls[ 3 ] = Wall(  0, -1,  0, MAXX      , 40, 0, 0, 0 ) ;
  walls[ 4 ] = Wall(  0,  1,  0, MAXX      , 40, 0, 0, 0 ) ;
  walls[ 5 ] = Wall(  _1_S3, -_1_S3, -_1_S3, MAXX * 1.8, 40, 0, 0, 0 ) ;
  walls[ 6 ] = Wall( -_1_S3, -_1_S3, -_1_S3, MAXX * 1.8, 40, 0, 0, 0 ) ;
  walls[ 7 ] = Wall( -_1_S3,  _1_S3, -_1_S3, MAXX * 1.8, 40, 0, 0, 0 ) ;
  walls[ 8 ] = Wall(  _1_S3,  _1_S3, -_1_S3, MAXX * 1.8, 40, 0, 0, 0 ) ;
 }

inline void InitIcos1( void )
 { 
  icos[ 0 ]  = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC,        0,        0,    - DANG ) ;
  icos[ 1 ]  = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC,        0,        0, pi - DANG ) ;
  icos[ 3 ]  = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC,   DANG - pi / 2, 0,    pi / 2 ) ;
  icos[ 4 ]  = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC, - DANG - pi / 2, 0,  - pi / 2 ) ;
  icos[ 6 ]  = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC,   pi / 2, - DANG + pi / 2,  0 ) ;
  icos[ 8 ]  = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC,       pi,        0,      DANG ) ;
  icos[ 9 ]  = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC, - pi / 2,   DANG + pi / 2,  0 ) ;
  icos[ 12 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC, - pi / 2,   DANG - pi / 2,  0 ) ;
  icos[ 13 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC,        0,       pi,      DANG ) ;
  icos[ 15 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC,   pi / 2, - DANG - pi / 2,  0 ) ;
  icos[ 17 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC, - DANG + pi / 2, 0,    pi / 2 ) ;
  icos[ 18 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, TETC,   DANG + pi / 2, 0,  - pi / 2 ) ;
 }
 
inline void InitIcos3( void )
 {
  icos[  2 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, 0,  0,     pi / 6, 0 ) ;
  icos[  2 ].TurnX(   AC_1_3 ) ;
  icos[  2 ].TurnY( - pi / 6 ) ;
  icos[  2 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[  5 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, 0,  0, 5 * pi / 6, 0 ) ;
  icos[  5 ].TurnX(   AC_1_3 ) ;
  icos[  5 ].TurnY(   pi / 6 ) ;
  icos[  5 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[  7 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, 0, pi, 5 * pi / 6, 0 ) ;
  icos[  7 ].TurnX( - AC_1_3 ) ;
  icos[  7 ].TurnY(   pi / 6 ) ;
  icos[  7 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 10 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, 0, pi,     pi / 6, 0 ) ;
  icos[ 10 ].TurnX( - AC_1_3 ) ;
  icos[ 10 ].TurnY( - pi / 6 ) ;
  icos[ 10 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 11 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, 0,  0, 7 * pi / 6, 0 ) ;
  icos[ 11 ].TurnX( - AC_1_3 ) ;
  icos[ 11 ].TurnY( - pi / 6 ) ;
  icos[ 11 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 14 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, 0,  0,   - pi / 6, 0 ) ;
  icos[ 14 ].TurnX( - AC_1_3 ) ;
  icos[ 14 ].TurnY(   pi / 6 ) ;
  icos[ 14 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 16 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, 0, pi,   - pi / 6, 0 ) ;
  icos[ 16 ].TurnX(   AC_1_3 ) ;
  icos[ 16 ].TurnY(   pi / 6 ) ;
  icos[ 16 ].Move( Vector( 0, 0, TETC ) ) ;
  icos[ 19 ] = Tetraeder( 40, 0, 0, 0.5, 0, 0, 0, pi, 7 * pi / 6, 0 ) ;
  icos[ 19 ].TurnX(   AC_1_3 ) ;
  icos[ 19 ].TurnY( - pi / 6 ) ;
  icos[ 19 ].Move( Vector( 0, 0, TETC ) ) ;
 }

inline int setvideomode( void )
 {
  return ( _setvideomode( VIDEO_MODE ) == 0 ) ;
 }

inline void setpixel( int x, int y, Color c )
 {
  *( char* ) ( SCREEN + ( MAXY - y - 1 ) * ( MAXX + MAXX ) + x + MAXX ) = c.r ;
 }
 
inline void putpixel( int x, int y, int color )
 {
  *( char* ) ( SCREEN + ( MAXY - y - 1 ) * ( MAXX + MAXX ) + x + MAXX ) = color ;
 }
 
inline void setpalette( void )
 {
  unsigned i, j ;
  for( i = 0 ; i < ( MAX_PAL >> 2 ) ; i ++ )
   for( j = 0 ; j < 4 ; j ++ ) 
    pal[ ( i << 2 ) + j ] = ( ( ( i << 8 ) + i ) << 8 ) + i ;
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

inline int SLComp( const void* _a, const void* _b )
 {
  double a = ( ( FList* ) _a )->dist ;
  double b = ( ( FList* ) _b )->dist ;
  return ( ( a == b ) ? 0 : ( a > b ) ? 1 : ( -1 ) ) ;
 }

inline int FLComp( const void* _a, const void* _b )
 {
  double a = ( ( FList* ) _a )->dist ;
  double b = ( ( FList* ) _b )->dist ;
  return ( ( a == b ) ? 0 : ( a > b ) ? ( -1 ) : 1 ) ;
 }
 
inline Color ShadowCast( Vector const& v )
 {
  Vector r = light ;
  Vector d = v - light ;
  int i ;
  Color c = Color( LIGHT_R, LIGHT_G, LIGHT_B ) ;
  
  llc = 0 ;
  max_dist = 1 ;
  for( i = 0 ; i < 20 ; i ++ )
   {
    if( icos[ i ].flag == 0 ) continue ;
    icos[ i ].Cross( r, d, llist, llc ) ;
   }
  for( i = 0 ; i < MAX_WALL ; i ++ )
   {
    if( walls[ i ].flag == 0 ) continue ;
    walls[ i ].Cross( r, d, llist, llc ) ;
   }
  if( llc == 0 ) return c ;
  qsort( llist, llc, sizeof( FList ), & SLComp ) ;
  for( i = llc - 1 ; i >= 0 ; i -- )
   if( llist[ i ].trp == 0 )
    {
     c = llist[ i ].color ;
     break ;
    }
  i ++ ;
  for( ; i < llc ; i ++ )
   c = ( c * ( fabs( cos( llist[ i ].f->n, d ) ) * llist[ i ].trp ) ) * llist[ i ].color ;
  return c ;
 }

inline void Trace( int x, int y )
 {
  Vector r = RayBase( x, y ) ;
  Vector d = RayDir( x, y ) ;
  Vector v ;
  short i ;
  double mind = MAX_DOUBLE ;
  short min = -1 ;
  Color c, sc ;

  tlc = 0 ;
  max_dist = MAX_DOUBLE ;
  for( i = 0 ; i < 20 ; i ++ )
   {
    if( icos[ i ].flag == 0 ) continue ;
    icos[ i ].Cross( r, d, tlist, tlc ) ;
   }
  for( i = 0 ; i < MAX_WALL ; i ++ )
   {
    if( walls[ i ].flag == 0 ) continue ;
    walls[ i ].Cross( r, d, tlist, tlc ) ;
   }
  if( tlc == 0 )
   {
    setpixel( x, y, Color( 0, 0, 0 ) ) ;
    return ;
   }
  qsort( tlist, tlc, sizeof( FList ), & FLComp ) ;
  for( i = tlc - 1 ; i >= 0 ; i -- )
   if( tlist[ i ].trp == 0 ) break ;
  c = tlist[ i ].color ;
  v = r + d * tlist[ i ].dist ;
  if( ( d * tlist[ i ].f->n ) * ( ( v - light ) * tlist[ i ].f->n ) > 0 )
   c = c + ShadowCast( v ) * fabs( cos( tlist[ i ].f->n, v - light ) ) ; // * tlist[ i ].color ;
  i ++ ;
  for( ; i < tlc ; i ++ )
   {
    v = r + d * tlist[ i ].dist ;
    sc = ShadowCast( v ) ;
    if( ( d * tlist[ i ].f->n ) * ( ( v - light ) * tlist[ i ].f->n ) > 0 )
     sc = sc * ( 1 - tlist[ i ].trp ) ;
     else sc = sc * tlist[ i ].trp ;
    c = ( c * tlist[ i ].trp + sc ) * fabs( cos( tlist[ i ].f->n, d ) ) * tlist[ i ].color ;
   }
  setpixel( x, y, c ) ;
  return ;
 }

inline int Shot( void )
 {
  short x, y ;
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
  for( y = -MAXY ; y < MAXY ; y ++ )
   for( x = -MAXX ; x < MAXX ; x ++ )
    Trace( x, y ) ;
  if( write( f, ( void* ) SCREEN, 64000 ) != 64000 )
   {
    _setvideomode( _TEXTC80 ) ;
    puts( "Can't write file.\n" ) ;
    exit( 2 ) ;
   }
  shot_count ++ ; 
  lseek( f, 0, SEEK_SET ) ;
  write( f, &shot_count, sizeof( int ) ) ;
  lseek( f, 0, SEEK_END ) ;
  if( ( kbhit() && ( getch() == 27 ) ) )
   {
    close( f ) ;
    return 1 ;
   }
  return 0 ;
 }

int main ( void )
 {
  int i ;
  double fi ;
  double lr ;
  
  InitWalls() ;
  InitIcos1() ;
  InitIcos3() ;
  f = open( "mg.sht", O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IRWXU | S_IRWXG | S_IRWXO );
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
   
  fi = 0 ;
  lr = MAXX * 0.9 ;
  for( i = 0 ; i < N_SHOTS ; i ++ )
   {
    light = Vector( lr * cos( fi ), lr * sin( fi ),
                    lr * sin( fi ) * 0.72 + MAXX * 0.75 ) ;
    if( Shot() ) return 0 ;
    fi += 2 * pi / N_SHOTS ;
   }
  close( f ) ;
  _setvideomode( _TEXTC80 ) ;
  return 0 ;
 }
