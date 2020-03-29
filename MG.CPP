#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <malloc.h>
#include <iostream.h>
#include "vector3d.h"
#include "color.h"
#include "mglib.h"
#include "facets.h"
#include "trace.h"
#include "header.h"
#include "error.h"

#define pi 3.1415926535
#define N_SHOTS ( 52 * 4 )                      // Number of screen shots to generate
#define _MAX_WALL 9
#define D_FI ( 2 * pi / N_SHOTS )
#define D_MOVE ( 4 * 75 * 5 / N_SHOTS )

HEAD head = { 24, 0, 0, 0, 0, 0, 0 } ;
long& count = head.count ;
Color backcolor = Color( 0, 10, 20 ) ;
Color lightcolor = Color( 96, 144, 144 ) ;
Vector light ;
int MAX_WALL = _MAX_WALL ;
Wall walls[ _MAX_WALL ] ;
Tetraeder icos[ 20 ] ;
char* fname ;
int f ;
char* p ;
int MAXX = 800 ;
int MAXY = 600 ;
int SHOT_SIZE = MAXX * MAXY * 4 * 3;
double ASPECT = 1.0 ;
double TETC = ( MAXX * 0.75 ) ;
char* SCREEN = NULL ;

void setpixel( int _x, int _y, Color const& color )
 {
  * p ++ = ( char ) color.b ;
  * p ++ = ( char ) color.g ;
  * p ++ = ( char ) color.r ;
 }

void InitShot( void )
 {
  p = SCREEN ;
 }
 
void WriteShot( void ) throw( errorcode )
 {
  if( write( f, SCREEN, SHOT_SIZE ) != SHOT_SIZE ) throw RTRER_CANTWRITESHOT ;
  count ++ ;
  if( lseek( f, 0, SEEK_SET ) != 0 ) throw RTRER_ERRORSEEKING ;
  if( write( f, &head, sizeof( HEAD ) ) != sizeof( HEAD ) ) throw RTRER_CANTSETHEADER ;
  lseek( f, 0, SEEK_END ) ;
 }
 
int main ( int argc, char* argv[] )
 {
  int i ;
  double fi ;
  double lr ;
  double mv ;
  try
   {
    if( argc < 2 ) 
     {
      cout << "No file name specified" << endl << endl ;
      PrintHelp() ;
     }
    fname = strupr( argv[ 1 ] ) ;
    f = open( fname, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO );
    if( f < 0 ) throw RTRER_CANTCREATEFILE ;
    cout << "Rendering " << MAXX * 2 << "x" << MAXY * 2 << ':' << endl
         << "Shot:        Line: " ;
    if( ( SCREEN = ( char* ) malloc( SHOT_SIZE ) ) == NULL ) throw RTRER_CANTALLOCBUF ;
    if( write( f, &head, sizeof( HEAD ) ) != sizeof( HEAD ) ) throw RTRER_CANTWRITEHEADER ;
    InitWalls() ;
    fi = 0 ;
    mv = 0 ;
    lr = MAXX * 0.9 ;
    for( i = 0 ; i < N_SHOTS ; i ++ )
     {
      light = Vector( lr * cos( fi ), lr * sin( fi ),
                      lr * sin( fi ) * 0.72 + MAXX * 0.75 ) ;
      InitIcos( mv ) ;
      TurnIcos( - 2 * fi ) ;
      Shot() ;
      if( ( i > ( N_SHOTS / 4 ) ) && ( i <= ( 3 * N_SHOTS / 4 ) ) )
       if( i <= ( N_SHOTS / 2 ) ) mv += D_MOVE ;
        else mv -= D_MOVE ;
      fi += D_FI ;
     }
    close( f ) ;
    Successful() ;
   }
  catch( errorcode n )
   {
    if( errmes[ n ].close_file )
     {
      close( f ) ;
      unlink( fname ) ;
     }
    cout << endl << errmes[ n ].mes << endl ;
    return n ;
   }
  catch( ... )
   {
    cout << "*** FATAL *** Unhandled exception. Report to VMZ."
         << endl << endl ;
    return -1 ;
   }
  cout << "*** FATAL *** Unconditional termination. Report to VMZ."
         << endl << endl ;
  return -2 ;
 }
