/*
 *  Final Project - Daniel Zrawski
 *  CSCI 4229 - Summer 2015
 *
 *  Much reused code from Prof. Schreuder
 *
 *  Bicycle wheel with reflective rim sidewalls.
 *  
 *  Key bindings:
 *  l          Toggles lighting
 *  F1         Toggle smooth/flat shading
 *  m          Toggles movement
 *  []         Lower/rise light
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  a/d        Rotate wheel along x-axis
 *  w/s        Rotate wheel along y-axis
 *  < >        Move Camera along x-axis 
 *  Home,End   Move Camera along z-axis
 *  / '        move Camera along y-axis
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include "assembly.c"

int axes=1;       //  Display axes
int move=1;       //  Move light
int mode = 1;
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=30;   //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =   25;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   1;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   60;  // Specular intensity (%)
int shininess =   64;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   14;  // Elevation of light

//navigation variables
int x_pos = 0;
int y_pos = 0;
int z_pos = 0;

int    textures[9];   //  Sky textures

int spinTheta = 0;
int globalX = 0;
int globalY = 0;

double axel_length = 7;
double axel_r = 0.1875;
double sidewall_r1 = 18;
double sidewall_r2 = 19.5;
double rimbase_r1 = 17.5;
double rimbase_r2 = 18;
double rim_width= 1.5;
double hub_r = 0.5;
double hub_length = 4;



/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}



/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=8.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
      glTranslated(x_pos, y_pos, z_pos);
   }
   //  Orthogonal - set world orientation


   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        //float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.5);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);



  //Draw scene
  Wheel();
  Background(3.5*dim);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor4f(1,1,1,0.4);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");

      glWindowPos2i(5,25);
      Print("X Rotation=%d  Y Rotation=%d" ,globalX, globalY);

   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s",smooth?"Smooth":"Flat");


   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   spinTheta = fmod(45*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
      th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
      th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
      ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
      ph -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.2;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.2;
    //  Smooth color model
    else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;

    //  Toggle ball increment
    else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
    //  Flip sign
    else if (key == GLUT_KEY_F9)
      one = -one;

    
    else if (key == GLUT_KEY_HOME)
        z_pos++;
    else if (key == GLUT_KEY_END)
        z_pos--;
    

    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Update projection
    Project(mode?fov:0,asp,dim);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0') {
      th = ph = 0;
        x_pos = y_pos = z_pos = 0;
        globalX = globalY = spinTheta = 0;
    }
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.4;
   else if (ch==']')
      ylight += 0.4;

    
    else if (ch == ',')
        x_pos--;
    else if (ch == '.')
        x_pos++;

    else if (ch == '/')
        y_pos--;
    else if (ch == '\'')
        y_pos++;

   else if (ch == 'a')
      globalY = fmod(globalY - 1, 360);
    else if (ch == 'd')
       globalY = fmod(globalY + 1, 360);

  else if (ch == 'w')
    globalX = fmod(globalX + 1, 360);
  else if (ch == 's')
    globalX = fmod(globalX - 1, 360);


   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);
   glutInitWindowSize(600,600);
   glutCreateWindow("Daniel Zurawski - Final Project");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   textures[0] = LoadTexBMP("textures/brushed_metal.bmp");
   textures[1] = LoadTexBMP("textures/rubber.bmp");
   textures[2] = LoadTexBMP("textures/outside1.bmp");
   textures[3] = LoadTexBMP("textures/outside2.bmp");
   textures[4] = LoadTexBMP("textures/outside3.bmp");
   textures[5] = LoadTexBMP("textures/outside4.bmp");
   //textures[6] = LoadTexBMP("sky.bmp");
   //textures[7] = LoadTexBMP("grass.bmp");
   textures[8] = LoadTexBMP("textures/dark_metal.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
