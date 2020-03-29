#ifndef __VIDEO_H__
#define __VIDEO_H__

struct rccoord
 {
  char row, col ;
 } ;

rccoord GetTextPosition( void ) ;
void SetTextPosition( rccoord pos ) ;

#endif
