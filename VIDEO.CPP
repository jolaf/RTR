#include <stdlib.h>
#include <string.h>
#include <i86.h>
#include <iostream.h>
#include "video.h"

rccoord text_pos ; 

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
