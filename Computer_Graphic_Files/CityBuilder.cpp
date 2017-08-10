/*******************************************************************
	       City Modelling Program
********************************************************************/
//////////
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"

#include "TerrainGrid.h"
#include "Mesh.h"
#include "Tank.h"
#include "Bullet.h"


void initOpenGL();
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void timer(int value);
VECTOR3D ScreenToWorld(int x, int y);
void updateCameraPos();
void limitCameraAngle();

static int currentButton;
static unsigned char currentKey;
#define M_PI 3.14159265358979323846
const float FPS = 30.0;

// City Interaction State Variable
enum Action {TRANSLATE, ROTATE, SCALE, EXTRUDE, SELECT, MULTIPLESELECT, DESELECT_ALL, NAVIGATE, TANK, THIRD};
enum Action currentAction = TRANSLATE;

GLfloat light_position0[] = {12.0, 6.0,-12.0,1.0};
GLfloat light_position1[] = {-12.0, 6.0,12.0,1.0};
GLfloat light_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat light_ambient[]   = {0.2, 0.2, 0.2, 1.0};




//Tank
Tank* tank1;
Cannon* cannon1;

//Tank enemy
Tank* tankEN1;
Cannon* canEN1;
Tank* tankEN2;
Cannon* canEN2;

//Bullet
Bullet* bul;


 //mouse position difference
float mouse_diffX;
float mouse_diffY;

//when zoom in or out
float left = -1.0;
float right = 1.0;
float top = 1.0;
float bottom =-1.0;
 
 
// Building meshes
Mesh *buildings1;
Mesh *buildings2;
Mesh *buildings3;
Mesh *buildings4;
Mesh *buildings5;
Mesh *buildings6;
Mesh *buildings7;
Mesh *buildings8;
Mesh *buildings9;
Mesh *buildings10;
Mesh *buildings11;
Mesh *buildings12;
Mesh *buildings13;
Mesh *buildings14;
Mesh *buildings15;

int numBuildings = 0;
 
float buildingFloorHeight = 0.25;	// Building Floor Height for city building

// Street meshes
Mesh *streets[10];
Mesh *street2;
int numStreets = 0;

// Vehicle mesh
Mesh *vehicle;

// City terrain mesh
TerrainGrid *terrainGrid = NULL;
int gridSize = 16;


// Camera Control
VECTOR3D lookFrom;
VECTOR3D lookAt;
VECTOR3D up;



float radius = 12;			// Camera Distance
float lookFromx = 0;		// Camera X Position
float lookFromy = 0;	// Camera Y Position
float lookFromz = radius;		// Camera Z Position

float angleTheta = 0;		// Camera X angle
float anglePhi = 80;		// Camera Y angle

float upx = 0;			// Up Vector
float upy = 1;
float upz = 0;

float lookAtx = 0;		// Camera is looking at
float lookAty = 0;
float lookAtz = 0;

float camerax = 0;		// Camera X Position
float cameray = 0;	// Camera Y Position
float cameraz = radius;		// Camera Z Position


static float zoomFactor = 1.0; 

float xbefore;			// Previous X position of tank
float zbefore;			// Previous Y position of tank


GLint glutWindowWidth    = 750;
GLint glutWindowHeight   = 500;
GLint viewportWidth  = glutWindowWidth;
GLint viewportHeight = glutWindowHeight;

// Wolrd Boundaries
GLdouble worldLeftBase  =  -8.0;
GLdouble worldRightBase =  8.0;
GLdouble worldBottomBase=  -8.0;
GLdouble worldTopBase   =  8.0;

// World view boundaries
GLdouble wvLeftBase		=  worldLeftBase, 
         wvRightBase	=  worldRightBase,
		 wvBottomBase	=  worldBottomBase, 
		 wvTopBase		=  worldTopBase;



int main(int argc, char **argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(750, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("City Navigator");

  initOpenGL();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotionHandler);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(functionKeys);
  glutTimerFunc(1000.0 / FPS, timer, 0);
  glutMainLoop();
  return 0;
}


int width, height;

// Setup openGL */
void initOpenGL()
{
  glViewport(0, 0, (GLsizei) viewportWidth, (GLsizei) viewportHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0*zoomFactor,(float)viewportWidth/(float)viewportHeight,0.2,80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Set up and enable lighting
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.6, 0.6, 0.6, 0.0);  
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  //Nice perspective.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);
  
  // Set up Terrain Grid
  VECTOR3D origin = VECTOR3D(-16.0f,0.0f,16.0f);
  terrainGrid = new TerrainGrid(gridSize, 32.0);
  terrainGrid->InitGrid(gridSize, origin, 32.0, 32.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //updateCameraPos();
  gluLookAt(lookFromx, lookFromy, lookFromz,lookAtx, lookAty, lookAtz, upx, upy, upz);

  
  VECTOR3D scale;
  VECTOR3D trans;
  VECTOR3D angles;
  
  // Create a building
  int numFloors = 10;
  scale.x = scale.z = 0.5;
  scale.y = 1.0;
  trans.x = -6.0;
  trans.z = -6.0;
  trans.y = 0;

  buildings1 = createMesh(scale, trans, buildingFloorHeight, 10);
    trans.z += 2;
      buildings2 = createMesh(scale, trans, buildingFloorHeight, 6);
    trans.z += 3;
          buildings3 = createMesh(scale, trans, buildingFloorHeight, 15);
    trans.z += 2.5;
    buildings4 = createMesh(scale, trans, buildingFloorHeight, 8);
    trans.z += 2;
    buildings5 = createMesh(scale, trans, buildingFloorHeight,11);
    trans.z += 3;
    buildings6 = createMesh(scale, trans, buildingFloorHeight, 10);
    trans.z += 2;
    buildings7 = createMesh(scale, trans, buildingFloorHeight, 15);
    trans.z += 3;
    buildings8 = createMesh(scale, trans, buildingFloorHeight, 13);
    trans.z = -6-4;
    trans.x -= 1;
    buildings9 = createMesh(scale, trans, buildingFloorHeight, 10);
    trans.x += 3;
    buildings10 = createMesh(scale, trans, buildingFloorHeight, 9);
    trans.x += 2;
    buildings11 = createMesh(scale, trans, buildingFloorHeight, 18);
    trans.x += 3;
    buildings12 = createMesh(scale, trans, buildingFloorHeight, 13);
    trans.x += 2;
    buildings13 = createMesh(scale, trans, buildingFloorHeight, 12);
    trans.x += 3;
    buildings14 = createMesh(scale, trans, buildingFloorHeight, 9);
    trans.x += 2;
    buildings15 = createMesh(scale, trans, buildingFloorHeight, 15);
    
    
    numBuildings = 2;

  // Create a street
  scale.z = 0.1;
  scale.x = 0.7;
  scale.y = 1.5;
  trans.x = -4.0;
  trans.y = -0.08;
  trans.z = -8.0;
  

  streets[0] = createMesh(scale, trans, 1.0, 15);
  streets[0]->angles.x = 90.0;
  streets[0]->angles.y = 00.0;
  streets[0]->angles.z = 00.0;
    trans.x +=15;
    street2 = createMesh(scale, trans, 1.0, 14);
      street2->angles.x = 90.0;
    street2->angles.z = 90.0;
    
  numStreets = 1;

  // Create a vehicle - initially oriented along z axis direction
  scale.x = 0.125;
  scale.z = 0.25;
  scale.y = 1.0;
  trans.x = -4.0;
  trans.z = 6.0;
  trans.y = 0;

  vehicle = createMesh(scale, trans, 0.125, 2);
  vehicle->angles.x = vehicle->angles.y = vehicle->angles.z = 0.0; 
  vehicle->selected = true;
    
    
    
    
    // Create new Tank
    tank1 = createTank();

    
   //Create new Cannon
    cannon1 = createCannon();
    
    
    
    //Create new Tank Enemy
    tankEN1= createTank() ;
    tankEN2 =createTank();
    
    
    //Create new Cannon Enemy
    canEN1 = createCannon();
    canEN2 = createCannon();
    
    
    
    changeTankPosition(tankEN1, canEN1, -3, 1, 7);
    changeTankPosition(tankEN2, canEN2, 9, 1, -3);
    
    //Create bullet
    bul = createBullet();
    
    
    
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  
  glLoadIdentity();

  if (currentAction == TANK)
      
      ;
	  
  else
	updateCameraPos();
  
  gluLookAt(lookFromx, lookFromy, lookFromz, lookAtx, lookAty, lookAtz, upx, upy, upz);				
  
 
    

    // Draw Buildings
	  drawMesh(buildings1);
    drawMesh(buildings2);
    drawMesh(buildings3);
        drawMesh(buildings4);
        drawMesh(buildings5);
        drawMesh(buildings6);
        drawMesh(buildings7);
        drawMesh(buildings8);
        drawMesh(buildings9);
        drawMesh(buildings10);
        drawMesh(buildings11);
        drawMesh(buildings12);
        drawMesh(buildings13);
        drawMesh(buildings14);
        drawMesh(buildings15);
    

  
  // Draw Streets

	  drawMesh(streets[0]);
    drawMesh(street2);
   
    
    
    //draw Tank

    drawTank(tankEN2, canEN2);
        drawTank(tank1, cannon1);
        drawTank(tankEN1, canEN1);
    
    
    //draw bullet
    if (bul != NULL){
        checkifhit(tankEN1, bul->translation);
        checkifhit(tankEN2, bul->translation);
        
        if (tankEN1->hit == false  || tankEN2->hit == false)
    bulletMove(bul);
        else bul->land = true;
        
    drawBullet(bul);
    }
    
    


  terrainGrid->DrawGrid(gridSize);
  
  glutSwapBuffers();
}


// Called at initialization and whenever user resizes the window */
void reshape(int w, int h)
{
  glutWindowWidth = w; glutWindowHeight = h;

  viewportWidth  = glutWindowWidth;
  viewportHeight = glutWindowHeight;

  glViewport(0, 0, (GLsizei) viewportWidth, (GLsizei) viewportHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // keep same aspect ratio as screen window
  gluPerspective(60.0*zoomFactor,(float)viewportWidth/(float)viewportHeight,0.2,80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

VECTOR3D pos = VECTOR3D(0,0,0);

// Mouse Control coordinates
int prevx, prevy;

int selectedControlPoint = -1;

void mouse(int button, int state, int x, int y)
{
	currentButton = button;

    switch(button)
    {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN)
	  {
	    prevx = x;
	    prevy = y;

	  }
      else
      {
         if (currentAction == TANK)
         {
             prevx = prevy =0;
         }
             
      }
	  break;
            
            
  
    default:
	  break;
    }
  
  glutPostRedisplay();
}


/**************************************************************************
 * Mouse Control
 **************************************************************************/

void mouseMotionHandler(int xMouse, int yMouse)
{
    
    if (currentAction == TANK)
    {
        VECTOR3D wpos;
        wpos = ScreenToWorld(xMouse, yMouse);
        if (wpos.x < -8.0) wpos.x = -8.0;
        if (wpos.x > 8.0) wpos.x = 8.0;
        if (wpos.z < -8.0) wpos.z = -8.0;
        if (wpos.z > 8.0) wpos.z = 8.0;
        wpos.y = 0.0;
        
        
        
        if (prevx !=0 & prevy !=0)
        {
        mouse_diffX = xMouse -prevx;
        mouse_diffY = yMouse -prevy;
        cannon1 ->angles.y -= mouse_diffX/6;
            
            double ztmp , xnew,znew;
            ztmp = vehicle->scaleFactor.z+0.1;
            xnew = ztmp * sin ((cannon1->angles.y -90)/180* M_PI);
            znew = ztmp * cos ((cannon1->angles.y -90)/180* M_PI);
            

            
            
            lookAtx = cannon1->translation.x - 2*xnew;;
            lookAtz = cannon1->translation.z - 2*znew;
        }
        
    }
    
    
    
	if (currentButton == GLUT_LEFT_BUTTON)
    {
	    VECTOR3D wpos;
	  
	    angleTheta += prevx - xMouse;
	    prevx = xMouse;

	    anglePhi += prevy - yMouse;
	    prevy = yMouse;

	    while (angleTheta < -180)
		  angleTheta += 360;
	    while (angleTheta > 180)
		  angleTheta -= 360;
	    while (anglePhi < 0)
		  anglePhi += 360;

	    limitCameraAngle();
	  
    }
    

    
    //glutPostRedisplay();
	return;
}

/**************************************************************************
 * Timer function to limit Frames Per Second
 **************************************************************************/
void timer(int value)
{
	glutTimerFunc(1000.0 / FPS, timer, 0);
	glutPostRedisplay();
}


/**************************************************************************
 * Utility Functions
 **************************************************************************/
float degToRad(float degrees)
{
	return degrees / 180 * M_PI; 
}

float radToDeg(float radians)
{
	return radians * 180 / M_PI;
}

void updateCameraPos()
{
	// Spherical coordinates formula
	lookFromx = lookAtx + radius * sin(anglePhi*0.0174532) * sin(angleTheta*0.0174532); 
	lookFromy = lookAty + radius * cos(anglePhi*0.0174532);
	lookFromz = lookAtz + radius * sin(anglePhi*0.0174532) * cos(angleTheta*0.0174532); 
}


/**************************************************************************
 * Limit Camera angle
 **************************************************************************/
void limitCameraAngle()
{
	if (anglePhi > 80)
		anglePhi = 80;
	if (anglePhi < 30)
		anglePhi = 30;
	//if (angleTheta < 10)
		//angleTheta = 10;
}


VECTOR3D ScreenToWorld(int x, int y)
{

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;
	
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	// Read all pixels at given screen XY from the Depth Buffer
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	return VECTOR3D((float)posX, (float)posY, (float)posZ);
}// ScreenToWorld()


/* Handles input from the keyboard, non-arrow keys */
void keyboard(unsigned char key, int x, int y)
{
  double xtmp, ztmp, xnew, znew; 
  switch (key) 
  {
  // Navigate
  case 'n':
	  if (currentAction != NAVIGATE)
	  {
      
		  currentAction = NAVIGATE;
		  camerax = lookFromx;
		  cameray = lookFromy;
		  cameraz = lookFromz;
		  ztmp = vehicle->scaleFactor.z+0.1;
          xnew = ztmp * sin (degToRad(vehicle->angles.y));
          znew = ztmp * cos (degToRad(vehicle->angles.y));
	      lookFromx = vehicle->translation.x;
	      lookFromy = 2*buildingFloorHeight;
	      lookFromz = vehicle->translation.z;
	      lookAtx = vehicle->translation.x - 2*xnew;
	      lookAty = buildingFloorHeight;
	      lookAtz = vehicle->translation.z - 2*znew;
	  }
	  else
	  {
		  currentAction = TRANSLATE;
		  lookFromx = camerax;
		  lookFromy = cameray;
		  lookFromz = cameraz;
	  }
	  break;
          
          
  case 't':
          
          if (currentAction != TANK)
          {
              
              currentAction = TANK;
              camerax = lookFromx;
              cameray = lookFromy;
              cameraz = lookFromz;
              
              ztmp = cannon1->scaleFactor.z+0.1;
              xnew = ztmp * sin (degToRad(cannon1->angles.y -90));
              znew = ztmp * cos (degToRad(cannon1->angles.y -90));
              
              lookFromx = cannon1->translation.x;
              lookFromy = cannon1->translation.y+1;
              lookFromz = cannon1->translation.z;
              
              
              lookAtx = cannon1->translation.x - 2*xnew;;
              lookAty = lookFromy;
              lookAtz = cannon1->translation.z - 2*znew;
          }
          else
          {
              currentAction = TRANSLATE;
              lookFromx = camerax;
              lookFromy = cameray;
              lookFromz = cameraz;
          }
          break;
          
          
          
          
    case 'q':
          if (currentAction !=TANK)
          {
              cannon1->angles.y +=2.0;
              
          }
              break;
          
          
    case 'e':
          if (currentAction != TANK)
          {
              cannon1->angles.y -=2.0;
          }
          break;
          
    case ' ':
          if (bul->land == true){
          bul->translation =  cannon1->CannonTip;
          bul->angles = cannon1->angles;
          bul->land = false;
          }
          
          
          break;
          
          
    
  
  }
  glutPostRedisplay();
    
}

void functionKeys(int key, int x, int y)
{
    
    
  double xtmp, ztmp, xnew, znew;
    switch (key)
    {
        case GLUT_KEY_DOWN:
            tank1->translation.x += 0.2 * sin (tank1->rotate * 3.1415 /180);
            tank1->translation.z -= 0.2 * cos (tank1->rotate * 3.1415 /180);
            
            cannon1->translation = cannon1->CannonTip = tank1->translation;
            
            
            
            if (currentAction == TANK)
            {
                lookFromz -= 0.2 * cos (tank1->rotate * 3.1415 /180);
                lookFromx += 0.2 * sin (tank1->rotate * 3.1415 /180);
                lookAtz -= 0.2 * cos (tank1->rotate * 3.1415 /180);
                lookAtx += 0.2 * sin (tank1->rotate * 3.1415 /180);
            }
            

            break;
        case GLUT_KEY_UP:
            tank1->translation.x -= 0.2 * sin (tank1->rotate * 3.1415 /180);
            tank1->translation.z += 0.2 * cos (tank1->rotate * 3.1415 /180);
            
            cannon1->translation =  cannon1->CannonTip=  tank1 ->translation;
            

            if (currentAction == TANK)
            {
                lookFromz += 0.2 * cos (tank1->rotate * 3.1415 /180);
                lookFromx -= 0.2 * sin (tank1->rotate * 3.1415 /180);
                lookAtz += 0.2 * cos (tank1->rotate * 3.1415 /180);
                lookAtx -= 0.2 * sin (tank1->rotate * 3.1415 /180);
            }
            
   
            break;
        case GLUT_KEY_RIGHT:
            tank1->angles.y -= 2.0;
            tank1->rotate +=2.0;
            
            

            
            
            break;    		  
            
        case GLUT_KEY_LEFT:
            tank1->angles.y += 2.0;
            tank1->rotate -=2.0;
            
            
            
            break;
    }
    
    


    
    
}

