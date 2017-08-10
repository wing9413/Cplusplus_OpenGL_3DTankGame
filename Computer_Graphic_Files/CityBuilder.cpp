/*******************************************************************
 Interactive 3D City Modelling Program
 ********************************************************************/
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
#include "cube.h"
#include "TerrainGrid.h"

void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
VECTOR3D ScreenToWorld(int x, int y);


static int currentButton;
static unsigned char currentKey;

GLfloat light_position0[] = {12.0, 6.0,-12.0,1.0};
GLfloat light_position1[] = {-12.0, 6.0,12.0,1.0};
GLfloat light_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat light_ambient[]   = {0.2, 0.2, 0.2, 1.0};

//position of camera
float camX = -12.0;
float camY = 6.0;
float camZ = 12.0;

//direction of camera look at
float lookX = 0;
float lookY = 0;
float lookZ  =0;

//mouse controlled angel
float mouse_angle_UD = 0;
float mouse_angle_LR = 0;

//the mouse position difference for each move
float mouse_diffX;
float mouse_diffY;

// the previous mouse position
float mouse_preX;
float mouse_preY;

//cube's rotation
float cubeRote = 0;
float PI = 3.1415;

//when zoom in or out
float left = -1.0;
float right = 1.0;
float top = 1.0;
float bottom =-1.0;

// Cubes
// you may want to create a fixed size array of cubes or a fixed size array of pointers to cubes
// and othr variables to keep track of the number of current cubes etc.
// see cube.h createCube function
Cube *cubeList [20];        //create a cube list that store every single cube
int cubeNum= 0;             //a number that count the current cube
int maxNum = 0;             //a number that store the length of the cube list
int selectNum =0;           //a number that store the location of the selected cube before '+' is pressed



// Terrain Grid
TerrainGrid *terrainGrid = NULL;

int gridSize = 16;

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(750, 750);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("City Builder");
    
    initOpenGL(750,750);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);
    glutMainLoop();
    return 0;
}



// Setup openGL */
void initOpenGL(int w, int h)
{
    // Set up viewport, projection, then change to modelview matrix mode -
    // display function will then set up camera and modeling transforms
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(60.0,1.0,0.2,80.0);
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
    glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);
    glEnable(GL_NORMALIZE);
    
    // Set up Terrain Grid
    VECTOR3D origin = VECTOR3D(-8.0f,0.0f,8.0f);		// Terrain bounds: -8.0 to 8.0 in x and z direction
    terrainGrid = new TerrainGrid(gridSize, 16.0);	// size of terrain is therefore 16.0
    terrainGrid->InitGrid(gridSize, origin, 16.0, 16.0);
}



void display(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (currentKey != 'v')// display vehicle view when 'v' key is pressed
    {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top, 2, 60.0);
    }
    
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(mouse_angle_UD, 1.0, 0.0, 0.0);
    glRotatef(cubeRote, 0.0, 1.0, 0.0);
    // Set up fixed camera position and where it is looking, it's orientation
    gluLookAt(camX,camY,camZ,lookX,lookY,lookZ,0.0,1.0,0.0);
    glRotatef(mouse_angle_LR, 0.0, 1.0, 0.0);
    // Add code to draw all cubes currently in your array
    // see cube.h drawCube()
    for (int i=0; i<maxNum; i++)        //print out all the cubes that have been created
    {
        
        drawCube(cubeList[i]);
       
    }
    
    
    // Draw Terrain
    terrainGrid->DrawGrid(gridSize);
    
    glutSwapBuffers();
    
}


// Called at initialization and whenever user resizes the window */
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,1.0,0.2,40.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

VECTOR3D pos = VECTOR3D(0,0,0);

void mouse(int button, int state, int x, int y)
{
    currentButton = button;
    
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                VECTOR3D wpos;
                wpos = ScreenToWorld(x,y);
                wpos.y = 0.0;
                terrainGrid->UpdateGrid();
                terrainGrid->ComputeNormals();
            }
            else
            {
                mouse_preX = 0;
                mouse_preY = 0;
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
            {
                
            }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}


void mouseMotionHandler(int xMouse, int yMouse)
{
    if (currentButton == GLUT_LEFT_BUTTON)   //
    {
        VECTOR3D wpos;
        wpos = ScreenToWorld(xMouse, yMouse);
        if (wpos.x < -8.0) wpos.x = -8.0;
        if (wpos.x > 8.0) wpos.x = 8.0;
        if (wpos.z < -8.0) wpos.z = -8.0;
        if (wpos.z > 8.0) wpos.z = 8.0;
        wpos.y = 0.0;
        
        
        // move the camera
        if (mouse_preX !=0 && mouse_preY !=0)
        {
            mouse_diffX = xMouse - mouse_preX;
            mouse_diffY = yMouse - mouse_preY;
            mouse_angle_LR += mouse_diffX;
            if ((mouse_angle_UD < 8.7 || mouse_diffY < 0) && (mouse_angle_UD > -2.5 || mouse_diffY >0) ){
            mouse_angle_UD += mouse_diffY /10;
                camX += mouse_diffY/8;
                camY += mouse_diffY/8;
                camZ -=mouse_diffY/8;
            }


        }
    }
    
    if (currentButton == GLUT_RIGHT_BUTTON)
    {
        VECTOR3D wpos;
        wpos = ScreenToWorld(xMouse, yMouse);
        if (wpos.x < -8.0) wpos.x = -8.0;
        if (wpos.x > 8.0) wpos.x = 8.0;
        if (wpos.z < -8.0) wpos.z = -8.0;
        if (wpos.z > 8.0) wpos.z = 8.0;
        wpos.y=0.0;
        
        
        //Zoom in or Zoom out
        if (mouse_preX!=0 && mouse_preY!=0)
        {
            mouse_diffX= xMouse-mouse_preX;
            mouse_diffY=yMouse-mouse_preY;
            if (right > 0.375 || mouse_diffY >0 )
            {
                left -= mouse_diffY /8;
                right += mouse_diffY /8;
                bottom -= mouse_diffY /8;
                top += mouse_diffY /8;
            }
            
        }
    }
    mouse_preX = xMouse;
    mouse_preY = yMouse;
    glutPostRedisplay();
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
    //char buffer[];
    int i;
    
    switch (key)                        //when the letter on the keyboard is entered
    {
        case 't':                       //To translate
            currentKey='t';
            break;
        case 's':                       //To sacle
            currentKey='s';
            break;
        case 'r':                       //To rotate
            currentKey='r';
            break;
        case 'h':                       //To change the height
            currentKey='h';
            break;
        case 'c':                       //To change the selected cube
            currentKey='c';
            break;
        case 'm':
                for (i=0; i<maxNum; i++){
                    if (cubeList[i]->select == 0){
                        if (cubeList[i]->meshed == false && cubeList[i]->floor !=0)mesh(cubeList[i]);
                    }
                }
            break;
            
        case 'v':
            if (maxNum !=0)
            {
                currentKey = 'v';
                for (i=0; i<maxNum; i++){
                    if (cubeList[i]->select == 0){
                        camX = cubeList[i] ->tx;
                        camY = cubeList[i] ->ty + 1;
                        camZ = cubeList[i] ->tz;
                        lookX = camX;
                        lookY = camY;
                        lookZ = camZ + 1;
                    }
                }
                
            }
            break;
        case '+':                       //To multiple select
            currentKey='+';
            break;
        case '-':                       //to deselect all selected cubes and reset
            for (int j=0; j<maxNum-1; j++)
            {
                cubeList[j] -> select =1;   //change all cube to unslected
            }
            cubeList[maxNum-1] ->select =0; //change the last created cube into current selected cube
            selectNum = maxNum;
            cubeNum = maxNum;
            break;
            
    }
    glutPostRedisplay();
}

void functionKeys(int key, int x, int y)
{
    Cube *thing;
    if (key == GLUT_KEY_F1)
    {
        // Create and initialize new cube
        // add to your array see cube.h
        thing = createCube();           //new cube created
        if (cubeNum>0){
            cubeList[cubeNum-1] -> select = 1;}         //change the previous cube into unselected
        if (cubeNum == maxNum){
        cubeNum ++;
            selectNum ++;
            maxNum++;}
        else{
            maxNum++;
            selectNum = maxNum;
            cubeNum = maxNum;
        }
        cubeList[cubeNum-1] = thing;                    //store the newest cube into the cube list
        
    }
    
    int i;
     // Arrow keys
     switch(key)
     {
     case GLUT_KEY_DOWN:                                //when down arrow key is entered
             if (currentKey== 't'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                 cubeList[i]->tx -= 1.0f;               //cube translate downward
                     }
                 }}
             if (currentKey== 'h'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         if (cubeList[i]->ty > 1){
                         cubeList[i]->sfy -=0.5f;       //cube's height shorter
                         cubeList[i]->ty -=0.5f;
                             cubeList[i]->floor--;
                         }
                     }
                 }
                 }
             if (currentKey== 's'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         if (cubeList[i]->sfz > 0.5){
                         cubeList[i]->sfz -=0.5f;       //set scale
                         }
                     }
                 }
             }
             if (currentKey == 'v'){                    //vehicle view
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i] ->select ==0){
                         float coss = cos(cubeRote* PI /180) *0.2;
                         float sinn = sin(cubeRote* PI / 180) *0.2;
                         cubeList[i]->tz -= coss;
                         cubeList[i]->tx += sinn;
                         camX += sinn;
                         camZ -= coss;
                         lookX += sinn;
                         lookZ -=coss;
                         
                         
                     }
                         
                 }
             }
     break;
     case GLUT_KEY_UP:                                  //when up arrow key is entered
             if (currentKey== 't'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         cubeList[i]->tx += 1.0f;       //cube translate upward
                     }
                 }}
             if (currentKey== 'h'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         cubeList[i]->sfy += 0.5f;       //cube's height shorter
                         cubeList[i]->ty += 0.5f;
                         cubeList[i]->floor++;
                     }
                 }
                 }
             if (currentKey== 's'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         cubeList[i]->sfz +=0.5f;       //scale the mode
                     }
                 }
             }
             if (currentKey == 'v')                     //vehicle view
             {
                 for (i=0; i<maxNum; i++)
                 {
                     if (cubeList[i]->select == 0)
                     {
                         float coss = cos(cubeRote* PI /180) *0.2;
                         float sinn = sin(cubeRote* PI / 180) *0.2;
                         cubeList[i]->tx -= sinn;
                         cubeList[i]->tz += coss;
                         camX -= sinn;
                         camZ += coss;
                         lookX -= sinn;
                         lookZ +=coss;
                     }
                 }
             }
             
             
     break;
     case GLUT_KEY_RIGHT:                               //when right arraw key
             if (currentKey== 't'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         cubeList[i]->tz += 1.0f;       //cube translate to right
                     }
                 }
             }
             if (currentKey== 's'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         cubeList[i]->sfx +=0.5f;       //scale the mode
                     }
                 }
             }
             
             if (currentKey== 'r'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         cubeList[i]->angle +=1.0f;     //rotate the cube to right
                     }
                 }
             }
             if (currentKey== 'c')                      //change the current selected cube
             {
                 if (cubeNum<maxNum){
                     cubeList[cubeNum-1]->select = 1;
                     cubeNum++;
                     selectNum++;
                     cubeList[cubeNum-1]->select = 0;
                 }
             }
             if (currentKey== '+')                      //multiple slected
             {
                 if (cubeNum < maxNum && cubeNum >= selectNum){
                     cubeNum ++;
                     
                     cubeList[cubeNum-1]->select = 0;
                 }
                 
                 else if (cubeNum > 0 && cubeNum < maxNum){
                     cubeList[cubeNum-1]->select = 1;
                     cubeNum ++;
                     cubeList[cubeNum-1]->select = 0;
                 }
                 
             }
             
             if (currentKey == 'v')                 //vehicle view
             {
                 for (i=0; i<maxNum; i++)
                 {
                     if (cubeList[i]->select == 0)
                     {
                         cubeList[i] -> angle -= 2.0;
                         cubeRote += 2;
                     }
                 }
             }
     break;
     case GLUT_KEY_LEFT:
             if (currentKey== 't'){
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         cubeList[i]->tz += -1.0f;          //cube translate to left
                     }
                 }
             }
             if (currentKey== 's'){
                 for (i=0; i<maxNum; i++){                  //scale the sube
                     if (cubeList[i]->select == 0 && cubeList[i]->sfx > 0.5){
                         cubeList[i]->sfx -=0.5f;
                     }
                 }
             }
             if (currentKey== 'r'){                         //rotate the cube to left
                 for (i=0; i<maxNum; i++){
                     if (cubeList[i]->select == 0){
                         cubeList[i]->angle -=1.0f;
                     }
                 }
             }
             if (currentKey== 'c')                          //change the current slected cube
             {
                 if (cubeNum>1){
                     cubeList[cubeNum-1]->select = 1;
                     cubeNum--;
                     selectNum --;
                     cubeList[cubeNum-1]->select = 0;
                 }
             }
             
             if (currentKey == 'v')                         //vehicle view
             {
                 for (i=0; i< maxNum; i++)
                 {
                     if (cubeList[i] -> select == 0){
                         cubeList[i]-> angle += 2.0;
                         cubeRote -= 2.0;
                     }
                         
                 }
             }
             
             if (currentKey== '+')                          //multiple slected
             {
                 if (cubeNum > 1 && selectNum < cubeNum){
                     cubeList[cubeNum-1]->select = 1;
                     cubeNum --;
                     cubeList[cubeNum-1]->select = 0;
                 }
                 else
                 {
                     if (cubeNum >1){
                     cubeNum --;
                     cubeList[cubeNum-1]->select = 0;
                     }
                 }
             }
     break;
     default:
     break;
     }
    
    
    glutPostRedisplay();
}

