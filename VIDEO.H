#ifndef __VIDEO_H__
#define __VIDEO_H__

struct rccoord
 {
  char row, col ;
 } ;

short SetVGA256Mode( void ) ;
short SetVESA168Mode( void ) ;
void MapAllPalette( char* pal ) ;
void SetBank( int n ) ;
void TextMode( void ) ;
rccoord GetTextPosition( void ) ;
void SetTextPosition( rccoord pos ) ;

#endif
