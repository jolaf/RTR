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
#include "video.h"
#include "error.h"

#define pi 3.1415926535
#define N_SHOTS 52                      // Number of screen shots to generate
#define _MAX_WALL 9
#define D_FI ( 2 * pi / N_SHOTS )
#define D_MOVE ( 4 * 75 / N_SHOTS )

HEAD head = { 0, 1, 0, 0, 0, 0, 0 } ;
char& bits = head.bits ;
char& rle  = head.rle ;
char& mode = head.mode ;
char& dir  = head.dir ;
long& count = head.count ;
long& start = head.start ;
Color backcolor = Color( 0, 10, 20 ) ;
Color lightcolor = Color( 96, 144, 144 ) ;
Vector light ;
int MAX_WALL = _MAX_WALL ;
Wall walls[ _MAX_WALL ] ;
Tetraeder icos[ 20 ] ;
char* fname = "MG.SHT" ;
extern int SHOT_SIZE ;
extern int MAX_PAL ;
int f ;
char* p ;
extern int MAXX ;
extern int MAXY ;
double ASPECT = 1.2 ;
double TETC = ( MAXX * 0.75 ) ;
extern char* SCREEN ;
char* wr_buf ;
char mem = 0 ;
char quiet = 0 ;
char show = 0 ;
extern int page_size ;
extern unsigned short bytes_line ;
extern int lines_bank ;
extern int banks_shot ;
extern int rem_lines ;

extern void ShowFilm( int mode, int dir ) ;

inline void SetupVideo( void ) throw( errorcode )
 {
  if( bits == 24 ) return ;
  char i ;
  char pal[ 768 ] ;
  for( i = 0 ; i < 64 ; i ++ )
   memset( pal + ( i * 12 ), i, 12 ) ;
  if( mem == 0 ) MapAllPalette( pal ) ;
  if( write( f, pal, 768 ) != 768 )
   throw RTRER_CANTWRITEPAL ;
 }

void putpixel( int x, int y, int color )
 {
  if( bits == 8 )
   * ( SCREEN + ( MAXY - y ) * ( MAXX + MAXX ) + x + MAXX ) = ( char ) color ;
   else
    {
     char* p = SCREEN + ( ( MAXY - y ) % lines_bank ) * bytes_line
                      + 3 * ( x + MAXX ) ;
     //SetBank( ( MAXY - y ) / lines_bank ) ;
     * p ++ = ( char ) ( color ) ;
     * p ++ = ( char ) ( color >> 8 ) ;
     * p    = ( char ) ( color >> 16 ) ;
    }
 }

int getpixel( int x, int y )
 {
  if( bits == 8 )
   return ( * ( SCREEN + ( MAXY - y ) * ( MAXX + MAXX ) + x + MAXX ) ) ;
   else
    {
     char* p = SCREEN + ( ( MAXY - y ) % lines_bank ) * bytes_line
                      + 3 * ( x + MAXX ) ;
     //SetBank( ( MAXY - y ) / lines_bank ) ;
     return ( ( * p ++ ) + ( * p ++ ) << 8 + ( * p ) << 16 ) ;
    }
 }

void setpixel( int x, int y, Color const& color )
 {
  if( bits == 8 ) * p ++ = ( char ) bright( color ) ;
   else
    {
     if( mem == 0 && x == - MAXX )
      {
       int d = ( MAXY - y ) % lines_bank ;
       if( d == 0 ) SetBank( ( MAXY - y ) / lines_bank ) ;
       p = SCREEN + d * bytes_line ;
      }
     * p ++ = ( char ) color.b ;
     * p ++ = ( char ) color.g ;
     * p ++ = ( char ) color.r ;
    }
 }

void InitShot( void )
 {
  p = SCREEN ;
 }
 
void WriteShot( void ) throw( errorcode )
 {
  if( rle )
   {
    int size = 0 ;
    char* p = SCREEN ;
    char* t = wr_buf + sizeof( int ) ;
    char count ;
    int n = 0 ;
    if( bits == 8 )
     {
      char c ;
      while( n < SHOT_SIZE )
       {
        c = *p ;
        n ++ ;
        count = 0 ;
        while( *( ++ p ) == c && count < 62 && n < SHOT_SIZE )
         { count ++ ; n ++ ; }
        size ++ ;
        if( c >= 192 || count ) 
         {
          * t ++ = ( char ) ( count + 193 ) ;
          size ++ ;
         }
        * t ++ = c ;
       }
     }
     else
      {
       int c ;
       if( mem )
        while( n < SHOT_SIZE )
         {
          c = ( * ( ( int* ) p ) ) & 0x00FFFFFF ;
          n += 3 ;
          count = 0 ;
          while( ( ( ( * ( ( int* ) ( p += 3 ) ) ) & 0x00FFFFFF ) == c )
                 && count < 62 && n < SHOT_SIZE )
           { count ++ ; n += 3 ; }
          size += 3 ;
          if( ( ( ( char ) c ) >= 192 ) || count )
           {
            * t ++ = ( char ) ( count + 193 ) ;
            size ++ ;
           }
          ( * ( int* ) t ) = c ;
          t += 3 ;
         }
        else
         {
          int i, j ;
          for( i = 0 ; i <= banks_shot ; i ++ )
           {
            SetBank( i ) ;
            for( j = 0 ; j < ( ( i == banks_shot ) ? rem_lines : lines_bank ) ; j ++ )
             {
              p = SCREEN + j * bytes_line ;
              n = 0 ;
              while( n < MAXX * 6 )
               {
                c = ( * ( ( int* ) p ) ) & 0x00FFFFFF ;
                n += 3 ;
                count = 0 ;
                while( ( ( ( * ( ( int* ) ( p += 3 ) ) ) & 0x00FFFFFF ) == c )
                     && count < 62 && n < MAXX * 6 )
                 { count ++ ; n += 3 ; }
                size += 3 ;
                if( ( ( char ) c >= 192 ) || count )
                 {
                  * t ++ = ( char ) ( count + 193 ) ;
                  size ++ ;
                 }
                ( * ( int* ) t ) = c ;
                t += 3 ;
               } 
             }
           }
         }
      }
    ( * ( int* ) wr_buf ) = size ;
    size += sizeof( int ) ;
    if( write( f, wr_buf, size ) != size ) throw RTRER_CANTWRITESHOT ;
   }   
   else
    if( bits == 8 || mem )
     {
      if( write( f, SCREEN, SHOT_SIZE ) != SHOT_SIZE ) throw RTRER_CANTWRITESHOT ;
     }
     else
      {
       int i, j ;
       for( i = 0 ; i <= banks_shot ; i ++ )
        {
         SetBank( i ) ;
         for( j = 0 ; j < ( ( i == banks_shot ) ? rem_lines : lines_bank ) ; j ++ )
          if( write( f, SCREEN + ( j * bytes_line ), MAXX * 6 ) != MAXX * 6 ) throw RTRER_CANTWRITESHOT ;
        }
      }
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
  char chvid = 0 ;
  char chmode = 0 ;
  char chdir = 0 ;
  char charc = 0 ;
  char chrend = 0 ;
  char vmode = 0 ;
  for( i = 1 ; i < argc ; i ++ )
   strlwr( argv[ i ] ) ;
  try
   {
    if( strcmp( argv[ 1 ], "render" ) == 0 || strcmp( argv[ 1 ], "r" ) == 0 )
     {
      cout << "RENDERING mode active" << endl << endl ;
      show = 0 ;
     }
     else if( strcmp( argv[ 1 ], "show" ) == 0 || strcmp( argv[ 1 ], "s" ) == 0 )
      {
       cout << "SHOWING mode active" << endl << endl ;
       show = 1 ;
      }
      else 
       {
        cout << endl << "Command not found" << endl << endl ;
        PrintHelp() ;
       }
    if( strcmp( argv[ 2 ], "quiet" ) == 0 || strcmp( argv[ 2 ], "q" ) == 0 )
     {
      quiet = 1 ;
      i = 3 ;
     }
     else i = 2 ;
    if( argv[ i ] == NULL || SwitchSymbol( *argv[ i ] ) ) 
     {
      cout << "No file name specified" << endl << endl ;
      PrintHelp() ;
     }
    fname = strupr( argv[ i ++ ] ) ;
    for( ; i < argc ; i ++ )
     {
      if( SwitchSymbol( *argv[ i ] ) )
       {
        int l = strlen( argv[ i ] ) ;
        int j ;
        char* p = argv[ i ] ;
        for( j = 1 ; j < l ; j ++ )
         {
          p ++ ;
          if( SwitchSymbol( *p ) ) continue ;
          switch( *p )
           {
            case 'b' : case 'f' :
             if( chdir || !show ) throw p ;
             dir = ( argv[ i ][ 1 ] == 'b' ) ;
             chdir = 1 ;
             break ;
            case 'c' : case 't' : case 'p' :
             if( chmode ) throw p ;
             switch( *p )
              {
               case 'c' : mode = 0 ; break ;
               case 't' : mode = 1 ; break ;
               case 'p' : mode = 2 ; break ;
              }
             chmode = 1 ;
             break ;
            case 'v' : case 's' : case 'a' :
             if( chvid || show ) throw p ;
             switch( *p )
              {
               case 'v': vmode = 1 ; break ;
               case 's': vmode = 2 ; break ;
               case 'a': vmode = 0 ; break ;
              }
             chvid = 1 ;
             break ;
            case 'r' : case 'n' :
             if( charc || show ) throw p ;
             rle = ( *p == 'r' ) ;
             charc = 1 ;
             break ;
            case 'd' : case 'm' :
             if( chrend || show ) throw p ;
             mem = ( *p == 'm' ) ;
             chrend = 1 ;
             break ;
            default :
             throw p ;
           }
         }  
       }
       else throw argv[ i ] ;
     }
    if( show ) ShowFilm( chmode ? mode : -1, chdir ? dir : -1 ) ;
    if( vmode == 0 && mem ) throw RTRER_MEMANDNOTVMODE ;
    f = open( fname, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO );
    if( f < 0 ) throw RTRER_CANTCREATEFILE ;
    if( !quiet ) 
     cout << "Picture mode: "
          << ( ( vmode == 0 ) ? "Auto" : 
             ( ( vmode == 1 ) ? "VGA 13h 320x200x256" :
                                "VESA 10Fh 320x200x16.8M" ) ) << endl
          << "Render to: " << ( mem ? "Memory" : "Screen" ) << endl
          << "Target file: " << fname << endl
          << "Archivation: " << ( rle ? "RLE" : "None" ) << endl
          << "Showing mode: "
          << ( ( mode == 0 ) ? "Cycling" :
             ( ( mode == 1 ) ? "Back-and-Forth" : "Ping-Pong" ) ) << endl
          << "Starting direction: " << ( dir ? "Backward" : "Forward" ) << endl
          << "Shots to render: " << N_SHOTS << endl << endl ;
    if( mem )
     {
      if( vmode == 1 )
       {
        bits = 8 ;
        SHOT_SIZE = 64000 ;
        cout << "Rendering to memory in VGA 13h 320x200x256 mode:" << endl
             << "Shot:        Line: " ;
       }
       else
        {
         bits = 24 ;
         SHOT_SIZE = 192000 ;
         cout << "Rendering to memory in VESA 10Fh 320x200x16.8M mode:" << endl
              << "Shot:        Line: " ;
        }
      if( ( SCREEN = ( char* ) malloc( SHOT_SIZE ) ) == NULL ) throw RTRER_CANTALLOCBUF ;
     }
    if( write( f, &head, sizeof( HEAD ) ) != sizeof( HEAD ) ) throw RTRER_CANTWRITEHEADER ;
    if( mem == 0 )
      switch( vmode )
       {
        case 0 :
         if( SetVESA168Mode() )
          {
           if( SetVGA256Mode() ) throw RTRER_CANTSETVESANORVGA ;
           bits = 8 ;
          }
          else bits = 24 ;
          break ;
        case 1 :
         if( SetVGA256Mode() ) throw RTRER_CANTSETVGA ;
         bits = 8 ;
         break ;
        case 2 :
         if( SetVESA168Mode() ) throw RTRER_CANTSETVESA ;
         bits = 24 ;
         break ;
       }
    if( rle && ( ( wr_buf = ( char* ) malloc( SHOT_SIZE ) ) == NULL ) ) throw RTRER_CANTALLOCARCHIVE ;
    SetupVideo() ;
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
    if( errmes[ n ].text_mode ) TextMode() ;
    if( errmes[ n ].close_file )
     {
      close( f ) ;
      if( show == 0 ) unlink( fname ) ;
     }
    if( mem ) cout << endl ;
    cout << errmes[ n ].mes << endl ;
    return n ;
   }
  catch( char* p )
   {
    cout << "Incorrect switch: "
         << p << endl << endl ;
    PrintHelp() ;
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
