// Vertex positions of a standard size cube (width 2), centered at the origin
// of its own Model Coordinate System



typedef struct Cube
{
    VECTOR3D center;
    float tx, ty, tz;			// Translatation Deltas
    float sfx, sfy, sfz;		// Scale Factors
    float angle;				// Angle around y-axis of cube coordinate system
    int select;                 // is this cube is selected
    int floor;                  // count how many floors
    bool meshed;                // cube mesh
    
    GLfloat vertices[100][3];
    GLubyte quads[300];
    GLfloat quadNormals[100][3];
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
} Cube;

Cube *createCube()
{
    
    Cube *newCube = (Cube*)calloc(1, sizeof(Cube));
    newCube->meshed = false;
    
    newCube->select = 0;       //when the cube is selected.
    
    newCube->angle = 0.0;
    newCube->sfx = newCube->sfz = 0.5;
    newCube->sfy = 0.5;
    newCube->tx = 0.0;
    newCube->ty = 1.0;
    newCube->tz = 0.0;
    newCube -> floor =0;
    
    //vertices
    newCube ->vertices[0][0]=-1.0;
    newCube ->vertices[0][1]=-1.0;
    newCube ->vertices[0][2]=-1.0;
    
    newCube ->vertices[1][0]= 1.0;
    newCube ->vertices[1][1]=-1.0;
    newCube ->vertices[1][2]=-1.0;
    
    newCube ->vertices[2][0]=1.0;
    newCube ->vertices[2][1]=-1.0;
    newCube ->vertices[2][2]=1.0;
    newCube ->vertices[3][0]=-1.0;
    newCube ->vertices[3][1]=-1.0;
    newCube ->vertices[3][2]=1.0;
    newCube ->vertices[4][0]=-1.0;
    newCube ->vertices[4][1]=1.0;
    newCube ->vertices[4][2]=-1.0;
    newCube ->vertices[5][0]=1.0;
    newCube ->vertices[5][1]=1.0;
    newCube ->vertices[5][2]=-1.0;
    newCube ->vertices[6][0]=1.0;
    newCube ->vertices[6][1]=1.0;
    newCube ->vertices[6][2]=1.0;
    newCube ->vertices[7][0]=-1.0;
    newCube ->vertices[7][1]=1.0;
    newCube ->vertices[7][2]=1.0;
    
    //back
    newCube->quads[12]=0;
    newCube->quads[13]=4;
    newCube->quads[14]=5;
    newCube->quads[15]=1;
    //top
    newCube->quads[8]=4;
    newCube->quads[9]=7;
    newCube->quads[10]=6;
    newCube->quads[11]=5;
    //left
    newCube->quads[16]=7;
    newCube->quads[17]=4;
    newCube->quads[18]=0;
    newCube->quads[19]=3;
    //right
    newCube->quads[20]=1;
    newCube->quads[21]=5;
    newCube->quads[22]=6;
    newCube->quads[23]=2;
    //front
    newCube->quads[4]=6;
    newCube->quads[5]=7;
    newCube->quads[6]=3;
    newCube->quads[7]=2;
    //bottom
    newCube->quads[0]=0;
    newCube->quads[1]=1;
    newCube->quads[2]=2;
    newCube->quads[3]=3;



    //back
    newCube->quadNormals[3][0]=0;
    newCube->quadNormals[3][1]=0;
    newCube->quadNormals[3][2]=-1;
    //top
    newCube->quadNormals[2][0]=0;
    newCube->quadNormals[2][1]=1;
    newCube->quadNormals[2][2]=0;
    //left
    newCube->quadNormals[4][0]=-1;
    newCube->quadNormals[4][1]=0;
    newCube->quadNormals[4][2]=0;
    //right
    newCube->quadNormals[5][0]=1;
    newCube->quadNormals[5][1]=0;
    newCube->quadNormals[5][2]=0;
    //front
    newCube->quadNormals[1][0]=0;
    newCube->quadNormals[1][1]=0;
    newCube->quadNormals[1][2]=1;
    //bottom
    newCube->quadNormals[0][0]=0;
    newCube->quadNormals[0][1]=-1;
    newCube->quadNormals[0][2]=0;





    newCube->mat_ambient[0] = 0.0;
    newCube->mat_ambient[1] = 0.05;
    newCube->mat_ambient[2] = 0.0;
    newCube->mat_ambient[3] = 1.0;
    newCube->mat_specular[0] = 0.0;
    newCube->mat_specular[1] = 0.0;
    newCube->mat_specular[2] = 0.004;
    newCube->mat_specular[3] = 1.0;
    newCube->mat_diffuse[0] = 0.5;
    newCube->mat_diffuse[1] = 0.5;
    newCube->mat_diffuse[2] = 0.5;
    newCube->mat_diffuse[3] = 1.0;
    newCube->mat_shininess[0] = 0;
    
    newCube->highlightMat_ambient[0] = 0.0;
    newCube->highlightMat_ambient[1] = 0.00;
    newCube->highlightMat_ambient[2] = 0.0;
    newCube->highlightMat_ambient[3] = 1.0;
    newCube->highlightMat_specular[0] = 0.0;
    newCube->highlightMat_specular[1] = 0.0;
    newCube->highlightMat_specular[2] = 0.0;
    newCube->highlightMat_specular[3] = 1.0;
    newCube->highlightMat_diffuse[0] = 1.0;
    newCube->highlightMat_diffuse[1] = 0.0;
    newCube->highlightMat_diffuse[2] = 0.0;
    newCube->highlightMat_diffuse[3] = 1.0;
    newCube->highlightMat_shininess[0] = 0.0;
    return newCube;
}

void drawCube(Cube *cube)
{
    // Setup the material and lights used for the cube
    // you may want to use the alternate material if this cube is selected
    
    if (cube->select ==1)   //when the cube is not selected
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, cube->mat_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, cube->mat_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, cube->mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, cube->mat_shininess);
    }
    else                    //when the cube is selected
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, cube->highlightMat_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, cube->highlightMat_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, cube->highlightMat_diffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, cube->highlightMat_shininess);
    }
    
    
    // Transform Cube
    // you need to add code here
    glPushMatrix();
    glTranslatef(cube->tx, cube->ty, cube->tz);         //Translation
    glRotatef(cube->angle, 0,1 , 0);                    //rotate the cube on y-axis direction
    glScalef(cube->sfx,cube->sfy, cube->sfz);           //Scale
    
    int size = sizeof(cube->quadNormals)/sizeof(GLfloat);
    
    // Draw Cube using simple immediate mode rendering
    glBegin(GL_QUADS);
    int j=0;
    int i;
    for (i=0; i<size; i++)
    {
        glNormal3f(cube->quadNormals[i][0],cube->quadNormals[i][1],cube->quadNormals[i][2]);
        glVertex3f(cube->vertices[cube->quads[j]][0],cube->vertices[cube->quads[j]][1],cube->vertices[cube->quads[j]][2]);
        j++;
        glVertex3f(cube->vertices[cube->quads[j]][0],cube->vertices[cube->quads[j]][1],cube->vertices[cube->quads[j]][2]);
        j++;
        glVertex3f(cube->vertices[cube->quads[j]][0],cube->vertices[cube->quads[j]][1],cube->vertices[cube->quads[j]][2]);
        j++;
        glVertex3f(cube->vertices[cube->quads[j]][0],cube->vertices[cube->quads[j]][1],cube->vertices[cube->quads[j]][2]);
        j++;
    }
    
    glEnd();
    
    glPopMatrix();                                        //popMatrix
    
}



void mesh(Cube * cube)
{
    GLfloat baseVertex[4][3];
    int i, j;
    
    //copy the cube base vertex
    for (i=0; i<4; i++)
    {
        for (j=0; j<3; j++)
        {
            baseVertex[i][j]= cube->vertices[i][j];
        }
    }
    
    //store the quad number
    GLubyte face[4][4] = {{0,4,5,1},//back
                        {7,4,0,3},  //left
                        {1,5,6,2},  //right
                        {6,7,3,2}}; //front
    

    // store the quad normal number
    GLfloat faceNormal[4][4] = {{0,0,-1},  //back
                                {-1,0,0},   //left
                                {1,0,0},    //right
                                {0,0,1}};   //front
    
    int height =3;
    cube->sfy = 0.5;   // reset the cube size
    
    
    int count = 24;
    j=1;
    int m;
    
    
    
    // adding a cube for each floor
    for (i= 0; i< (cube->floor); i++)
    {
        cube->ty -=0.5;
        
        //adding four vertex
        for(m=0; m<4; m++)
        {
            cube->vertices[i*4+8+m][0] = baseVertex[m][0];
            cube->vertices[i*4+8+m][1] = height+i*2;
            cube->vertices[i*4+8+m][2] = baseVertex[m][2];
        }
        
        //adding four quad for the new vertex
        for (m=0; m<4; m++)
        {
            cube ->quads[count] = j*4 + face[m][0]; count++; cube ->quads[count] = j*4 + face[m][1]; count++;
            cube ->quads[count] = j*4 + face[m][2]; count++; cube ->quads[count] = j*4 + face[m][3]; count++;
        }
        
        
        //adding four quad normal for each quad
        for (m=0; m<4; m++)
        {
            cube ->quadNormals[i*4+6+m][0] = faceNormal[m][0];
            cube ->quadNormals[i*4+6+m][1] = height+i-1;
            cube ->quadNormals[i*4+6+m][2] = faceNormal[m][2];
        }


        
        j++;
    }
    
    
    cube ->meshed = true;   // the cube is already meshed
    
    
}
    
    
    
    
