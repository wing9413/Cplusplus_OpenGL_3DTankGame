//
//  bullet.h
//  A3 new
//
//  Created by Jenny on 30/11/14.
//  Copyright (c) 2014 qw. All rights reserved.
//
GLfloat vertice[] = {4,1.25,0.25,   4,0.75,0.25,   4,0.75,-0.25,    4,1.25,-0.25,
                      5,1.25,0.25,   5,0.75,0.25,   5,0.75,-0.25,    5,1.25,-0.25 };



GLubyte quad[] = {0,1,2,    2,3,0 ,
                    4,5,6    ,6,7,4,
                   0,1,5,   5,4,0,
                   7,6,2,    2,3,7,
                 7,3,0,   0,4,7,
                   6,5,1,    1,2,6};       //top





typedef struct Bullet
{
    VECTOR3D center;
    VECTOR3D translation;		// Translatation Deltas
    VECTOR3D scaleFactor;		// X, Y, Z Scale Factors
    VECTOR3D angles;            // Rotation angles around x, y, z axis
    
    
    bool land;
    
    float rotate;
    
    
    
    // Material properties for drawing
    float mat_ambient[4];
    float mat_specular[4];
    float mat_diffuse[4];
    float mat_shininess[1];
    
    
}Bullet;


Bullet *createBullet ()
{
    Bullet *newBullet = (Bullet*)calloc(1, sizeof(Bullet));
    
    
    newBullet->translation.x = 0;
    newBullet->translation.y = -3;
    newBullet->translation.z = 0;
    
    
    newBullet->angles.x  = 0;
    newBullet->angles.z = 0;
    newBullet->angles.y =0;
    
    newBullet->scaleFactor.x = newBullet->scaleFactor.y  = newBullet->scaleFactor.z  =  0.5;
    
    newBullet->land = false;
    
    
    newBullet->mat_ambient[0] = 0.0;
    newBullet->mat_ambient[1] = 0.05;
    newBullet->mat_ambient[2] = 0.0;
    newBullet->mat_ambient[3] = 1.0;
    newBullet->mat_specular[0] = 0.0;
    newBullet->mat_specular[1] = 0.0;
    newBullet->mat_specular[2] = 0.004;
    newBullet->mat_specular[3] = 1.0;
    newBullet->mat_diffuse[0] = 0.5;
    newBullet->mat_diffuse[1] = 0.5;
    newBullet->mat_diffuse[2] = 0.5;
    newBullet->mat_diffuse[3] = 1.0;
    newBullet->mat_shininess[0] = 0;
    
    
    return newBullet;
    
    
}


void drawBullet(Bullet *bullet)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, bullet->mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bullet->mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bullet->mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, bullet->mat_shininess);
    
    
    glPushMatrix();
    
    glTranslatef(bullet->translation.x, bullet->translation.y, bullet->translation.z);
    glRotatef(bullet->angles.y, 0, 1, 0);
    glScalef(bullet->scaleFactor.x, bullet->scaleFactor.y, bullet->scaleFactor.z);
    
    // Activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertice);
    // Draw cube
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_BYTE, quad);
    // Deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
    glPopMatrix();
}



void bulletMove(Bullet *bullet)
{
    if (bullet->translation.y > -0.7 && bullet->land == false){
        
        
        bullet->translation.x += 0.2 * cos (bullet->angles.y * 3.1415 /180);
        bullet->translation.z -= 0.2 * sin (bullet->angles.y * 3.1415 /180);
        bullet->translation.y -= 0.04 ;
        
    }
    else
    {
        bullet -> land = true;
    }
}


