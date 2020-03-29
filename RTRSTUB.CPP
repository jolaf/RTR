#include <iostream.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <sys\stat.h>
#include <io.h>

#define MAIN_NAME "RTR.EXE"
#define namelen 7
#define MAX_STUB_LENGTH 51200

enum errorcode
 {
  RTRER_EXFAILED = 1,
  RTRER_NODOS4GW,
  RTRER_CORRUPT,
  RTRER_BADSIZE,
  RTRER_NOMAIN,
  RTRER_CANTRUN,
  RTRER_DOS4GWERROR,
  RTRER_UNHANDLED
 } ;

static char* errmes[]=
 {
  "That's all right",
  "Execution failed",
  "Can't locate DOS/4GW Run-Time",
  "Main executable is corrupt",
  "Can't load executable format",
  "Can't locate main executable",
  "Can't run main executable",
  "DOS/4GW initialization error: "
 } ;


char* paths_to_check[] = { "DOS4GPATH", "PATH" } ;

inline char* Get_DOS4GW_Path( void ) throw( errorcode )
 {
  static char fullpath[ 80 ] ;
  if( access( "DOS4GW.EXE", F_OK | X_OK ) == 0 ) return "DOS4GW.EXE" ;
  if( access( "DOS4G.EXE" , F_OK | X_OK ) == 0 ) return "DOS4G.EXE" ;
  for( int i = 0 ; i < sizeof( paths_to_check ) / sizeof( paths_to_check[ 0 ] ) ; i ++ )
   {
    _searchenv( "DOS4GW.EXE", paths_to_check[ i ], fullpath ) ;
    if( fullpath[ 0 ] && ( access( fullpath, F_OK | X_OK ) == 0 ) ) return fullpath ;
    _searchenv( "DOS4G.EXE ", paths_to_check[ i ], fullpath ) ;
    if( fullpath[ 0 ] && ( access( fullpath, F_OK | X_OK ) == 0 ) ) return fullpath ;
   }
  throw RTRER_NODOS4GW ;
 }

int main( int argc, char* argv[] ) throw( errorcode ) 
 {
  char* av[ 4 ] ;
  char cmdline[ 128 ] ;
  int filelen = strlen( argv[ 0 ] ) ;
  struct stat buf ;
  
  cout << endl
       << "3D Objects Ray Tracing Renderer" << endl
       << "Copyright (c) 1996-2020 by VMZ SoftSystems, Inc." << endl
       << "Modernized historical version" << endl << endl;
  strupr( argv[ 0 ] ) ;
  try
   {
    if( argc < 1 ) throw RTRER_EXFAILED ;
    if( stat( argv[ 0 ], & buf ) ) throw RTRER_NOMAIN ;
    if( access( argv[ 0 ], F_OK | X_OK ) ) throw RTRER_CANTRUN ;
    if( filelen < namelen || strcmp( MAIN_NAME,
        &( argv[ 0 ][ filelen - namelen ] ) ) ) throw RTRER_CORRUPT ;
    if( buf.st_size < MAX_STUB_LENGTH ) throw RTRER_BADSIZE ;
    av[ 0 ] = Get_DOS4GW_Path() ;
    av[ 1 ] = argv[ 0 ] ;
    av[ 2 ] = getcmd( cmdline ) ;
    av[ 3 ] = NULL ;
    putenv( "DOS4G=QUIET" ) ;
    execvp( av[ 0 ], av ) ;
    throw RTRER_DOS4GWERROR ;
   }
  catch( errorcode n )
   {
    cout << "*** FATAL *** " << errmes[ n ] ;
    if( n == RTRER_DOS4GWERROR ) cout << strerror( errno ) ;
    return n ;
   }
  catch( ... )
   {
    cout << "*** FATAL *** Unhandled exception: report to VMZ." << endl ;
    return -1 ;
   }
 }
