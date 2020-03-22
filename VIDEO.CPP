#include <stdlib.h>
#include <string.h>
#include <i86.h>
#include <iostream.h>
#include "video.h"

#define TEXT_SIZE 8000
#define TEXT_SCREEN ( ( char*) 0xB8000 )
#define MY_DEAR_VESA_MODE 0x010F

rccoord text_pos ; 
char text_saved = 0 ;
char text_buf[ TEXT_SIZE ] ;
char* SCREEN = NULL ;
int SHOT_SIZE ;
int MAXX = 160 ;
int MAXY = 100 ;
int page_size = 0 ;
unsigned short bytes_line = 0 ;
int lines_bank = 0 ;
int banks_shot = 0 ;
int rem_lines = 0 ;
extern char quiet ;
extern char mem ;

struct RMI
 {
  long edi, esi, ebp, __, ebx, edx, ecx, eax ;
  short flags, es, ds, fs, gs, ip, cs, sp, ss ;
 } ;

rccoord GetTextPosition( void )
 {
  REGS regs ;
  rccoord r ;
  regs.h.ah = 0x03 ;
  regs.h.bh = 0x00 ;
  int386( 0x10, &regs, &regs ) ;
  r.row = ( char ) ( regs.h.dh + 1  ) ;
  r.col = ( char ) ( regs.h.dl + 1 ) ;
  return r ;
 }
 
void SetTextPosition( rccoord pos )
 {
  REGS regs ;
  regs.h.ah = 0x02 ;
  regs.h.bh = 0x00 ;
  regs.h.dh = ( char ) ( pos.row - 1 ) ;
  regs.h.dl = ( char ) ( pos.col - 1 ) ;
  int386( 0x10, &regs, &regs ) ;
 }

inline void SaveText( void )
 {
  REGS regs ;
  regs.h.ah = 0x0F ;
  int386( 0x10, &regs, &regs );
  if( ( regs.h.al & 0x7F ) == 3 && regs.h.bh == 0 && regs.h.ah == 80 )
   {
    memcpy( text_buf, TEXT_SCREEN, TEXT_SIZE ) ;
    text_pos = GetTextPosition() ;
    text_saved = 1 ;
   }
  else text_saved = 0 ;
 }

inline void SetTextMode( void )
 {
  REGS regs ;
  regs.w.ax = 0x0003 ;
  int386( 0x10, &regs, &regs ) ;
 }
 
short SetVGA256Mode( void )
 {
  REGS regs ;
  if( !quiet ) cout << "Attempting to setup VGA 13h 320x200x256 mode... " ;
  SaveText() ;
  regs.w.ax = 0x0013 ;
  int386( 0x10, &regs, &regs ) ;
  regs.h.ah = 0x0F ;
  int386( 0x10, &regs, &regs ) ;
  if( regs.h.al != 0x13 )
   {
    if( !quiet ) cout << "FAILED" << endl ;
    SCREEN = NULL ;
    return 1 ;
   }
  regs.w.ax = 0x1001 ;
  regs.h.bh = 0x00 ;
  int386( 0x10, &regs, &regs ) ;
  SCREEN = ( char* ) 0xA0000 ;
  SHOT_SIZE = 64000 ;
  return 0 ;
 }

short SetVESA168Mode( void )
 {
  REGS regs ;
  SREGS sregs ;
  RMI rmi;
  short selector ;
  short segment ;
  char far* buf ;
  if( !quiet ) cout << "Attempting to setup VESA 10Fh 320x200x16.8M mode... " ;
  SaveText() ;
  memset( &sregs, 0, sizeof( SREGS ) ) ;
  regs.w.ax = 0x0100 ;
  regs.w.bx = 0x0010 ;
  int386x( 0x31, &regs, &regs, &sregs ) ;
  segment = regs.w.ax ;
  selector = regs.w.dx ;
  buf = ( char far* ) MK_FP( selector, 0 ) ;
  memset( &rmi, 0, sizeof( RMI ) ) ;
  rmi.eax = 0x00004F01 ;
  rmi.ecx = MY_DEAR_VESA_MODE ;
  rmi.es = segment ;
  rmi.edi = 0x00000000 ;
  regs.w.ax = 0x0300 ;
  regs.w.bx = 0x0010 ;
  regs.w.cx = 0x0000 ;
  sregs.es = FP_SEG( &rmi ) ;
  regs.x.edi = FP_OFF( &rmi ) ;
  int386x( 0x31, &regs, &regs, &sregs ) ;
  if( buf[ 0 ] & 1 )
   {
    regs.w.ax = 0x4F02 ;
    regs.w.bx = MY_DEAR_VESA_MODE ;
    int386( 0x10, &regs, &regs ) ;
    if( regs.w.ax == 0x004F )
     {
      page_size = ( int ) ( buf[ 0x04 ] << 10 ) ;
      bytes_line = ( unsigned short ) ( ( ( ( int ) buf[ 0x11 ] ) << 8 ) + ( int ) buf[ 0x10 ] ) ;
      lines_bank = 0x10000 / bytes_line ;
      banks_shot = ( MAXY + MAXY ) / lines_bank ;
      rem_lines = ( MAXY + MAXY ) % lines_bank ;
      SCREEN = ( char* )( ( ( ( int ) buf[ 0x09 ] ) << 12 )
                        + ( ( ( int ) buf[ 0x08 ] ) << 4  ) ) ;
      SHOT_SIZE = 192000 ;
      return 0 ;
     }
   }
  page_size = 0 ;
  bytes_line = 0 ;
  lines_bank = 0 ;
  SCREEN = NULL ;
  if( !quiet ) cout << "FAILED" << endl ;
  return 1 ;
 }

void MapAllPalette( char* pal )
 {
  REGS regs ;
  SREGS sregs ;
  RMI rmi;
  short selector ;
  short segment ;
  memset( &sregs, 0, sizeof( SREGS ) ) ;
  regs.w.ax = 0x0100 ;
  regs.w.bx = 0x0030 ;
  int386x( 0x31, &regs, &regs, &sregs ) ;
  segment = regs.w.ax ;
  selector = regs.w.dx ;
  _fmemcpy( MK_FP( selector, 0 ), MK_FP( FP_SEG( pal ), FP_OFF( pal ) ), 0x300 ) ;
  memset( &rmi, 0, sizeof( RMI ) ) ;
  rmi.eax = 0x00001012 ;
  rmi.ebx = 0x00000000 ;
  rmi.ecx = 0x00000100 ;
  rmi.edx = 0x00000000 ;
  rmi.es  = segment ;
  regs.w.ax = 0x0300 ;
  regs.w.bx = 0x0010 ;
  regs.w.cx = 0x0000 ;
  sregs.es = FP_SEG( &rmi ) ;
  regs.x.edi = FP_OFF( &rmi ) ;
  int386x( 0x31, &regs, &regs, &sregs ) ;
 }

void SetBank( int n )
 {
  REGS regs ;
  regs.w.ax = 0x4F05 ;
  regs.w.bx = 0x0000 ;
  regs.w.dx = ( unsigned short ) ( ( n << 16 ) / page_size ) ;
  int386( 0x10, &regs, &regs );
 }

void TextMode( void )
 {
  if( mem ) return ;
  SetTextMode() ;
  if( text_saved )
   {
    memcpy( TEXT_SCREEN, text_buf, TEXT_SIZE ) ;
    SetTextPosition( text_pos ) ;
    if( !quiet ) cout << "OK" << endl ;
   }
 }
