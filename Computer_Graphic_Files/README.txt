CPS511-Computer Graphic Final Project


The assignment has been running on Xcode 6 on Mac.
Therefore four header file have changed.
#include <iostream>             ->		#include <windows.h>
#include <OpenGL/gl.h>		->		#include <gl/gl.h>
#include <OpenGL/glu.h>		->		#include <gl/glu.h>
#include <GLUT/glut.h>		->		#include <gl/glut.h>



The program start with a controllable Tank, two non-controllable Enemy Tank, 15 buildings, and two streets.
When one of the Enemy Tank is hit by a bullet, the Enemy Tank change into red.
When the Enemy Tank is not hit, it is in grey. 



Functions are working well with function description:

1) LEFT BUTTON - (WORLD VIEW)Moving the mouse to slide the camera along this hemisphere.
		left/right motion of the mouse control the “azimuth”
		Up/down motion of the mouse control the “elevation” of the camera

		(FIRST PERSON VIEW of TANK)Moving the cannon and the camera at the same time
                 left/right motion of the mouse control cannon and the camera to left/right
                
2) ‘t’ key - First person View of One TANK
             move the camera on top of the tank

3) arrow up key - The tank move forward in First Person View and World View

4) arrow down key - The tank move backward in First Person View and World View

5) arrow left key - rotate only the body of the tank to left in First Person View and World View

6) arrow right key - rotate only the body of the tank to right in First Person View and World View

7) ‘q’ key - rotate only the cannon to left in the world view

8) ‘e’ key - rotate only the cannon to right in the world view

9) spacebar - shot a single bullet by a controllable Tank until it lands on the floor 
            



		
