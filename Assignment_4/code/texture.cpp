#include "math3d.h"
#include "gltools.h"
#include"GL/glut.h"

#define ESCAPE 27
int window;

static GLint iWidth, iHeight, iComponents;
static GLenum eFormat;
static GLfloat v1,v2;

static GLfloat noLight[4] = {0.0f, 0.0f, 0.0f, 1.0f};
static GLfloat ambientLight[4] = {0.3f, 0.3f, 0.3f, 1.0f};
static GLfloat diffuseLight[4] = {0.7f, 0.7f, 0.7f, 1.0f};
static GLfloat brightLight[4] = {1.0f, 1.0f, 1.0f, 1.0f};

//light position
static GLfloat lightPos[] = { 5.0f, 5.0f, 5.0f, 1.0f};

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, brightLight);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glEnable(GL_LIGHT0);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, brightLight);

  glMateriali(GL_FRONT, GL_SHININESS, 30);

  //load texture image
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    void *pImage = NULL;
    pImage = gltReadTGABits("H:\\study\\program\\CG\\texture.tga", &iWidth, &iHeight, &iComponents, &eFormat);
    if (pImage)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
        free(pImage);
        pImage = NULL;
    }

  //set texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  //set texture environment
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);
}

//draw a pyramid, and set up its moving style
void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //vertices set
    M3DVector3f vertices[5] = {{0.0f, 0.8f, 0.0f},
        {-0.50f, 0.0f, -0.50f},
        {0.50f, 0.0f, -0.50f},
        {-0.50f, 0.0f, 0.50f},
        {0.50f, 0.0f, 0.50f}};

    M3DVector3f normal;

    glPushMatrix();
    glTranslatef(0.0f, -0.3f, -4.0f);
    //rotate round
    glRotatef(v1, 0.0f, 1.0f, 0.0f);
    glRotatef(v2, 1.0f, 0.0f, 0.0f);
    v1+=1;
    v2+=1;
        glBegin(GL_TRIANGLES);
      //down
      glColor3f(1.0f, 0.0f, 0.0f);
          glNormal3f(0.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
          glVertex3fv(vertices[1]);
      glTexCoord2f(1.0f, 1.0f);
          glVertex3fv(vertices[2]);
      glTexCoord2f(1.0f, 0.0f);
          glVertex3fv(vertices[4]);

      glTexCoord2f(1.0f, 0.0f);
          glVertex3fv(vertices[4]);
      glTexCoord2f(0.0f, 0.0f);
          glVertex3fv(vertices[3]);
      glTexCoord2f(0.0f, 1.0f);
          glVertex3fv(vertices[1]);

      //front
      glColor3f(0.0f, 1.0f, 0.0f);
          m3dFindNormal(normal, vertices[0], vertices[3], vertices[4]);
      glNormal3fv(normal);
      glTexCoord2f(0.5f, 0.5f);
          glVertex3fv(vertices[0]);
      glTexCoord2f(0.0f, 0.0f);
          glVertex3fv(vertices[3]);
      glTexCoord2f(1.0f, 0.0f);
          glVertex3fv(vertices[4]);

      //left
      glColor3f(0.0f, 0.0f, 1.0f);
      m3dFindNormal(normal, vertices[1], vertices[3], vertices[0]);
      glNormal3fv(normal);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3fv(vertices[1]);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3fv(vertices[3]);
      glTexCoord2f(0.5f, 0.5f);
      glVertex3fv(vertices[0]);

      //right
      glColor3f(0.0f, 1.0f, 1.0f);
      m3dFindNormal(normal, vertices[0], vertices[4], vertices[2]);
      glNormal3fv(normal);
      glTexCoord2f(0.5f, 0.5f);
      glVertex3fv(vertices[0]);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3fv(vertices[4]);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3fv(vertices[2]);

      //back
      glColor3f(1.0f, 0.0f, 1.0f);
      m3dFindNormal(normal, vertices[0], vertices[2], vertices[1]);
      glNormal3fv(normal);
      glTexCoord2f(0.5f, 0.5f);
      glVertex3fv(vertices[0]);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3fv(vertices[2]);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3fv(vertices[1]);

        glEnd();

    glPopMatrix();

    glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    float fAspect = (GLfloat)w/(GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(35.0, fAspect, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}

//action taken when keyboard be pressed
void keyPressed(unsigned char key, int x, int y)
{
    //enter esc, then break
    if (key == ESCAPE)
    {
        glutDestroyWindow(window);
        exit(0);
    }
}

void TimerFunc(int value)
{
  glutPostRedisplay();
  glutTimerFunc(60, TimerFunc, 1);
}


int main(int args, char *arv[])
{
    glutInit(&args, arv);
    glutInitDisplayMode(GL_RGB | GL_DOUBLE | GL_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("texture");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    SetupRC();

    glutTimerFunc(50, TimerFunc, 1);

    glutFullScreen();
    glutKeyboardFunc(&keyPressed);
    glutMainLoop();

    return 0;
}
