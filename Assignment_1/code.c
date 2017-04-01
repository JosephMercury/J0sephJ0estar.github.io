#include<windows.h>//涓€瀹氳鍔犲湪绗竴琛屼笉鐒朵細鎶ラ敊
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "unistd.h"
#include <stdio.h>
//鎺ユ敹Esc
#define ESCAPE 27

int window;

//瀹氫箟鐢诲浘涓殑鍚勪釜鍙傛暟
float angleOfLeftArmA,angleOfLeftArmB,angleOfRightArmA,angleOfRightArmB,angleOfLeftLegA,angleOfLeftLegB,angleOfRightLegA,angleOfRightLegB,a = 0.4,b=0.2,angle = 0.1;
GLuint texture[1];
//openGL鍒濆鍖栫▼搴?
void InitGL(int Width, int Height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
}

// The function called when our window is resized
void ReSizeGLScene(int Width, int Height)
{
    if (Height==0)
        Height=1;
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

//鐢昏韩浣撶殑鍚勪釜閮ㄥ垎鐨勫嚱鏁?
//openGL宸﹁竟绯讳负鍙虫墜绯讳笖y杞存湞涓婏紝z杞翠粠灞忓箷鎸囧嚭
//涓€娆＄敾涓よ竟锛屽洜涓烘槸鏍规嵁y杞村绉版墍浠ユ瘡瀵归兘鏄痽涓婂潗鏍囦笉鍙?
void DrawBody()
{
    glColor3f(0.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    //鐢绘湞鍓嶇殑闈?
    glVertex3f(-0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f,-1.0f, 0.25f);
    glVertex3f(-0.5f,-1.0f, 0.25f);
    //鐢绘湞宸︾殑闈?
    glVertex3f( 0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f, 1.0f,-0.25f);
    glVertex3f( 0.5f,-1.0f,-0.25f);
    glVertex3f( 0.5f,-1.0f, 0.25f);
    //鐢绘湞鍚庣殑闈?
    glVertex3f( 0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f,-0.25f);
    glVertex3f( 0.5f,-1.0f,-0.25f);
    //鐢绘湞鍙崇殑闈?
    glVertex3f(-0.5f, 1.0f, 0.25f);
    glVertex3f(-0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f, 0.25f);
    //鐢绘湞涓婄殑闈?
    glVertex3f( 0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f, 1.0f, 0.25f);
    //鐢绘湞涓嬬殑闈?
    glVertex3f( 0.5f,-1.0f, 0.25f);
    glVertex3f( 0.5f,-1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f, 0.25f);
    glEnd();
}
void DrawShoulder()
{
    glColor3f(0.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    //鐢绘湞鍓嶇殑闈?
    glVertex3f(-1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f,-0.2f, 0.25f);
    glVertex3f(-1.0f,-0.2f, 0.25f);
    //鐢绘湞宸︾殑闈?
    glVertex3f( 1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f, 0.2f,-0.25f);
    glVertex3f( 1.0f,-0.2f,-0.25f);
    glVertex3f( 1.0f,-0.2f, 0.25f);
    //鐢绘湞鍚庣殑闈?
    glVertex3f( 1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f,-0.25f);
    glVertex3f( 1.0f,-0.2f,-0.25f);
    //鐢绘湞鍙崇殑闈?
    glVertex3f(-1.0f, 0.2f, 0.25f);
    glVertex3f(-1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f, 0.25f);
    //鐢绘湞涓婄殑闈?
    glVertex3f( 1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f, 0.2f, 0.25f);
    //鐢绘湞涓嬬殑闈?
    glVertex3f( 1.0f,-0.2f, 0.25f);
    glVertex3f( 1.0f,-0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f, 0.25f);
    glEnd();
}
void DrawWaise()
{
    glColor3f(0.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    //鐢绘湞鍓嶇殑闈?
    glVertex3f(-0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f,-0.2f, 0.25f);
    glVertex3f(-0.7f,-0.2f, 0.25f);
    //鐢绘湞宸︾殑闈?
    glVertex3f( 0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f, 0.2f,-0.25f);
    glVertex3f( 0.7f,-0.2f,-0.25f);
    glVertex3f( 0.7f,-0.2f, 0.25f);
    //鐢绘湞鍚庣殑闈?
    glVertex3f( 0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f,-0.25f);
    glVertex3f( 0.7f,-0.2f,-0.25f);
    //鐢绘湞鍙崇殑闈?
    glVertex3f(-0.7f, 0.2f, 0.25f);
    glVertex3f(-0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f, 0.25f);
    //鐢绘湞涓婄殑闈?
    glVertex3f( 0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f, 0.2f, 0.25f);
    //鐢绘湞涓嬬殑闈?
    glVertex3f( 0.7f,-0.2f, 0.25f);
    glVertex3f( 0.7f,-0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f, 0.25f);
    glEnd();
}
void DrawArmA()
{
    glColor3f(0.0f,0.0f,1.0f);
    glBegin(GL_QUADS);

    //鐢绘湞鍓嶇殑闈?
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f,-1.0f, 0.25f);
    glVertex3f(-0.2f,-1.0f, 0.25f);
    //鐢绘湞宸︾殑闈?
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
    glVertex3f( 0.2f,-1.0f, 0.25f);
    //鐢绘湞鍚庣殑闈?
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
    //鐢绘湞鍙崇殑闈?
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
    glVertex3f(-0.2f,-1.0f, 0.25f);
    //鐢绘湞涓婄殑闈?
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f, 0.25f);
    //鐢绘湞涓嬬殑闈?
    glVertex3f( 0.2f,-1.0f, 0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
    glVertex3f(-0.2f,-1.0f, 0.25f);


    glEnd();
}
void DrawArmB()
{
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_QUADS);

    //鐢绘湞鍓嶇殑闈?
    glVertex3f(-0.2f, -0.56f, 0.21f);
    glVertex3f( 0.2f, -0.56f, 0.21f);
    glVertex3f( 0.2f,-2.0f, 0.21f);
    glVertex3f(-0.2f,-2.0f, 0.21f);
    //鐢绘湞宸︾殑闈?
    glVertex3f( 0.2f, -0.56f, 0.21f);
    glVertex3f( 0.2f, -0.56f,-0.21f);
    glVertex3f( 0.2f,-2.0f,-0.21f);
    glVertex3f( 0.2f,-2.0f, 0.21f);
    //鐢绘湞鍚庣殑闈?
    glVertex3f( 0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f,-2.0f,-0.21f);
    glVertex3f( 0.2f,-2.0f,-0.21f);
    //鐢绘湞鍙崇殑闈?
    glVertex3f(-0.2f, -0.56f, 0.21f);
    glVertex3f(-0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f,-2.0f,-0.21f);
    glVertex3f(-0.2f,-2.0f, 0.21f);
    //鐢绘湞涓婄殑闈?
    glVertex3f( 0.2f, -0.56f, 0.21f);
    glVertex3f( 0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f, -0.56f, 0.21f);
    //鐢绘湞涓嬬殑闈?
    glVertex3f(-0.2f,-2.0f, 0.25f);
    glVertex3f( 0.2f,-2.0f, 0.25f);
    glVertex3f( 0.2f,-2.0f,-0.25f);
    glVertex3f(-0.2f,-2.0f,-0.25f);
    glEnd();
}
void DrawLegA()
{
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.3f,0.25f, 0.0f);
    glVertex3f( 0.3f,0.25f, 0.0f);
    glVertex3f( 0.3f,-1.5f, 0.0f);
    glVertex3f(-0.3f,-1.5f, 0.0f);
    //鐢绘湞鍓嶇殑闈?
    glVertex3f(-0.3f,0.25f, 0.255f);
    glVertex3f( 0.3f,0.25f, 0.255f);
    glVertex3f( 0.3f,-1.5f, 0.255f);
    glVertex3f(-0.3f,-1.5f, 0.255f);
    //鐢绘湞宸︾殑闈?
    glVertex3f( 0.3f,0.25f, 0.255f);
    glVertex3f( 0.3f,0.25f,-0.255f);
    glVertex3f( 0.3f,-1.5f,-0.255f);
    glVertex3f( 0.3f,-1.5f, 0.255f);
    //鐢绘湞鍚庣殑闈?
    glVertex3f( 0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,-1.5f,-0.255f);
    glVertex3f( 0.3f,-1.5f,-0.255f);
    //鐢绘湞鍙崇殑闈?
    glVertex3f(-0.3f,0.25f, 0.255f);
    glVertex3f(-0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,-1.5f,-0.255f);
    glVertex3f(-0.3f,-1.5f, 0.255f);
    //鐢绘湞涓婄殑闈?
    glVertex3f( 0.3f,0.25f, 0.255f);
    glVertex3f( 0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,0.25f, 0.255f);
    //鐢绘湞涓嬬殑闈?
    glVertex3f( 0.3f,-1.5f, 0.255f);
    glVertex3f( 0.3f,-1.5f,-0.255f);
    glVertex3f(-0.3f,-1.5f,-0.255f);
    glVertex3f(-0.3f,-1.5f, 0.255f);

    glEnd();
}
void DrawLegB()
{
    glColor3f(0.0f,0.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.3f,-1.2f, 0.0f);
    glVertex3f( 0.3f,-1.2f, 0.0f);
    glVertex3f( 0.3f,-3.25f, 0.0f);
    glVertex3f(-0.3f,-3.25f, 0.0f);
    //鐢绘湞鍓嶇殑闈?
    glVertex3f(-0.3f,-1.2f, 0.255f);
    glVertex3f( 0.3f,-1.2f, 0.255f);
    glVertex3f( 0.3f,-3.25f, 0.255f);
    glVertex3f(-0.3f,-3.25f, 0.255f);
    //鐢绘湞宸︾殑闈?
    glVertex3f( 0.3f,-1.2f, 0.255f);
    glVertex3f( 0.3f,-1.2f,-0.255f);
    glVertex3f( 0.3f,-3.25f,-0.255f);
    glVertex3f( 0.3f,-3.25f, 0.255f);
    //鐢绘湞鍚庣殑闈?
    glVertex3f( 0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-3.25f,-0.255f);
    glVertex3f( 0.3f,-3.25f,-0.255f);
    //鐢绘湞鍙崇殑闈?
    glVertex3f(-0.3f,-1.2f, 0.255f);
    glVertex3f(-0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-3.25f,-0.255f);
    glVertex3f(-0.3f,-3.25f, 0.255f);
    //鐢绘湞涓婄殑闈?
    glVertex3f( 0.3f,-1.2f, 0.255f);
    glVertex3f( 0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-1.2f, 0.255f);
    //鐢绘湞涓嬬殑闈?
    glVertex3f( 0.3f,-3.25f, 0.255f);
    glVertex3f( 0.3f,-3.25f,-0.255f);
    glVertex3f(-0.3f,-3.25f,-0.255f);
    glVertex3f(-0.3f,-3.25f, 0.255f);

    glEnd();
}
void DrawHead()
{
    glColor3f(0.0f,1.0f,1.0f);
    glBegin(GL_QUADS);

    //鐢绘湞鍓嶇殑闈?
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.6f, 0.6f, 0.35f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 0.6f, 0.6f, 0.35f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 0.6f,-0.6f, 0.35f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.6f,-0.6f, 0.35f);
    //鐢绘湞宸︾殑闈?
    glVertex3f( 0.6f, 0.6f, 0.35f);
    glVertex3f( 0.6f, 0.6f,-0.35f);
    glVertex3f( 0.6f,-0.6f,-0.35f);
    glVertex3f( 0.6f,-0.6f, 0.35f);
    //鐢绘湞鍚庣殑闈?
    glVertex3f( 0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f,-0.35f);
    glVertex3f( 0.6f,-0.6f,-0.35f);
    //鐢绘湞鍙崇殑闈?
    glVertex3f(-0.6f, 0.6f, 0.35f);
    glVertex3f(-0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f, 0.35f);
    //鐢绘湞涓婄殑闈?
    glVertex3f( 0.6f, 0.6f, 0.35f);
    glVertex3f( 0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f, 0.6f, 0.35f);
    //鐢绘湞涓嬬殑闈?
    glVertex3f( 0.6f,-0.6f, 0.35f);
    glVertex3f( 0.6f,-0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f, 0.35f);
    glEnd();
}
void DrawNick()
{
    glColor3f(0.0f,1.0f,1.0f);
    glBegin(GL_QUADS);

    //鐢绘湞鍓嶇殑闈?
    glVertex3f(-0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f,-0.1f, 0.25f);
    glVertex3f(-0.2f,-0.1f, 0.25f);
    //鐢绘湞宸︾殑闈?
    glVertex3f( 0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f, 0.1f,-0.25f);
    glVertex3f( 0.2f,-0.1f,-0.25f);
    glVertex3f( 0.2f,-0.1f, 0.25f);
    //鐢绘湞鍚庣殑闈?
    glVertex3f( 0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f,-0.25f);
    glVertex3f( 0.2f,-0.1f,-0.25f);
    //鐢绘湞鍙崇殑闈?
    glVertex3f(-0.2f, 0.1f, 0.25f);
    glVertex3f(-0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f, 0.25f);
    //鐢绘湞涓婄殑闈?
    glVertex3f( 0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f, 0.1f, 0.25f);
    //鐢绘湞涓嬬殑闈?
    glVertex3f( 0.2f,-0.1f, 0.25f);
    glVertex3f( 0.2f,-0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f, 0.25f);

    glEnd();
}
//涓荤敾鍥剧▼搴?
void DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();//褰撳墠鐭╅樀閲嶇疆涓哄崟浣嶇煩闃?
    //涓€寮€濮嬩粠鍙冲悗鏂瑰嚭鐜?
    glTranslatef(0.0f,0.0f,-40.0f);//灏嗗綋鍓嶅浘鍍忓悜z杞磋礋鏂瑰悜绉诲姩
    glRotatef(angle,0,1,0);//缁晊杞翠笉鍋滄棆杞紝閫犳垚绉诲姩鐨勫亣璞?
    glPushMatrix();//寤虹珛灞€閮ㄥ潗鏍囩郴
    glTranslatef(0.0f,0.0f,-20.0f);
    glRotatef(-90,0,1,0);//淇濇寔鑴告甯告湞鍚?
    glPushMatrix();

    glTranslatef(0.0f,1.2f,0.0f);//娌縴杞存鏂瑰悜
    glPushMatrix();
    glTranslatef(0.8,0.0,0.0);
    glTranslatef(0.0,-0.4,0.0);
    glRotatef(angleOfLeftArmA,-1,0,0);//鑳宠唺涓嶆柇鍓嶅悗绉诲姩
    //鐢昏兂鑶婄殑涓ゆ
    DrawArmA();
    glPushMatrix();
    glTranslatef(0.0,-0.37,0);
    glPushMatrix();
    glRotatef(angleOfLeftArmB,-1,0,0);
    DrawArmB();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();//鎭㈠鍏ㄥ眬鍧愭爣绯?

    glPushMatrix();
    glTranslatef(-0.8,0.0,0.0);
    glTranslatef(0.0,-0.4,0.0);
    glRotatef(angleOfRightArmA,-1,0,0);
    DrawArmA();
    glPushMatrix();
    glTranslatef(0.0,-0.37,0);
    glRotatef(angleOfRightArmB,-1,0,0);
    glPushMatrix();
    DrawArmB();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    //杩斿洖鑲╄唨
    glPushMatrix();
    glTranslatef(0.0,0.3,0.0);
    glPushMatrix();
    glTranslatef(0.0,0.7,0.0);
    DrawHead();
    glPopMatrix();
    DrawNick();
    glPopMatrix();
    DrawShoulder();
    glPopMatrix();
    //杩斿洖韬綋
    glPushMatrix();
    glTranslatef(0.0f,-1.2f,0.0f);
    glPushMatrix();
    glTranslatef(0.4,0.0,0.0);
    glTranslatef(0.0,-0.4,0.0);
    glRotatef(angleOfLeftLegA,-1,0,0);
    DrawLegA();
    glPushMatrix();
    glTranslatef(0.0,-0.3,0.0);
    glPushMatrix();
    glRotatef(angleOfLeftLegB,-1,0,0);
    DrawLegB();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.4,0.0,0.0);
    glTranslatef(0.0,-0.4,0.0);
    glRotatef(angleOfRightLegA,-1,0,0);
    DrawLegA();
    glPushMatrix();
    glTranslatef(0.0,-0.3,0.0);
    glPushMatrix();
    glRotatef(angleOfRightLegB,-1,0,0);
    DrawLegB();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    DrawWaise();
    glPopMatrix();
    DrawBody();
    glPopMatrix();
    glPopMatrix();
    //宸﹀彸鑳宠唺杩愬姩骞呭害淇濇寔涓€鑷?
    angleOfLeftArmA+=a;
    angleOfRightArmA-=a;

    //鍦ㄤ富鑳宠唺鍦ㄨ韩浣撲笂鏂规椂灏忚兂鑶婄殑杩愬姩鎯呭喌
    if(angleOfLeftArmA>0)
    {
        //濡傛灉鍦ㄥ悜涓婅繍鍔ㄧ殑璇?
        if(a>0)
        {
            //灏忚兂鑶婁細涓婃憜涓€瀹氳搴﹀悗淇濇寔
            if(angleOfLeftArmB<45)
                angleOfLeftArmB+=a;
            else angleOfLeftArmB=45;
        }
        else//鍦ㄥ悜涓嬭繍鍔ㄧ殑璇濆皬鑳宠唺浼氫笅鎽嗙洿鑷充笌涓昏兂鑶婂钩榻?
            if(angleOfLeftArmB>0)
                angleOfLeftArmB+=a;
    }
    else//韬綋涓嬫柟鏃朵笌涓昏兂鑶婅繍鍔ㄦ儏鍐电浉鍚?
        angleOfLeftArmB=0;

    //瀵瑰彸鑳宠唺鍚岀悊锛屼笉杩嘺<0鏃朵唬琛ㄥ悜涓婅繍鍔?
    if(angleOfRightArmA>0)
    {
        if(a<0)
        {
            if(angleOfRightArmB<45)
                angleOfRightArmB-=a;
            else angleOfRightArmB=45;
        }
        else if(angleOfRightArmB>=0)
            angleOfRightArmB-=a;
    }
    else angleOfRightArmB=0;

    //宸﹀彸鑵胯繍鍔ㄥ箙搴︿繚鎸佷竴鑷翠笖涓庤兂鑶婄浉瀵圭浉鍙?
    angleOfLeftLegA-=b;
    angleOfRightLegA+=b;

    //鍦ㄨ韩浣撳悗鏂规椂灏忚吙寰井灞堣吙
    if(angleOfRightLegA<0)
        angleOfRightLegB=-15;
    else
        angleOfRightLegB=0;
    if(angleOfLeftLegA<0)
        angleOfLeftLegB=-15;
    else
        angleOfLeftLegB=0;

    angle+=0.1;//鏁翠綋缁晊杞磋浆鐨勯€熷害

    //鑻ヨ繍鍔ㄥ埌浜嗘渶澶у箙搴︽椂锛屽紑濮嬪弽鍚戣繍鍔?
    if(angleOfLeftArmA>50)
    {
        a=-0.4;
        b=-0.2;
    }
    if(angleOfLeftArmA<-50)
    {
        a=0.4;
        b=0.2;
    }
    glutSwapBuffers();
}

//鎺ユ敹鍒版寜閿椂鐨勫搷搴旂▼搴?
void keyPressed(unsigned char key, int x, int y)
{
    //鐭殏浼戠湢锛屽彲浠ユ洿鍔犱粩缁嗚瀵熻繍鍔ㄦ儏鍐?
    usleep(100000);
    //濡傛灉鏄?Esc'灏卞叧闂獥鍙?
    if (key == ESCAPE)
    {
        glutDestroyWindow(window);
        //姝ｅ父缁撴潫绋嬪簭
        exit(0);
    }
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    //鍒濆鍖栧睆骞曞ぇ灏?
    glutInitWindowSize(640, 480);
    //灞忓箷浠庡乏涓婅寮€濮?
    glutInitWindowPosition(0, 0);
    //鎵撳紑灞忓箷
    window = glutCreateWindow("STICK PERSON");
    //璁剧疆鐢诲浘鍑芥暟
    glutDisplayFunc(&DrawGLScene);
    //鍏ㄥ睆
    glutFullScreen();
    //鍏ㄥ睆涔嬪悗閲嶆柊鐢诲浘
    glutIdleFunc(&DrawGLScene);
    //璁剧疆鍏ㄥ睆鍚庡ぇ灏忓彉鎹㈠嚱鏁?
    glutReshapeFunc(&ReSizeGLScene);
    //璁剧疆鐩戝惉閿洏鍔熻兘
    glutKeyboardFunc(&keyPressed);
    //鍒濆鍖栧睆骞?
    InitGL(640, 480);
    //寮€濮嬪惊鐜墽琛?
    glutMainLoop();
    return 1;
}
