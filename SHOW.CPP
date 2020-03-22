#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <i86.h>
#include <io.h>
#include <iostream.h>
#include "mglib.h"
#include "header.h"
#include "video.h"
#include "error.h"

struct TAB
 {
  long ofs ;
  long size ;
 } * tab ;

extern int MAXX ;
extern int MAXY ;
extern int page_size ;
extern unsigned short bytes_line ;
extern int lines_bank ;
extern int banks_shot ;
extern int rem_lines ;
extern char quiet ;
extern char* fname ;
extern int SHOT_SIZE ;
extern char* SCREEN ;
extern int f ;

char Bits ;
char Rle ;
long size ;
char* buf ;
char* arc ;

inline void Show( int n ) throw( errorcode )
 {
  int i, j ;
  int size ;
  lseek( f, ( Rle ? tab[ n ].ofs : ( SHOT_SIZE * n + sizeof( HEAD ) ) ), SEEK_SET ) ;
  if( Rle )
   {
    size = tab[ n ].size ;
    long n = 0 ;
    char* t = buf ;
    char* p = arc ;
    if( read( f, arc, size ) != size ) throw RTRER_CANTREADFILE ;
    if( Bits == 8 )
     while( n < size )
      {
       if( *p < 193 ) * t ++ = * p ;
        else
         {
          for( i = ( char ) ( ( * p ++ ) - 192 ) ; i > 0 ; i -- )
           * t ++ = *p ;
          n ++ ;
         }
       p ++ ;
       n ++ ;
      }
     else
      while( n < size )
       {
        if( *p < 193 )
         {
          ( * ( ( long* ) t ) ) = ( * ( ( long* ) p ) ) ;
          t += 3 ;
         }
         else
          {
           for( i = ( char ) ( ( * p ++ ) - 192 ) ; i > 0 ; i -- )
            {
             ( * ( ( long* ) t ) ) = ( * ( ( long* ) p ) ) ;
             t += 3 ;
            }
           n ++ ;
          }
        p += 3 ;
        n += 3 ;
       }
   }
   else
    if( read( f, buf, SHOT_SIZE ) != SHOT_SIZE ) throw RTRER_CANTREADFILE ;
  if( Bits == 8 ) memcpy( SCREEN, buf, SHOT_SIZE ) ;
   else
    for( i = 0 ; i <= banks_shot ; i ++ )
     {
      SetBank( i ) ;
      for( j = 0 ; j < ( ( i == banks_shot ) ? rem_lines : lines_bank ) ; j ++ )
       memcpy( SCREEN + bytes_line * j, buf + ( ( i * lines_bank + j ) * MAXX * 6 ), MAXX * 6 ) ;
     }
 }

void ShowFilm( int _mode, int _dir ) throw( errorcode )
 {
  long i ;
  int c ;
  int d ;
  char pause = 1 ;
  char pal[ 768 ] ;
  HEAD head ;
  long& count = head.count ;
  long& start = head.start ;
  int mode ;
  int dir ;
  
  f = open( fname, O_RDONLY | O_BINARY ) ;
  if( f < 0 ) throw RTRER_CANTOPENFILE ;
  if( read( f, &head, sizeof( HEAD ) ) != sizeof( HEAD ) ) throw RTRER_CANTREADHEADER ;
  if( count == 0 ) throw RTRER_NOSCREENSHOTS ;
  Bits = head.bits ; Rle = head.rle ;
  if( _mode != -1 ) mode = _mode ; else mode = head.mode ;
  if( _dir != -1 ) dir = _dir ; else dir = head.dir ;
  dir = ( int ) ( 1 - 2 * dir ) ;
  if( !quiet )
   cout << "Film file: " << fname << endl
        << "Picture: " << ( ( Bits == 8 ) ?
           "VGA 13h 320x200x256 Mode" : "VESA 10Fh 320x200x16.8M Mode" ) << endl
        << "Archivation: " << ( Rle ? "RLE" : "None" ) << endl
        << "Showing mode: " << ( ( mode == 0 ) ? "Cycling" : 
                               ( ( mode == 1 ) ? "Back-and-Forth" :
                               "Ping-Pong" ) ) << endl
        << "Starting direction: " << ( ( dir == 1 ) ? "Forward" : "Backward" ) << endl
        << "Shots inside: " << count << endl << endl ;
  if( Rle )
   {
    tab = ( TAB* ) malloc( count * sizeof( TAB ) ) ;
    if( tab == NULL ) throw RTRER_CANTALLOCTABLE ;
   }
  if( Bits == 8 )
   {
    if( read( f, pal, 768 ) != 768 ) throw RTRER_CANTREADPAL ;
    if( SetVGA256Mode() ) throw RTRER_CANTSETVGA ;
    MapAllPalette( pal ) ;
   }
   else if( SetVESA168Mode() ) throw RTRER_CANTSETVESA ;
  buf = ( char* ) malloc( SHOT_SIZE ) ;
  if( buf == NULL ) throw RTRER_CANTALLOCSHOW ;
  if( Rle )
   {
    arc = ( char* ) malloc( SHOT_SIZE ) ;
    if( arc == NULL ) throw RTRER_CANTALLOCARCHIVE ;
    long ofs = sizeof( HEAD ) ;
    if( Bits == 8 ) ofs += 768 ;
    for( i = 0 ; i < count ; i ++ )
     {
      if( lseek( f, ofs, SEEK_SET) != ofs ) throw RTRER_ERRORSEEKING ;
      tab[ i ].ofs = ofs += sizeof( long ) ;
      if( read( f, &tab[ i ].size, sizeof( long ) ) != sizeof( long ) ) throw RTRER_CANTREADFILE ;
      ofs += tab[ i ].size ;
     }
   }
  if( count == 1 )
   {
    Show( 0 ) ;
    getch() ;
    while( kbhit() ) getch() ;
    Successful() ;
   }
  i = start ;
  d = dir ;
  if( mode == 2 ) d = ( int ) ( - d ) ;
  while( 1 )
   {
    Show( i ) ;
    do
     {
      while( kbhit() )
       switch( c = getch() )
        {
         case '-' : if( pause < 11 ) pause ++ ; break ;
         case '+' : if( pause >  0 ) pause -- ; break ;
         case  27 : Successful() ;
         case   0 : c = getch() ;
                    switch( c )
                     {
                      case 'K' : ;
                      case 'M' : ;
                     }
        }
     }
     while( pause >= 10 ) ;
    if( mode == 1 )
     {
      if( i == start ) d = dir ;
       else 
        if( ( i == start - dir ) ||
            ( i == count - 1 && start == 0 && dir == 1 ) ||
            ( i == 0 && start == count - 1 && dir == - 1  ) ) 
          d = ( int ) ( - dir ) ;
     }
     else if( mode == 2 && i == start ) d = ( int ) ( - d ) ;
    i += d ;
    if( i == count ) i = 0 ;
     else if( i == -1 ) i = count - 1 ;
    delay( pause * 100 ) ;
   }
 }
