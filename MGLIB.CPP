#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream.h>
#include <io.h>
#include "video.h"

extern int MAXX ;
extern int MAXY ;
extern int f ;
extern char* fname ;
extern long& count ;
extern char text_buf[] ;
extern rccoord text_pos ;
extern int TEXT_SCREEN ;
extern int TEXT_SIZE ;
extern char mem ;
extern char show ;
extern int lines_bank ;
extern char& bits ;
extern char quiet ;

extern void putpixel( int x, int y, int color ) ;
extern int  getpixel( int x, int y ) ;
extern void SetBank( int n ) ;
extern void TextMode( void ) ;

inline void digit( int x, int y, char c )
 {
  static char digits[ 10 ][ 5 ] = { { 15,  9,  9,  9, 15 },
       {  1, 1,  1, 1, 1 }, { 15, 1, 15, 8, 15 }, { 15, 1,  7, 1, 15 },
       {  9, 9, 15, 1, 1 }, { 15, 8, 15, 1, 15 }, { 15, 8, 15, 9, 15 },
       { 15, 1,  1, 1, 1 }, { 15, 9, 15, 9, 15 }, { 15, 9, 15, 1, 15 } } ;
  int i, j ;
  for( i = 0 ; i < 4 ; i ++ )
   for( j = 0 ; j < 5 ; j ++ )
    if( ( digits[ c ][ j ] >> ( 3 - i ) ) & 1 )
     putpixel( x + i, y - j, 0xffffffff ^ getpixel( x + i, y - j ) ) ;
 }
 
void Number( void )
 {
  if( mem )
   {
    rccoord pos = GetTextPosition() ;
    pos.col = 7 ; SetTextPosition( pos ) ;
    cout << count + 1 ;
    pos.col = 20 ; SetTextPosition( pos ) ;
    cout << "0  " ;
    pos.col = 21 ; SetTextPosition( pos ) ;
    return ;
   }
  int i = count + 1 ;
  int c = 0 ;
  char m ;
  if( bits == 24 ) SetBank( ( MAXY + MAXY - 7 ) / lines_bank ) ;
  do
   {
    m = ( char ) ( i % 10 ) ;
    i = i / 10 ;
    c ++ ;
    digit( MAXX - 1 - c * 6, 7 - MAXY, m ) ;
   }
  while( i > 0 ) ;
 }

void CheckKbd( void ) 
 {
  int flag = 0 ;
  while( kbhit() )
   if( getch() == 27 ) flag = 1 ;
  if( flag )
   {
    TextMode() ;
    if( !quiet )
     {
      if( mem ) cout << endl ;
      cout << "Terminated by ESC pressing" << endl
           << "Shots written: " << count << endl ;
     }
    close( f ) ;
    if( count == 0 ) unlink( fname ) ;
    exit( 1 ) ;
   }
  return ;
 }

void PrintHelp( void )
 {
  cout << "The command line syntax is as follows:" << endl << endl
       << " RTR.EXE ( Render | Show )  [ Quiet ]  < File_Name >  [ Switches ]" << endl << endl
       << " - Command line is not case sensitive." << endl
       << " - You can use only the first letter of words \"Build\", \"Show\" or \"Quiet\"" << endl
       << "   instead of typing them wholy." << endl
       << " - One and only one of commands \"Render\" and \"Show\" must be specified." << endl
       << "   Type no command to see this help screen." << endl
       << " - Quite option suppresses the most of text information printed while working." << endl
       << " - File_Name is a name of file for writing the screen shots to or for showing." << endl 
       << " - Any one of the symbols '/', '\\', '+', '-' can be used as switch separator." << endl
       << " - Switches order and spacing are not important, switches can be also grouped" << endl
       << "   using only one switch separator, for example \"/mns\" instead of \"/m /s /n\"." << endl
       << " - Only one switch from each list can present, the first switch in the list" << endl
       << "   is selected by default." << endl << endl
       << "Press any key to continue ..." ;
  do getch() ; while( kbhit() ) ;
  text_pos = GetTextPosition() ;
  text_pos.col = 1 ;
  SetTextPosition( text_pos ) ;
  cout << "The RENDERING switches allowed are:" << endl
       << "  - a, v, s : To automatically detect video mode or select VGA 13h 320x200x256" << endl
       << "              or VESA 10Fh 320x200x16.8M mode for rendering." << endl
       << "              One of 'v' or 's' switches MUST be used if rendering to memory." << endl
       << "  - c, t, p : To select Cycling, Back-and-Forth or Ping-Pong viewing mode for" << endl
       << "              the film. This mode will be used while viewing the film later." << endl
       << "  - r, n    : To turn RLE archivation of screen shots on or off." << endl
       << "  - d, m    : To render screen shots for film to the screen or to the memory." << endl
       << "              If rendering to memory, 'v' or 's' switch must be used to select" << endl
       << "              the video mode the film will be rendered for." << endl
       << endl
       << "The SHOWING switches allowed are:" << endl
       << "  - c, t, p : To select viewing mode instead of one written in the file." << endl
       << "  - f, b    : To view film forward or backward." << endl ;
  exit( 0 ) ;
 }

int SwitchSymbol( char c )
 {
  return ( c == '/' || c == '\\' || c == '+' || c == '-' ) ;
 }

void Successful( void )
 {
  if( mem == 0 ) TextMode() ;
  if( !quiet )
   {
    if( mem ) cout << endl ;
    cout << "No errors detected" << endl ;
   }
  exit( 0 ) ;
 }
