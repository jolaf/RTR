#ifndef __ERROR_H__
#define __ERROR_H__

struct ERRMES
 {
  char* mes ;
  char  close_file ;
  char  text_mode ;
 } ;

enum errorcode
 {
  RTRER_THATSALLRIGHT ,
  RTRER_CANTWRITESHOT , 
  RTRER_ERRORSEEKING ,  
  RTRER_CANTSETHEADER , 
  RTRER_CANTCREATEFILE , 
  RTRER_CANTALLOCBUF , 
  RTRER_CANTWRITEHEADER ,
 } ;

extern ERRMES errmes[] ;

#endif
