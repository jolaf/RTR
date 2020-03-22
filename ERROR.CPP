#include "error.h"
 
ERRMES errmes[] =
 {
  "That's all right", 0, 0,
  "Can't write palette to the file", 1, 1,
  "Can't write screen shot to the file", 1, 1,
  "Error seeking the target file", 1, 1,
  "Can't write the file header", 1, 1,
  "Rendering to memory requires direct video mode selection", 0, 0,
  "Can't create the target file", 0, 0,
  "Not enough memory for rendering buffer", 1, 0,
  "Can't setup neither VGA nor VESA video mode", 1, 0,
  "Can't setup the VGA video mode", 1, 0,
  "Can't setup the VESA video mode", 1, 0,
  "Not enough memory for archivation", 1, 1,
  "Can't write the file header", 1, 0,
  "Can't read the source file", 1, 1,
  "Can't open the source file", 0, 0,
  "Not enough memory for showing buffer", 1, 1,
  "Can't read the source file header", 1, 0,
  "The file doesn't include any screen shots", 1, 0,
  "Can't read palette from the source file", 1, 0,
  "Not enough memory for screen shots table", 1, 0,
 } ;
