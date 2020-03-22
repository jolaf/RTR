#ifndef __FACETS_H__
#define __FACETS_H__

#include "vector3d.h"
#include "color.h"

struct Facet
 {
  Vector n ;
  double k ;
  char flag ;
  char exist ;
  Facet( double _x = 0, double _y = 0, double _z = 0,
         double _k = 0, char _flag = 1 ) ;
  double Cross( Vector const& r, Vector const& d ) ;
 } ;

struct Wall : public Facet
 {
  Color color ;
  Wall( void ) ;
  Wall( double _x, double _y, double _z, double _k, Color& _c, char _flag = 1 ) ;
 } ;

class Tetraeder
 {
  public:
  Facet facets[ 4 ] ;
  Vector a ;
  double ax, ay, az ;
  Color color ;
  double l ;
  char flag ;
  Tetraeder( void ) ;
  Tetraeder( double _x, double _y, double _z,
             double _ax, double _ay, double _az,
             Color const& _c, char _flag = 1, double _l = 50.0 ) ;
  void TurnX( double fi, double y = 0, double z = 0 ) ;
  void TurnY( double fi, double x = 0, double z = 0 ) ;
  void TurnZ( double fi, double x = 0, double y = 0 ) ;
  void Move ( Vector b ) ;
  double Cross( Vector const& r, Vector const& d, char* fn = NULL ) ;
 } ;

#endif
