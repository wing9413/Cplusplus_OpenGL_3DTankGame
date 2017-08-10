// Vertex positions of a standard size cube (width 2), centered at the origin
// of its own Model Coordinate System
GLfloat vertices[] = {1,0.5,-1,   1,-1,-1,  -2,-1,-1,   -2,0.5,-1,  3,0.5,-1,      -3,0.5,-1,   -3,-0.5,-1,    -2,-0.5,-1,
                      1,0.5,1,     1,-1,1,   -2,-1,1,    -2,0.5,1,   3,0.5,1,      -3,0.5,1,    -3,-0.5,1,      -2,-0.5,1};
    


GLubyte quads[] = {0,1,2,  2,3,0,    0, 4, 1,    3,7,6,   6,5,3,   6,7,2,        //back
                       8,9,10, 10,11,8,   8, 12,9,   11,15,14,  14,13,11,    14,15,10,      //front
                       5,6,14,   14,13,5,    //left
                       4,1,9,   9,12,4,      //right
                       6,2,10,   10,14,6,    //extra
                       2,1,9,    9,10,2 ,       //bottom
                       5,4,12,   12,13,5 };       //top


GLfloat vertices2[] = {0.25,0.5,0.75,      0.25,0.5,-0.75,      0.25, 1.5,-0.75,     0.25,1.5,0.75,
                       -1.25,0.5,0.75,     -1.25,0.5,-0.75,      -1.25,1.5,-0.75,     -1.25,1.5,0.75,
                       4, 0.75,0.25,       4,1.25,0.25,        0.25,1.25,0.25,        0.25,0.75,0.25,
                        4, 0.75,-0.25,     4,1.25,-0.25,       0.25,1.25,-0.25,       0.25,0.75,-0.25};



GLubyte quads2[] = {0,1,2,   2,3,0,       //right face
                    4,5,6,    6,7,4,      //left face
                    5,4,0,    0,1,5,      //bottom face
                    2,6,7,     7,3,2,     //top face
                    7,4,0,   0,3,7,        //front face
                    6,5,1,    1,2,6,        //back face
                    8,9,10,    10,11,8,     //front   can
                    13,12,15,   15,14,13,   //back    can
                    8,9,13,     13,12,8,     //right  can
                    11,8,12,    12,15,11,     //bottom
                    10,9,13,    13,14,10,     //top
                    10,11,15,    15,14,10};





typedef struct Tank
{
	VECTOR3D center;
	float tx, ty, tz;			// Translatation Deltas
	float sfx, sfy, sfz;		// Scale Factors
	float angle;				// Angle around y-axis of cube coordinate system
    int select;                 // is this Tank is selected
    VECTOR3D translation;		// Translatation Deltas
    VECTOR3D scaleFactor;		// X, Y, Z Scale Factors
    VECTOR3D angles;            // Rotation angles around x, y, z axis

    
    bool hit;
    float rotate;
    
    
    VECTOR3D range1;
    VECTOR3D range2;
    
	
	// Material properties for drawing
	float mat_ambient[4];
    float mat_specular[4];
    float mat_diffuse[4];
	float mat_shininess[1];
    // Material properties if selected
	float highlightMat_ambient[4];
    float highlightMat_specular[4];
    float highlightMat_diffuse[4];
	float highlightMat_shininess[1];
} Tank;


typedef struct Cannon
{
    VECTOR3D center;
    VECTOR3D translation;		// Translatation Deltas
    VECTOR3D scaleFactor;		// X, Y, Z Scale Factors
    VECTOR3D angles;            // Rotation angles around x, y, z axis
    
    
    float rotate;
    
    VECTOR3D CannonTip;
    
    
}Cannon;



Tank *createTank()
{
    
  Tank *newTank = (Tank*)calloc(1, sizeof(Tank));
  
    
    newTank->select = 0;       //when the cube is selected.
    

    
    newTank->translation.x = 0;
    newTank->translation.y = 1;
    newTank->translation.z = 0;
    
    
    newTank->angles.x = newTank->angles.z = 0.0;
    newTank->angles.y =270;
    
    newTank->scaleFactor.x = newTank->scaleFactor.y  = newTank->scaleFactor.z  =  0.5;
    

    
    newTank->range1.x = 0 + 1 ;
    newTank->range2.x = 0 - 1 ;
    
    newTank->range1.y = 1+ 0.5 ;
    newTank->range2.y = 1 - 0.5 ;
    
    newTank->range1.z = 0 + 0.5 ;
    newTank->range2.z = 0 -0.5 ;
    
    newTank->hit = false;
    
    
    newTank->mat_ambient[0] = 0.0;
    newTank->mat_ambient[1] = 0.05;
    newTank->mat_ambient[2] = 0.0;
    newTank->mat_ambient[3] = 1.0;
    newTank->mat_specular[0] = 0.0;
    newTank->mat_specular[1] = 0.0;
    newTank->mat_specular[2] = 0.004;
    newTank->mat_specular[3] = 1.0;
    newTank->mat_diffuse[0] = 0.5;
    newTank->mat_diffuse[1] = 0.5;
    newTank->mat_diffuse[2] = 0.5;
    newTank->mat_diffuse[3] = 1.0;
    newTank->mat_shininess[0] = 0;
    
    newTank->highlightMat_ambient[0] = 0.0;
    newTank->highlightMat_ambient[1] = 0.00;
    newTank->highlightMat_ambient[2] = 0.0;
    newTank->highlightMat_ambient[3] = 1.0;
    newTank->highlightMat_specular[0] = 0.0;
    newTank->highlightMat_specular[1] = 0.0;
    newTank->highlightMat_specular[2] = 0.0;
    newTank->highlightMat_specular[3] = 1.0;
    newTank->highlightMat_diffuse[0] = 1.0;
    newTank->highlightMat_diffuse[1] = 0.0;
    newTank->highlightMat_diffuse[2] = 0.0;
    newTank->highlightMat_diffuse[3] = 1.0;
    newTank->highlightMat_shininess[0] = 0.0;
  return newTank;
}


Cannon *createCannon()
{
      Cannon *newCannon = (Cannon*)calloc(1, sizeof(Tank));
    
    
    newCannon->translation.x = 0;
    newCannon->translation.y = 1;
    newCannon->translation.z = 0;
    
    
    newCannon->angles.x  = newCannon->angles.z = 0.0;
    newCannon->angles.y =270;
    
    newCannon->scaleFactor.x = newCannon->scaleFactor.y  = newCannon->scaleFactor.z  =  0.5;
    
    
    newCannon->CannonTip.x = 0;
    newCannon->CannonTip.y = 1;
    newCannon->CannonTip.z = 0;
    
    return newCannon;
}






void drawTank(Tank *tank,    Cannon *cannon)
{
    
    if (tank->hit == true)
    {
    glMaterialfv(GL_FRONT, GL_AMBIENT, tank->highlightMat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, tank->highlightMat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, tank->highlightMat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS,tank->highlightMat_shininess);
    }
    else
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, tank->mat_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, tank->mat_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, tank->mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, tank->mat_shininess);
    }
    
    
    glPushMatrix();
    glTranslatef(tank->translation.x, tank->translation.y, tank->translation.z);
    glRotatef(tank->angles.y, 0, 1, 0);
    glScalef(tank->scaleFactor.x, tank->scaleFactor.y, tank->scaleFactor.z);
    
    
    // Activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    // Draw cube
    glDrawElements(GL_TRIANGLES, 80, GL_UNSIGNED_BYTE, quads);
    // Deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
    glPopMatrix();
    
    
    
    glPushMatrix();
    
    glTranslatef(cannon->translation.x, cannon->translation.y, cannon->translation.z);
    glRotatef(cannon->angles.y, 0, 1, 0);
    glScalef(cannon->scaleFactor.x, cannon->scaleFactor.y, cannon->scaleFactor.z);
    
    // Activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices2);
    // Draw cube
    glDrawElements(GL_TRIANGLES, 80, GL_UNSIGNED_BYTE, quads2);
    // Deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
    glPopMatrix();
  
}


void changeTankPosition(Tank *tank,Cannon *cannon ,float newX, float newY, float newZ)
{
    tank->translation.x = newX;
    tank->translation.y = newY;
    tank->translation.z = newZ;
    
    
    tank->range2.x =   tank->translation.x - 0.4;
    tank->range2.y =   tank->translation.y - 0.4;
    tank->range2.z =   tank->translation.z - 0.4;
    
    tank->range1.x =   tank->translation.x + 0.4;
    tank->range1.y =   tank->translation.y + 0.4;
    tank->range1.z =   tank->translation.z + 0.4;
    
    cannon->translation.x = newX;
    cannon->translation.y = newY;
    cannon->translation.z = newZ;
}



void checkifhit(Tank* tank, VECTOR3D bullet)
{
    
    if (tank->hit == false)
    {
    
    if((bullet.x >= tank->range2.x && bullet.x <= tank->range1.x)  &&   (bullet.z>=tank->range2.z && bullet.z<= tank->range1.z))
    {
        tank->hit = true;
        
    }
        
        
    }
    
    
}
	