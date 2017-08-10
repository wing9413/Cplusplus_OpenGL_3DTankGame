CPS511-Computer Graphic Assignment 2


The assignment has been running on Xcode 6 in Mac.
Therefore four header file have changed.
#include <iostream>             ->		#include <windows.h>
#include <OpenGL/gl.h>		->		#include <gl/gl.h>
#include <OpenGL/glu.h>		->		#include <gl/glu.h>
#include <GLUT/glut.h>		->		#include <gl/glut.h>





Functions are working well with function description:

1) LEFT BUTTON - Moving the mouse to slide the camera along this hemisphere.
		left/right motion of the mouse control the “azimuth”
		Up/down motion of the mouse control the “elevation” of the camera

2) RIGHT BUTTON - Zoom in or Zoom out
		up motion of the control is zooming in
		down motion of the control is zooming out

3) ‘m’ key - create an extruded quad mesh from cube scaled in height, 
	     and mesh only happen when height is added.

		