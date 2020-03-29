#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream.h>
#include <io.h>
#include "video.h"

extern int f ;
extern char* fname ;
extern long& count ;

void Number( void )
 {
  rccoord pos = GetTextPosition() ;
  pos.col = 7 ; SetTextPosition( pos ) ;
  cout << count + 1 ;
  pos.col = 20 ; SetTextPosition( pos ) ;
  cout << "0   " ;
  pos.col = 21 ; SetTextPosition( pos ) ;
 }

void CheckKbd( void ) 
 {
  int flag = 0 ;
  while( kbhit() )
   if( getch() == 27 ) flag = 1 ;
  if( flag )
   {
    cout << endl << endl ;
    cout << "Terminated by ESC pressing" << endl
         << "Shots written: " << count << endl ;
    close( f ) ;
    if( count == 0 ) unlink( fname ) ;
    exit( 1 ) ;
   }
  return ;
 }

void PrintHelp( void )
 {
  cout << "The command line syntax is as follows:" << endl << endl
       << " RTR.EXE < File_Name >" << endl << endl
       << " - File_Name is a name of file for writing the screen shots." << endl ;
  exit( 0 ) ;
 }

void Successful( void )
 {
  cout << "No errors detected" << endl ;
  exit( 0 ) ;
 }
