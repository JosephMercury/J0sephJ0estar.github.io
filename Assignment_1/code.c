#include<windows.h>//一定要加在第一行
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "unistd.h"
#include <stdio.h>
//接收Esc
#define ESCAPE 27

/* The number of our GLUT window */
int window;

//定义画图中的各个参数
float angleOfLeftArmA,angleOfLeftArmB,angleOfRightArmA,angleOfRightArmB,angleOfLeftLegA,angleOfLeftLegB,angleOfRightLegA,angleOfRightLegB,a = 0.4,b=0.2,angle = 0.1;
GLuint texture[1];
//openGL初始化程序
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

//画身体的各个部分的函数
//openGL左边系为右手系且y轴朝上，z轴从屏幕指出
//一次画两边，因为是根据y轴对称所以每对都是y上坐标不变
void DrawBody()
{
    glColor3f(0.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    //画朝前的面
    glVertex3f(-0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f,-1.0f, 0.25f);
    glVertex3f(-0.5f,-1.0f, 0.25f);
    //画朝左的面
    glVertex3f( 0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f, 1.0f,-0.25f);
    glVertex3f( 0.5f,-1.0f,-0.25f);
    glVertex3f( 0.5f,-1.0f, 0.25f);
    //画朝后的面
    glVertex3f( 0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f,-0.25f);
    glVertex3f( 0.5f,-1.0f,-0.25f);
    //画朝右的面
    glVertex3f(-0.5f, 1.0f, 0.25f);
    glVertex3f(-0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f, 0.25f);
    //画朝上的面
    glVertex3f( 0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f, 1.0f, 0.25f);
    //画朝下的面
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
    //画朝前的面
    glVertex3f(-1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f,-0.2f, 0.25f);
    glVertex3f(-1.0f,-0.2f, 0.25f);
    //画朝左的面
    glVertex3f( 1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f, 0.2f,-0.25f);
    glVertex3f( 1.0f,-0.2f,-0.25f);
    glVertex3f( 1.0f,-0.2f, 0.25f);
    //画朝后的面
    glVertex3f( 1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f,-0.25f);
    glVertex3f( 1.0f,-0.2f,-0.25f);
    //画朝右的面
    glVertex3f(-1.0f, 0.2f, 0.25f);
    glVertex3f(-1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f, 0.25f);
    //画朝上的面
    glVertex3f( 1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f, 0.2f, 0.25f);
    //画朝下的面
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
    //画朝前的面
    glVertex3f(-0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f,-0.2f, 0.25f);
    glVertex3f(-0.7f,-0.2f, 0.25f);
    //画朝左的面
    glVertex3f( 0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f, 0.2f,-0.25f);
    glVertex3f( 0.7f,-0.2f,-0.25f);
    glVertex3f( 0.7f,-0.2f, 0.25f);
    //画朝后的面
    glVertex3f( 0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f,-0.25f);
    glVertex3f( 0.7f,-0.2f,-0.25f);
    //画朝右的面
    glVertex3f(-0.7f, 0.2f, 0.25f);
    glVertex3f(-0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f, 0.25f);
    //画朝上的面
    glVertex3f( 0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f, 0.2f, 0.25f);
    //画朝下的面
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

    //画朝前的面
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f,-1.0f, 0.25f);
    glVertex3f(-0.2f,-1.0f, 0.25f);
    //画朝左的面
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
    glVertex3f( 0.2f,-1.0f, 0.25f);
    //画朝后的面
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
    //画朝右的面
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
    glVertex3f(-0.2f,-1.0f, 0.25f);
    //画朝上的面
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f, 0.25f);
    //画朝下的面
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

    //画朝前的面
    glVertex3f(-0.2f, -0.56f, 0.21f);
    glVertex3f( 0.2f, -0.56f, 0.21f);
    glVertex3f( 0.2f,-2.0f, 0.21f);
    glVertex3f(-0.2f,-2.0f, 0.21f);
    //画朝左的面
    glVertex3f( 0.2f, -0.56f, 0.21f);
    glVertex3f( 0.2f, -0.56f,-0.21f);
    glVertex3f( 0.2f,-2.0f,-0.21f);
    glVertex3f( 0.2f,-2.0f, 0.21f);
    //画朝后的面
    glVertex3f( 0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f,-2.0f,-0.21f);
    glVertex3f( 0.2f,-2.0f,-0.21f);
    //画朝右的面
    glVertex3f(-0.2f, -0.56f, 0.21f);
    glVertex3f(-0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f,-2.0f,-0.21f);
    glVertex3f(-0.2f,-2.0f, 0.21f);
    //画朝上的面
    glVertex3f( 0.2f, -0.56f, 0.21f);
    glVertex3f( 0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f, -0.56f,-0.21f);
    glVertex3f(-0.2f, -0.56f, 0.21f);
    //画朝下的面
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
    //画朝前的面
    glVertex3f(-0.3f,0.25f, 0.255f);
    glVertex3f( 0.3f,0.25f, 0.255f);
    glVertex3f( 0.3f,-1.5f, 0.255f);
    glVertex3f(-0.3f,-1.5f, 0.255f);
    //画朝左的面
    glVertex3f( 0.3f,0.25f, 0.255f);
    glVertex3f( 0.3f,0.25f,-0.255f);
    glVertex3f( 0.3f,-1.5f,-0.255f);
    glVertex3f( 0.3f,-1.5f, 0.255f);
    //画朝后的面
    glVertex3f( 0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,-1.5f,-0.255f);
    glVertex3f( 0.3f,-1.5f,-0.255f);
    //画朝右的面
    glVertex3f(-0.3f,0.25f, 0.255f);
    glVertex3f(-0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,-1.5f,-0.255f);
    glVertex3f(-0.3f,-1.5f, 0.255f);
    //画朝上的面
    glVertex3f( 0.3f,0.25f, 0.255f);
    glVertex3f( 0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,0.25f,-0.255f);
    glVertex3f(-0.3f,0.25f, 0.255f);
    //画朝下的面
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
    //画朝前的面
    glVertex3f(-0.3f,-1.2f, 0.255f);
    glVertex3f( 0.3f,-1.2f, 0.255f);
    glVertex3f( 0.3f,-3.25f, 0.255f);
    glVertex3f(-0.3f,-3.25f, 0.255f);
    //画朝左的面
    glVertex3f( 0.3f,-1.2f, 0.255f);
    glVertex3f( 0.3f,-1.2f,-0.255f);
    glVertex3f( 0.3f,-3.25f,-0.255f);
    glVertex3f( 0.3f,-3.25f, 0.255f);
    //画朝后的面
    glVertex3f( 0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-3.25f,-0.255f);
    glVertex3f( 0.3f,-3.25f,-0.255f);
    //画朝右的面
    glVertex3f(-0.3f,-1.2f, 0.255f);
    glVertex3f(-0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-3.25f,-0.255f);
    glVertex3f(-0.3f,-3.25f, 0.255f);
    //画朝上的面
    glVertex3f( 0.3f,-1.2f, 0.255f);
    glVertex3f( 0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-1.2f,-0.255f);
    glVertex3f(-0.3f,-1.2f, 0.255f);
    //画朝下的面
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

    //画朝前的面
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.6f, 0.6f, 0.35f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 0.6f, 0.6f, 0.35f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 0.6f,-0.6f, 0.35f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.6f,-0.6f, 0.35f);
    //画朝左的面
    glVertex3f( 0.6f, 0.6f, 0.35f);
    glVertex3f( 0.6f, 0.6f,-0.35f);
    glVertex3f( 0.6f,-0.6f,-0.35f);
    glVertex3f( 0.6f,-0.6f, 0.35f);
    //画朝后的面
    glVertex3f( 0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f,-0.35f);
    glVertex3f( 0.6f,-0.6f,-0.35f);
    //画朝右的面
    glVertex3f(-0.6f, 0.6f, 0.35f);
    glVertex3f(-0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f, 0.35f);
    //画朝上的面
    glVertex3f( 0.6f, 0.6f, 0.35f);
    glVertex3f( 0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f, 0.6f, 0.35f);
    //画朝下的面
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

    //画朝前的面
    glVertex3f(-0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f,-0.1f, 0.25f);
    glVertex3f(-0.2f,-0.1f, 0.25f);
    //画朝左的面
    glVertex3f( 0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f, 0.1f,-0.25f);
    glVertex3f( 0.2f,-0.1f,-0.25f);
    glVertex3f( 0.2f,-0.1f, 0.25f);
    //画朝后的面
    glVertex3f( 0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f,-0.25f);
    glVertex3f( 0.2f,-0.1f,-0.25f);
    //画朝右的面
    glVertex3f(-0.2f, 0.1f, 0.25f);
    glVertex3f(-0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f, 0.25f);
    //画朝上的面
    glVertex3f( 0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f, 0.1f, 0.25f);
    //画朝下的面
    glVertex3f( 0.2f,-0.1f, 0.25f);
    glVertex3f( 0.2f,-0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f, 0.25f);

    glEnd();
}
//主画图程序
void DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();//当前矩阵重置为单位矩阵
    //一开始从右后方出现
    glTranslatef(0.0f,0.0f,-40.0f);//将当前图像向z轴负方向移动
    glRotatef(angle,0,1,0);//绕y轴不停旋转，造成移动的假象
    glPushMatrix();//建立局部坐标系
    glTranslatef(0.0f,0.0f,-20.0f);
    glRotatef(-90,0,1,0);//保持脸正常朝向
    glPushMatrix();

    glTranslatef(0.0f,1.2f,0.0f);//沿y轴正方向
    glPushMatrix();
    glTranslatef(0.8,0.0,0.0);
    glTranslatef(0.0,-0.4,0.0);
    glRotatef(angleOfLeftArmA,-1,0,0);//胳膊不断前后移动
    //画胳膊的两段
    DrawArmA();
    glPushMatrix();
    glTranslatef(0.0,-0.37,0);
    glPushMatrix();
    glRotatef(angleOfLeftArmB,-1,0,0);
    DrawArmB();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();//恢复全局坐标系

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
    //返回肩膀
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
    //返回身体
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
    //左右胳膊运动幅度保持一致
    angleOfLeftArmA+=a;
    angleOfRightArmA-=a;

    //在主胳膊在身体上方时小胳膊的运动情况
    if(angleOfLeftArmA>0)
    {
        //如果在向上运动的话
        if(a>0)
        {
            //小胳膊会上摆一定角度后保持
            if(angleOfLeftArmB<45)
                angleOfLeftArmB+=a;
            else angleOfLeftArmB=45;
        }
        else//在向下运动的话小胳膊会下摆直至与主胳膊平齐
            if(angleOfLeftArmB>0)
                angleOfLeftArmB+=a;
    }
    else//身体下方时与主胳膊运动情况相同
        angleOfLeftArmB=0;

    //对右胳膊同理，不过a<0时代表向上运动
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

    //左右腿运动幅度保持一致且与胳膊相对相反
    angleOfLeftLegA-=b;
    angleOfRightLegA+=b;

    //在身体后方时小腿微微屈腿
    if(angleOfRightLegA<0)
        angleOfRightLegB=-15;
    else
        angleOfRightLegB=0;
    if(angleOfLeftLegA<0)
        angleOfLeftLegB=-15;
    else
        angleOfLeftLegB=0;

    angle+=0.1;//整体绕y轴转的速度

    //若运动到了最大幅度时，开始反向运动
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

//接收到按键时的响应程序
void keyPressed(unsigned char key, int x, int y)
{
    //短暂休眠，可以更加仔细观察运动情况
    usleep(100000);
    //如果是'Esc'就关闭窗口
    if (key == ESCAPE)
    {
        glutDestroyWindow(window);
        //正常结束程序
        exit(0);
    }
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    //初始化屏幕大小
    glutInitWindowSize(640, 480);
    //屏幕从左上角开始
    glutInitWindowPosition(0, 0);
    //打开屏幕
    window = glutCreateWindow("STICK PERSON");
    //设置画图函数
    glutDisplayFunc(&DrawGLScene);
    //全屏
    glutFullScreen();
    //全屏之后重新画图
    glutIdleFunc(&DrawGLScene);
    //设置全屏后大小变换函数
    glutReshapeFunc(&ReSizeGLScene);
    //设置监听键盘功能
    glutKeyboardFunc(&keyPressed);
    //初始化屏幕
    InitGL(640, 480);
    //开始循环执行
    glutMainLoop();
    return 1;
}
