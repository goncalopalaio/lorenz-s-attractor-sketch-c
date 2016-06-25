#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef __APPLE__
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGl/glu.h>
#else
    #include <GL/glut.h>
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/*
*
* Lorenz Attractor
*
*/
#define MAX_LORENZ 10000
#define MAX_COLORS 300
float w = 800;
float h = 400;
typedef struct Observer{
	float x,y,z;
	float lx,ly,lz;
	float ux,uy,uz;
	float fov;
}Observer;
typedef struct Box{
	float maxx,minx,maxy,miny,maxz,minz;
}Box;
Observer ob;
Box box;

int ind=0;
typedef struct Point{
	float x,y,z;
	void init(float xx,float yy,float zz);
	void div(float val);
}Point;
void Point::init(float xx,float yy,float zz){
	x=xx;
	y=yy;
	z=zz;
}
void Point::div(float val){
	x/=val;
	y/=val;
	z/=val;	
}
Point* arr;
Point* colors;
float sigma = 10;
float ro = 28;
float beta = 8/3;
float dt = 0.01;

float dx,dy,dz;
float px,py,pz;
float obRadius=60;
float obAngle = 0;
float obSpeed = 0.01;


void updateBox(float x,float y,float z){
	box.minx = MIN(x,box.minx);
	box.miny = MIN(y,box.miny);
	box.minz = MIN(z,box.minz);
	box.maxx = MAX(x,box.maxx);
	box.maxy = MAX(y,box.maxy);
	box.maxz = MAX(z,box.maxz);

	float cx =(box.maxx -box.minx)/2;
	float cy =(box.maxy -box.miny)/2;
	float cz =(box.maxz -box.minz)/2;
	
	ob.lx = cx;
	ob.ly = cy;
	ob.lz = cz;

	#ifdef DEBUG
	 glPushMatrix();
        glColor3f(1,0,0); //draw x red
        glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(ob.lx,ob.ly,ob.lz);
        glEnd();
    glPopMatrix();
    #endif
}
void mainDisplayLoopCallback(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport (0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(ob.fov,w/h, 0.01, 900);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ob.x,ob.y,ob.z,
			  ob.lx,ob.ly,ob.lz,
			  ob.ux,ob.uy,ob.uz);
	/***********************************/

	
	if(ind<MAX_LORENZ){
		px = arr[ind-1].x;
		py = arr[ind-1].y;
		pz = arr[ind-1].z;
		//lorenz attractor
		dx = (sigma*(py-px))*dt;
		dy = (px*(ro-pz) - py)*dt;
		dz = (px*py - beta * pz)*dt;
		arr[ind].x = px+dx;
		arr[ind].y = py+dy;
		arr[ind].z = pz+dz;
		++ind;

		updateBox(px,py,pz);
	}

	glColor3f(1,1,1);
	glPushMatrix();
	//glPointSize(1);
	//glScalef(2,2,2);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for(int i = 0 ;i<ind-1;i++){
		glColor3f(colors[i%MAX_COLORS].x,colors[i%MAX_COLORS].y,colors[i%MAX_COLORS].z);
		glVertex3f(arr[i].x,arr[i].y,arr[i].z);
			
	}
	glEnd();
	glPopMatrix();

	ob.x = ob.lx + obRadius * cos(obAngle);
	ob.z = ob.lz +  obRadius * sin(obAngle);
	obAngle+=obSpeed;

	#ifdef DEBUG
    //draw xyz reference
    glPushMatrix();
        glTranslatef(0,0,0);
        glColor3f(1,0,0); //draw x red
        glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(1000,0,0);
        glEnd();
        glColor3f(0,1,0); //draw y green
        glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,1000,0);
        glEnd();
        glColor3f(0,0,1); //draw z blue
        glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,0,1000);
        glEnd();
    glPopMatrix();
    #endif


	glutPostRedisplay();
	glutSwapBuffers();

}

void keyboardUpCallback(unsigned char key, int x,int y){
	const int ESC_KEY = 27;
	if(key == ESC_KEY){
		free(arr);
		exit(0);
	}
}

float randf(float max){
    return max*((float)rand()/(float)(RAND_MAX)); 
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(w,h);
    glutCreateWindow("windowTitle");
	glutDisplayFunc(mainDisplayLoopCallback);
    glutKeyboardUpFunc(keyboardUpCallback);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    arr = (Point*) malloc(MAX_LORENZ*sizeof(Point));
	colors = (Point*) malloc(MAX_COLORS*sizeof(Point));

	float colorUnit = 255.0;
    

	Point c1,c2,c3,c4,c5,clearColor;

	//http://www.colourlovers.com/palette/4273323/Far_From_Home_III
	clearColor.init(255,249,251);
	c1.init(182,209,159);
	c2.init(230,221,96);
	c3.init(217,123,61);
	c4.init(204,41,71);
	c5.init(153,3,91);

	clearColor.div(colorUnit);
	c1.div(colorUnit);
	c2.div(colorUnit);
	c3.div(colorUnit);
	c4.div(colorUnit);
	c5.div(colorUnit);

    glClearColor(clearColor.x,clearColor.y,clearColor.z,0);

	int colorSection = MAX_COLORS/5;
	for(int i = 0 ;i<MAX_COLORS;i++){
		if(i<colorSection){
			colors[i]=c1;
		}else if(i<(colorSection)*2){
			colors[i]=c2;
		}else if(i<(colorSection)*3){
			colors[i]=c3;
		}
		else if(i<(colorSection)*4){
			colors[i]=c4;
		}
		else {
			colors[i]=c5;
		}
	}
	//starting values
	arr[0].x = 10;
	arr[0].y = 10;
	arr[0].z = 10;
	++ind;
	
	box.maxx=0;
	box.minx=10000;
	box.maxy=0;
	box.miny=10000;
	box.maxz=0;
	box.minz=10000;


	ob.x=0;
	ob.y=0;
	ob.z=obRadius;	  
	ob.lx=0;
	ob.ly=0;
	ob.lz=0;
	ob.ux=0;
	ob.uy=1;
	ob.uz=0;
	ob.fov=90;
	

    glutMainLoop();

	return 0;
}