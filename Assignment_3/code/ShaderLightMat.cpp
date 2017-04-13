#include<windows.h>
#include "unistd.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GL/freeglut.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<cstdio>
//Esc
#define ESCAPE 27

using namespace std;

class Renderer {

private:
  struct Vertex {
    float position[3];
    float texCoord[2];//coordinate
    float normal[3];
  };

public:
  float t;
  int modeVal;
  std::string shaderFilename;
  int shaderFile;

private:
  enum {Scene, numVAOs};//Vertex Array Object
  enum {SceneAll, numVBOs};
  GLuint vaoID[numVAOs];
  GLuint bufID[numVBOs];
  int sceneVertNo;
  GLuint progID;
  GLuint vertID;
  GLuint fragID;
  GLint vertexLoc;
  GLint texCoordLoc;
  GLint normalLoc;
  GLint projectionLoc;
  GLint modelviewLoc;
  GLint normalMatrixLoc;
  GLint modeLoc;
  float projection[16];  // projection matrix
  float modelview[16];  // modelview matrix
  std::string filename="./sphere.vbo";
  int file;

public:
  // constructor
  Renderer() : t(0.0), modeVal(1), shaderFilename("FlatShading"), shaderFile(0),
               sceneVertNo(0), progID(0), vertID(0), fragID(0),
               vertexLoc(-1), texCoordLoc(-1), normalLoc(-1),
               projectionLoc(-1), modelviewLoc(-1), normalMatrixLoc(-1), modeLoc(-1),
               filename("./sphere.vbo"), file(0)
               {}
  //destructor
  ~Renderer() {
     deleteAll();
  }

public:

  void reload() {
    deleteAll();
    init();
  }

  void init() {
    glEnable(GL_DEPTH_TEST);

    setupShaders();

    // create a Vertex Array Objects (VAO)
    glGenVertexArrays(numVAOs, vaoID);

    // generate a Vertex Buffer Object (VBO)
    glGenBuffers(numVBOs, bufID);

    // binding the pyramid VAO
    glBindVertexArray(vaoID[Scene]);

    std::vector <float> data;
    loadVertexData(filename, data);

    sceneVertNo = int(data.size()) / (3+2+3);

    glBindBuffer(GL_ARRAY_BUFFER, bufID[SceneAll]);
    glBufferData(GL_ARRAY_BUFFER, sceneVertNo*sizeof(Vertex),
                 &data[0], GL_STATIC_DRAW);

    int stride = sizeof(Vertex);
    char *offset = (char*)NULL;

    // position
    if(vertexLoc != -1) {
      glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, stride, offset);
      glEnableVertexAttribArray(vertexLoc);
    }

    // texCoord
    if(texCoordLoc != -1) {
      offset = (char*)NULL + 3*sizeof(float);
      glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, stride, offset);
      glEnableVertexAttribArray(texCoordLoc);
    }

    // normal
    if(normalLoc != -1){
      offset = (char*)NULL + (3+2)*sizeof(float);
      glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, stride, offset);
      glEnableVertexAttribArray(normalLoc);
    }
  }


  void resize(int w, int h) {
    glViewport(0, 0, w, h);

    // this function replaces gluPerspective
    mat4Perspective(projection, 45.0f, (float)w/(float)h, 0.5f, 4.0f);
    // mat4Print(projection);
  }

  void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // camera orbits in the z=2 plane
    // and looks at the origin
    // mat4LookAt replaces gluLookAt
    double rad = M_PI / 180.0f * t;
    mat4LookAt(modelview,
               1.5f*float(cos(rad)), 1.5f*float(sin(rad)), 1.5f, // eye
               0.0f, 0.0f, 0.0f, // look at
               0.0f, 0.0f, 1.0f); // up


    float modelviewInv[16], normalmatrix[16];
    mat4Invert(modelview, modelviewInv);
    mat4Transpose(modelviewInv, normalmatrix);

    glUseProgram(progID);
    // load the current projection and modelview matrix into the
    // corresponding UNIFORM variables of the shader
    glUniformMatrix4fv(projectionLoc, 1, false, projection);
    glUniformMatrix4fv(modelviewLoc, 1, false, modelview);
    if(normalMatrixLoc != -1) glUniformMatrix4fv(normalMatrixLoc, 1, false, normalmatrix);
    if(modeLoc != -1) glUniform1i(modeLoc, modeVal);

    // bind Triangle VAO
    glBindVertexArray(vaoID[Scene]);
    // render data
    glDrawArrays(GL_TRIANGLES, 0, sceneVertNo);
  }
private:
  void setupShaders() {

    // create shader
    vertID = glCreateShader(GL_VERTEX_SHADER);
    fragID = glCreateShader(GL_FRAGMENT_SHADER);

    // load shader source from file
    std::string vs = loadShaderSrc("./"+shaderFilename+".vert");
    const char* vss = vs.c_str();
    std::string fs = loadShaderSrc("./"+shaderFilename+".frag");
    const char* fss = fs.c_str();

    // specify shader source
    glShaderSource(vertID, 1, &(vss), NULL);
    glShaderSource(fragID, 1, &(fss), NULL);

    // compile the shader
    glCompileShader(vertID);
    glCompileShader(fragID);

    // create program and attach shaders
    progID = glCreateProgram();
    glAttachShader(progID, vertID);
    glAttachShader(progID, fragID);

    // "outColor" is a user-provided OUT variable
    // of the fragment shader.
    // Its output is bound to the first color buffer
    // in the framebuffer
    glBindFragDataLocation(progID, 0, "outputColor");

    // link the program
    glLinkProgram(progID);

    // retrieve the location of the IN variables of the vertex shaders
    vertexLoc = glGetAttribLocation(progID,"inputPosition");
    texCoordLoc = glGetAttribLocation(progID,"inputTexCoord");
    normalLoc = glGetAttribLocation(progID, "inputNormal");

    // retrieve the location of the UNIFORM variables of the shaders
    projectionLoc = glGetUniformLocation(progID, "projection");
    modelviewLoc = glGetUniformLocation(progID, "modelview");
    normalMatrixLoc = glGetUniformLocation(progID, "normalMat");
    modeLoc = glGetUniformLocation(progID, "mode");
  }


  void deleteAll() {
    glDeleteVertexArrays(numVAOs, vaoID);
    glDeleteBuffers(numVBOs, bufID);
    glDeleteProgram(progID);
    glDeleteShader(vertID);
    glDeleteShader(fragID);
  }

  std::string loadShaderSrc(const std::string& filename) {
    int s=filename.length();
    std::ifstream is(filename.c_str());
    if (is.is_open()) {
      std::stringstream buffer;
      buffer << is.rdbuf();
      return buffer.str();
    }
    cerr << "Unable to open file " << filename << endl;
    exit(1);
  }

  // the following functions are some matrix and vector helpers
  float vec3Dot( float *a, float *b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
  }

  void vec3Cross( float *a, float *b, float *res) {
    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
  }

  void vec3Normalize(float *a) {
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
    a[0] /= mag; a[1] /= mag; a[2] /= mag;
  }

  void mat4Identity( float *a) {
    for (int i = 0; i < 16; ++i) a[i] = 0.0f;
    for (int i = 0; i < 4; ++i) a[i + i * 4] = 1.0f;
  }

  void mat4Multiply(float *a, float *b, float *res) {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        res[j*4 + i] = 0.0f;
        for (int k = 0; k < 4; ++k) {
          res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
        }
      }
    }
  }

  void mat4Perspective(float *a, float fov, float aspect, float zNear, float zFar) {
    float f = 1.0f / float(tan (fov/2.0f * (M_PI / 180.0f)));
    mat4Identity(a);
    a[0] = f / aspect;
    a[1 * 4 + 1] = f;
    a[2 * 4 + 2] = (zFar + zNear)  / (zNear - zFar);
    a[3 * 4 + 2] = (2.0f * zFar * zNear) / (zNear - zFar);
    a[2 * 4 + 3] = -1.0f;
    a[3 * 4 + 3] = 0.0f;
  }

  void mat4LookAt(float *viewMatrix,
                  float eyeX, float eyeY, float eyeZ,
                  float centerX, float centerY, float centerZ,
                  float upX, float upY, float upZ) {

    float dir[3], right[3], up[3], eye[3];
    up[0]=upX; up[1]=upY; up[2]=upZ;
    eye[0]=eyeX; eye[1]=eyeY; eye[2]=eyeZ;

    dir[0]=centerX-eyeX; dir[1]=centerY-eyeY; dir[2]=centerZ-eyeZ;
    vec3Normalize(dir);
    vec3Cross(dir,up,right);
    vec3Normalize(right);
    vec3Cross(right,dir,up);
    vec3Normalize(up);
    // first row
    viewMatrix[0]  = right[0];
    viewMatrix[4]  = right[1];
    viewMatrix[8]  = right[2];
    viewMatrix[12] = -vec3Dot(right, eye);
    // second row
    viewMatrix[1]  = up[0];
    viewMatrix[5]  = up[1];
    viewMatrix[9]  = up[2];
    viewMatrix[13] = -vec3Dot(up, eye);
    // third row
    viewMatrix[2]  = -dir[0];
    viewMatrix[6]  = -dir[1];
    viewMatrix[10] = -dir[2];
    viewMatrix[14] =  vec3Dot(dir, eye);
    // forth row
    viewMatrix[3]  = 0.0f;
    viewMatrix[7]  = 0.0f;
    viewMatrix[11] = 0.0f;
    viewMatrix[15] = 1.0f;
  }

  void mat4Print(float* a) {
    // opengl uses column major order
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        cout << a[j * 4 + i] << " ";
      }
      cout << endl;
    }
  }

  void mat4Transpose(float* a, float *transposed) {
    int t = 0;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        transposed[t++] = a[j * 4 + i];
      }
    }
  }

  bool mat4Invert(float* m, float *inverse) {
    float inv[16];
    inv[0] = m[5]*m[10]*m[15]-m[5]*m[11]*m[14]-m[9]*m[6]*m[15]+
             m[9]*m[7]*m[14]+m[13]*m[6]*m[11]-m[13]*m[7]*m[10];
    inv[4] = -m[4]*m[10]*m[15]+m[4]*m[11]*m[14]+m[8]*m[6]*m[15]-
             m[8]*m[7]*m[14]-m[12]*m[6]*m[11]+m[12]*m[7]*m[10];
    inv[8] = m[4]*m[9]*m[15]-m[4]*m[11]*m[13]-m[8]*m[5]*m[15]+
             m[8]*m[7]*m[13]+m[12]*m[5]*m[11]-m[12]*m[7]*m[9];
    inv[12]= -m[4]*m[9]*m[14]+m[4]*m[10]*m[13]+m[8]*m[5]*m[14]-
             m[8]*m[6]*m[13]-m[12]*m[5]*m[10]+m[12]*m[6]*m[9];
    inv[1] = -m[1]*m[10]*m[15]+m[1]*m[11]*m[14]+m[9]*m[2]*m[15]-
             m[9]*m[3]*m[14]-m[13]*m[2]*m[11]+m[13]*m[3]*m[10];
    inv[5] = m[0]*m[10]*m[15]-m[0]*m[11]*m[14]-m[8]*m[2]*m[15]+
             m[8]*m[3]*m[14]+m[12]*m[2]*m[11]-m[12]*m[3]*m[10];
    inv[9] = -m[0]*m[9]*m[15]+m[0]*m[11]*m[13]+m[8]*m[1]*m[15]-
             m[8]*m[3]*m[13]-m[12]*m[1]*m[11]+m[12]*m[3]*m[9];
    inv[13]= m[0]*m[9]*m[14]-m[0]*m[10]*m[13]-m[8]*m[1]*m[14]+
             m[8]*m[2]*m[13]+m[12]*m[1]*m[10]-m[12]*m[2]*m[9];
    inv[2] = m[1]*m[6]*m[15]-m[1]*m[7]*m[14]-m[5]*m[2]*m[15]+
             m[5]*m[3]*m[14]+m[13]*m[2]*m[7]-m[13]*m[3]*m[6];
    inv[6] = -m[0]*m[6]*m[15]+m[0]*m[7]*m[14]+m[4]*m[2]*m[15]-
             m[4]*m[3]*m[14]-m[12]*m[2]*m[7]+m[12]*m[3]*m[6];
    inv[10]= m[0]*m[5]*m[15]-m[0]*m[7]*m[13]-m[4]*m[1]*m[15]+
             m[4]*m[3]*m[13]+m[12]*m[1]*m[7]-m[12]*m[3]*m[5];
    inv[14]= -m[0]*m[5]*m[14]+m[0]*m[6]*m[13]+m[4]*m[1]*m[14]-
             m[4]*m[2]*m[13]-m[12]*m[1]*m[6]+m[12]*m[2]*m[5];
    inv[3] = -m[1]*m[6]*m[11]+m[1]*m[7]*m[10]+m[5]*m[2]*m[11]-
             m[5]*m[3]*m[10]-m[9]*m[2]*m[7]+m[9]*m[3]*m[6];
    inv[7] = m[0]*m[6]*m[11]-m[0]*m[7]*m[10]-m[4]*m[2]*m[11]+
             m[4]*m[3]*m[10]+m[8]*m[2]*m[7]-m[8]*m[3]*m[6];
    inv[11]= -m[0]*m[5]*m[11]+m[0]*m[7]*m[9]+m[4]*m[1]*m[11]-
             m[4]*m[3]*m[9]-m[8]*m[1]*m[7]+m[8]*m[3]*m[5];
    inv[15]= m[0]*m[5]*m[10]-m[0]*m[6]*m[9]-m[4]*m[1]*m[10]+
             m[4]*m[2]*m[9]+m[8]*m[1]*m[6]-m[8]*m[2]*m[5];

    float det = m[0]*inv[0]+m[1]*inv[4]+m[2]*inv[8]+m[3]*inv[12];
    if (det == 0) return false;
    det = 1.0f / det;
    for (int i = 0; i < 16; i++) inverse[i] = inv[i] * det;
    return true;
  }

  bool loadVertexData(std::string &filename, std::vector<float> &data) {
    // read vertex data from file
    ifstream input(filename.c_str());
    if(!input) {
      cerr << "Can not find vertex data file " << filename << endl;
      return false;
    } else {
      int vertSize;
      double vertData;
      if(input >> vertSize) {
        if(vertSize > 0) {
          data.resize(vertSize);
          int i = 0;
          while(input >> vertData && i < vertSize) {
            // store it in the vector.
            data[i] = float(vertData);
            i++;
          }
          if(i != vertSize || vertSize % (3+2+3)) data.resize(0);
        }
      }
      input.close();
    }
    return false;
  }
};
int window;

//this is a static pointer to a Renderer used in the glut callback functions
static Renderer *renderer;

//glut static callbacks start
static void glutResize(int w, int h)
{
  renderer->resize(w,h);
}

static void glutDisplay()
{
  renderer->display();
  glutSwapBuffers();
  glutReportErrors();
}

static void timer(int v)
{
  float offset = 1.0f;
  renderer->t += offset;
  glutDisplay();
  glutTimerFunc(unsigned(20), timer, ++v);
}

static void glutKeyboard(unsigned char key, int x, int y) {
  bool redraw = false;
  std::string modeStr;
  std::stringstream ss;
  switch(key) {
    case '1':
      renderer->shaderFile++;
      if(renderer->shaderFile > 2)  renderer->shaderFile = 0;
      if(renderer->shaderFile == 0) renderer->shaderFilename = "FlatShading";
      if(renderer->shaderFile == 1) renderer->shaderFilename = "GouraudShading";
      if(renderer->shaderFile == 2) renderer->shaderFilename = "PhongShading";
      renderer->reload();
      redraw = true;
      modeStr = renderer->shaderFilename;
      break;
    case ESCAPE:
      glutDestroyWindow(window);
      //exit normally
      exit(0);
    }
    if(redraw) {
      glutDisplay();
      glutSetWindowTitle(modeStr.c_str());
    }
}


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(320, 320);

  window=glutCreateWindow("FlatShading");
  glewInit();
  glutDisplayFunc(glutDisplay);
  glutFullScreen();

  glutIdleFunc(glutDisplay);
  glutReshapeFunc(glutResize);
  glutKeyboardFunc(glutKeyboard);

  renderer = new Renderer;
  renderer->init();

  glutTimerFunc(unsigned(20), timer, 0);

  glutMainLoop();
}
