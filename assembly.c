
//Note: Most units in inches (kinda)

extern int textures[9];
extern double dim;
extern float shinyvec[1];
extern int spinTheta;
extern int globalX;
extern int globalY;

extern double axel_length;
extern double axel_r;
extern double sidewall_r1;
extern double sidewall_r2; 
extern double rimbase_r1; 
extern double rimbase_r2; 
extern double rim_width;
extern double hub_r;
extern double hub_length;

#include "math.h"


//Material property code from OpenGl Primer
typedef struct materialStruct{
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess[1];
} materialStruct;

materialStruct silverShiny = {
  {0.9, 0.9, 0.9, 0.9},
  {0.9, 0.9, 0.9, 0.9},
  {0.9, 0.9, 0.9, 0.9},
  {100.0}
};

void rim_sidewall(double x,double y,double z,
                 double r1, double r2,                 
                int spinTheta)
{
   glPushMatrix();
   glRotated(spinTheta,0,0,1);
   glTranslated(x,y,z);
   glScaled(1,1,1);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[6]);

   //sidewall  
   int theta;
   glBegin(GL_QUAD_STRIP);
   for (theta=0; theta<=360; theta+=8) {
      glTexCoord2f(0,0);
      glVertex3d(r1*Cos(theta), r1*Sin(theta), 0);
      glTexCoord2f(0,0.1);
      glVertex3d(r2*Cos(theta), r2*Sin(theta), 0);
   }
   glEnd();

   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void horiz_cylinder(double x,double y,double z,
                 double r, double dz,                 
                  int spinTheta)
{

    //  Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    glRotated(spinTheta,0,0,1);
    glScaled(r,r,dz);
    glTranslated(x,y,z);


    //TEXTURES
    glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);

    // RoundSide
    glBegin(GL_QUAD_STRIP);

    int theta;
    for(theta=0; theta<=360; theta+=8) {
        glNormal3f(Cos(theta), Sin(theta),0);       
       
        glTexCoord2f(0, Cos(theta));
        glVertex3d(Cos(theta), Sin(theta),-1);

        glTexCoord2f(1, Cos(theta));
        glVertex3d(Cos(theta), Sin(theta),1);
    }
    glEnd();
    
    //left cap
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,-1);
    glTexCoord2f(0.5,0.5);
    glVertex3f(0,0,0);
     for(theta=0; theta<=360; theta+=8) {
        glTexCoord2f(0.5*Sin(theta)+0.5, 0.5*Cos(theta)+0.5);
        glVertex3d(Cos(theta), Sin(theta),-1);
    }
    glEnd();

    //right cap
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(1,0,0);
    glTexCoord2f(0.5,0.5);
    glVertex3f(0,0,1);
     for(theta=0; theta<=360; theta+=8) {
        glTexCoord2f(0.5*Sin(theta)+0.5, 0.5*Cos(theta)+0.5);
        glVertex3d(Cos(theta), Sin(theta),1);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/* Draws the panoramic image as background*/
static void Background(double D)
{

   glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,textures[2]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,1);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);
   glEnd();


   glBindTexture(GL_TEXTURE_2D,textures[3]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0,1); glVertex3f(+D,+D,-D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,textures[4]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(1,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0,1); glVertex3f(+D,+D,+D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,textures[5]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0,1); glVertex3f(-D,+D,+D);
   glEnd();

   //Top and bottom. Commented out because it looks bad. Need to modify images
   /*   
   glBindTexture(GL_TEXTURE_2D,textures[6]);
   glBegin(GL_QUADS);    
   glTexCoord2f(0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(1,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0,1); glVertex3f(-D,+D,-D);
   glEnd();
    

   glBindTexture(GL_TEXTURE_2D,textures[7]);
   glBegin(GL_QUADS);    
   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();
   */  

   glDisable(GL_TEXTURE_2D);
}


void sidewalls_reflective(double x,double y,double z,
                 double r1, double r2) 
{

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  //much code taken from http://www.videotutorialsrock.com/opengl_tutorial/reflections/text.php

  //Note: Draw normal background at end to prevent issues when rotating mirror

  //This is what is shown on the mirror  
  glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
  glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
  glDisable(GL_DEPTH_TEST); //Disable depth testing
  glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
  //Make pixels in the stencil buffer be set to 1 when the stencil test passes
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  //Set all of the pixels covered by the floor to be 1 in the stencil buffer   

  rim_sidewall(x,y,z, r1,r2, spinTheta);

  glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
  glEnable(GL_DEPTH_TEST); //Enable depth testing
  //Make the stencil test pass only when the pixel is 1 in the stencil buffer
  glStencilFunc(GL_EQUAL, 1, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change

  //Draw the cube, reflected vertically, at all pixels where the stencil
  //buffer is 1

  //Modify image drawn on mirror
  glPushMatrix();
  glScalef(-1, 1, 1);
  Background(3.5*dim);
  glPopMatrix();

  glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer

  //TRANSPARENCY - BLEND THE MIRROR
  glEnable(GL_BLEND);
  glColor4f(1, 1, 1, 0.1);

  rim_sidewall(x,y,z, r1,r2, spinTheta);
  glDisable(GL_BLEND);   
}


void rim_base(double x,double y,double z,
                 double r1, double r2, double width,         
                 int spinTheta)
{

  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,silverShiny.shininess);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,silverShiny.specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,silverShiny.diffuse);


  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(spinTheta,0,0,1);
  glScaled(1,1,1);



  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,textures[0]);


  glColor3f(1,1,1);
  glBegin(GL_QUAD_STRIP);
  int theta;
  for (theta=0; theta<=360; theta+=8) {
    glNormal3f(-Cos(theta), -Sin(theta), 0.7);
    glTexCoord2f(0, 0);
    glVertex3d(r1*Cos(theta), r1*Sin(theta), 0.125*width);
    glTexCoord2f(0, 0.1);
    glVertex3d(r2*Cos(theta), r2*Sin(theta), 0.5*width);
  }
  glEnd();

  glColor3f(1,1,1);
  glBegin(GL_QUAD_STRIP);
  for (theta=0; theta<=360; theta+=8) {
    glNormal3f(-Cos(theta), -Sin(theta),-0.7);
    glTexCoord2f(0, 0);
    glVertex3d(r1*Cos(theta), r1*Sin(theta), -0.125*width);
    glTexCoord2f(0, 0.1);
    glVertex3d(r2*Cos(theta), r2*Sin(theta), -0.5*width);
  }
  glEnd();

  glColor3f(1,1,1);
  glBegin(GL_QUAD_STRIP);
  for (theta=0; theta<=360; theta+=8) {
    glNormal3f(-Cos(theta),0,0);
    glTexCoord2f(0, 0);
    glVertex3d(r1*Cos(theta), r1*Sin(theta), -0.125*width);
    glTexCoord2f(0, 0.1);
    glVertex3d(r1*Cos(theta), r1*Sin(theta),  0.125*width);
  }
  glEnd();

  glPopMatrix();
}


static void Torus(float x,float y,float z , float th,float ph , float S,float r, int spinTheta)
{
   int i,j;   // Counters

  int n = 8;

   int N=4*n; // Number of slices

   //  Transform
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
  glRotated(spinTheta,0,0,1);
   glScaled(S,S,S);
   glColor3f(1,1,1);


      glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,textures[1]);

   //  Loop along ring
   glBegin(GL_QUADS);
   for (i=0;i<N;i++)
   {
      float th0 =  i   *360.0/N;
      float th1 = (i+1)*360.0/N;
      //  Loop around ring
      for (j=0;j<N;j++)
      {
         float ph0 =  j   *360.0/N;
         float ph1 = (j+1)*360.0/N;
         glNormal3d(Cos(th1)*Cos(ph0),-Sin(th1)*Cos(ph0),Sin(ph0)); glTexCoord2d(0.5*th1/30.0,0.5*ph0/180.0); glVertex3d(Cos(th1)*(1+r*Cos(ph0)),-Sin(th1)*(1+r*Cos(ph0)),r*Sin(ph0));
         glNormal3d(Cos(th0)*Cos(ph0),-Sin(th0)*Cos(ph0),Sin(ph0)); glTexCoord2d(0.5*th0/30.0,0.5*ph0/180.0); glVertex3d(Cos(th0)*(1+r*Cos(ph0)),-Sin(th0)*(1+r*Cos(ph0)),r*Sin(ph0));
         glNormal3d(Cos(th0)*Cos(ph1),-Sin(th0)*Cos(ph1),Sin(ph1)); glTexCoord2d(0.5*th0/30.0,0.5*ph1/180.0); glVertex3d(Cos(th0)*(1+r*Cos(ph1)),-Sin(th0)*(1+r*Cos(ph1)),r*Sin(ph1));
         glNormal3d(Cos(th1)*Cos(ph1),-Sin(th1)*Cos(ph1),Sin(ph1)); glTexCoord2d(0.5*th1/30.0,0.5*ph1/180.0); glVertex3d(Cos(th1)*(1+r*Cos(ph1)),-Sin(th1)*(1+r*Cos(ph1)),r*Sin(ph1));
      }
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);

   //  Restore
   glPopMatrix();
}

/* Only draws 90 degrees of torus */
static void quarter_torus(float x,float y,float z , float th,float ph , float S,float r, int spinTheta)
{
   int i,j;   // Counters

  int n = 8;

   int N=4*n; // Number of slices

   //  Transform
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
    glRotated(spinTheta,0,0,1);
    glScaled(S,S,S);
    glColor3f(1,1,1);


    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,textures[0]); //brushed metal texture

   //  Loop along ring
   glBegin(GL_QUADS);
   for (i=0;i<N;i++)
   {
      float th0 =  i   *90.0/N;
      float th1 = (i+1)*90.0/N;
      //  Loop around ring
      for (j=0;j<N;j++)
      {
         float ph0 =  j   *360.0/N;
         float ph1 = (j+1)*360.0/N;
         glNormal3d(Cos(th1)*Cos(ph0),-Sin(th1)*Cos(ph0),Sin(ph0)); glTexCoord2d(0.5*th1/30.0,0.5*ph0/180.0); glVertex3d(Cos(th1)*(1+r*Cos(ph0)),-Sin(th1)*(1+r*Cos(ph0)),r*Sin(ph0));
         glNormal3d(Cos(th0)*Cos(ph0),-Sin(th0)*Cos(ph0),Sin(ph0)); glTexCoord2d(0.5*th0/30.0,0.5*ph0/180.0); glVertex3d(Cos(th0)*(1+r*Cos(ph0)),-Sin(th0)*(1+r*Cos(ph0)),r*Sin(ph0));
         glNormal3d(Cos(th0)*Cos(ph1),-Sin(th0)*Cos(ph1),Sin(ph1)); glTexCoord2d(0.5*th0/30.0,0.5*ph1/180.0); glVertex3d(Cos(th0)*(1+r*Cos(ph1)),-Sin(th0)*(1+r*Cos(ph1)),r*Sin(ph1));
         glNormal3d(Cos(th1)*Cos(ph1),-Sin(th1)*Cos(ph1),Sin(ph1)); glTexCoord2d(0.5*th1/30.0,0.5*ph1/180.0); glVertex3d(Cos(th1)*(1+r*Cos(ph1)),-Sin(th1)*(1+r*Cos(ph1)),r*Sin(ph1));
      }
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);

   //  Restore
   glPopMatrix();
}

static void hub_cyl(double x,double y,double z,
                 double r, double width,                 
                  int spinTheta)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(spinTheta,0,0,1);
  glColor3f(1,1,1);

  // RoundSide
  glBegin(GL_QUAD_STRIP);
  int theta;
  for(theta=0; theta<=360; theta+=8) {
    glNormal3f(Cos(theta), Sin(theta),0);       
   
    glTexCoord2f(0, Cos(theta));
    glVertex3d(r*Cos(theta), r*Sin(theta),-width/2.0);

    glTexCoord2f(1, Cos(theta));
    glVertex3d(r*Cos(theta), r*Sin(theta), width/2.0);
  }
  glEnd();
  
  //left cap
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,0,-1);
  glTexCoord2f(0.5,0.5);
  glVertex3f(0,0,-width/2.0);
   for(theta=0; theta<=360; theta+=8) {
    glTexCoord2f(0.5*Sin(theta)+0.5, 0.5*Cos(theta)+0.5);
    glVertex3d(r*Cos(theta), r*Sin(theta),-width/2.0);
  }
  glEnd();

  //right cap
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(1,0,0);
  glTexCoord2f(0.5,0.5);
  glVertex3f(0,0,width/2.0);
   for(theta=0; theta<=360; theta+=8) {
    glTexCoord2f(0.5*Sin(theta)+0.5, 0.5*Cos(theta)+0.5);
    glVertex3d(r*Cos(theta), r*Sin(theta),width/2.0);
  }
  glEnd();
  glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates with normal
 */
void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

void flange(double x, double y, double z,
            double r, double r_inc,
            double l, double z_inc,
            int sidedness,
            int spinTheta)
{

  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(spinTheta,0,0,1);
  glScaled(r,r,sidedness);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,textures[0]);

  int theta;
  double r_curr = 1;
  double z_curr = 0;

  //Curved Section
  glBegin(GL_QUAD_STRIP);
  for (z_curr = 0; z_curr<=l; z_curr+=z_inc) {
    for (theta = 0; theta<=360;theta+=8) {
      
      glNormal3d(Sin(theta), Cos(theta), 0.75*z_curr/l );

      glTexCoord2f(z_curr/l, Sin(theta));
      glVertex3d(r_curr*Sin(theta), r_curr*Cos(theta), z_curr);

      glTexCoord2f((z_curr+z_inc)/l, Sin(theta));
      glVertex3d(pow(r_curr + r_inc,1.5)*Sin(theta), pow(r_curr + r_inc,1.5)*Cos(theta), z_curr+z_inc);
    }

    r_curr = pow(r_curr + r_inc,1.5);
  }
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glNormal3d(0,0,1);
  glTexCoord2f(0,0);
  glVertex3d(0,0, z_curr);
  for (theta = 0; theta<=360;theta+=8) {
    glTexCoord2f(Sin(theta), Cos(theta));
    glVertex3d(r_curr*Sin(theta), r_curr*Cos(theta), z_curr);
  }
  glEnd();

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);

}

void spoke(double x, double y, double z,
            double xrot, double yrot, double zrot,
            int spinTheta)
{
  glPushMatrix();
  glTranslated(x,y,z);

  glRotated(xrot,1,0,0);
  glRotated(yrot,0,1,0);
  glRotated(zrot,0,0,1);

  quarter_torus(0,0.2,-8.5, 90,0, 0.2,0.4, 0);
  horiz_cylinder(0,0,0, 0.08,8.5, spinTheta);

  glPopMatrix();
}

void spoke_placed(double x, double y, double z,
            double theta, double orientation, int spinTheta)

{
  double x_init = 0;
  double y_init = -9.65;
  double z_init = -1.2;

  double xrot_init = 83;
  double yrot_init = 0;
  double zrot_init = 0;

  glPushMatrix();
  glTranslated(x,y,z);

  glScaled(1,1,orientation);
  glRotated(theta,0,0,1);
  glRotated(spinTheta,0,0,1);

  spoke(x_init, y_init, z_init, xrot_init, yrot_init, zrot_init, spinTheta);
  glPopMatrix();
}



void axel_thread(double x, double y, double z,
                double radius, double length, double tpi)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

   int theta;
   glColor3f(0.1,0.1,0.1);
   glBegin(GL_QUAD_STRIP);
   for(theta = 0; theta<= tpi*length*360; theta+=8) {
    glNormal3f(0,0,1); //thread angle so small assume parallel to z direction for simplicity
    glVertex3d(radius*Cos(theta),radius*Sin(theta),(1/tpi)*(theta/360));
    glVertex3d((0.06 + radius)*Cos(theta), (0.06 + radius)*Sin(theta),(1/tpi)*(theta/360)); //thread height 1.5mm
   }
   glEnd();

   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
   glPopMatrix();  
}



void axel(double x, double y, double z,
          double radius, double length,
          int spinTheta )
{  
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,textures[8]);
  hub_cyl(0,0,0, radius, length, spinTheta);
  glDisable(GL_TEXTURE_2D);
}

/* Draws complete wheel */
void Wheel() {
  //Draw scene
  glPushMatrix();
  glRotated(globalX,1,0,0);
  glRotated(globalY,0,1,0);
  glNormal3f(0,0,1);
  sidewalls_reflective(0,0,rim_width/2, sidewall_r1,sidewall_r2);
  glNormal3f(0,0,-1);
  sidewalls_reflective(0,0,-rim_width/2, sidewall_r1,sidewall_r2);

  //Put non-transparent sidewall behind transparent reflective sidewall
  rim_sidewall(0,0, (rim_width-.001)/2, sidewall_r1,sidewall_r2, spinTheta);
  rim_sidewall(0,0,-(rim_width-.001)/2, sidewall_r1,sidewall_r2, spinTheta); 
  

  rim_base(0,0,0, rimbase_r1, rimbase_r2, rim_width, spinTheta);

  //Tire
  Torus(0,0,0, 0,0, 20, 0.06, spinTheta);

  //Hub Assembly
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,textures[0]);  
  hub_cyl(0,0,0, hub_r, hub_length, spinTheta);
  flange(0,0, hub_length/2 - 0.4, 0.5, 0.02, 0.5, 0.08, 1, spinTheta);
  flange(0,0,-hub_length/2 + 0.4, 0.5, 0.02, 0.5, 0.08, -1, spinTheta);
  
  //Axel
  glBindTexture(GL_TEXTURE_2D,textures[8]);  
  axel(0,0,0, axel_r, axel_length, 0);
  axel_thread(0,0,-axel_length/2, axel_r,axel_length, 24);

  //Spoke Placement
  int theta;
  for (theta = 0; theta<=360; theta+=20) {
  spoke_placed(0,0,0, theta,1, spinTheta);
  spoke_placed(0,0,0, theta+10,-1, spinTheta);

  }

  glPopMatrix();
}

