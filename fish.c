#include<stdio.h>
#include<GL/glut.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#define pi 3.141592654
#define radians(i) pi*i/180.0
#define NUM_FISH 2
double cosines[360], sines[360], W=800, H=600;
GLfloat star[20][2] = {{100,600},{195,620},{290,640},{355, 800}, {420, 750}, {540,600}, {620, 810}, {750, 750}, {800, 720}, {820, 900}, {800, 600}, {860, 740}, {720, 680}, {800, 800}, {850, 780}, {930, 640}, {150, 700}, {240, 750}, {480, 650}, {650, 800}};
bool starShine[20] = {true}, gameOver = false;
int x = 0, score = 0, life = 3, right = 0;
float xpos = 0, ypos = 0, sPos = -20, bpos = 0, dy = 0.2;
void myInit(){
	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100,100,-100,100,-1,1);
	int i;
	for(i=0;i<360;i++)
		cosines[i] = cos(radians(i)), sines[i] = sin(radians(i));
	srand(time(0));
	ypos = rand()%80-40;
}
void drawFish1(){
	int i;
	glColor4f(0.1,0.1,0.1,1);
	glBegin(GL_POLYGON);
		for(i=0;i<360;i++)
			glVertex3f(1*cosines[i]-9,1*sines[i]+3,0.3);
	glEnd();
	glColor4f(0.1,0.6,0,1);
	glBegin(GL_POLYGON);
		for(i=0;i<360;i++)
			glVertex3f(2.5*cosines[i]-8,2.5*sines[i]+3,0.3);
	glEnd();
	glColor4f(1,0,0,0.8);
	glBegin(GL_POLYGON);
		for(i = 50; i<160; i++)
			glVertex3f(20*cosines[i],10*sines[i],0.2);
		glVertex3f(15*cosines[180],10*sines[180],0.2);
		for(i = 200; i<310; i++)
			glVertex3f(20*cosines[i],10*sines[i],0.2);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f(20*cosines[50],10*sines[50],0.2);
		glVertex3f(20,12,0.2);
		glVertex3f(15,0,0.2);
		glVertex3f(20,-12,0.2);
		glVertex3f(20*cosines[309],10*sines[309],0.2);
	glEnd();
}
void drawFish2(){
	int i;
	float j;
	glLineWidth(0.2);
	glColor3f(0,0,0);
	for(i=30;i<150;i+=20){
		glBegin(GL_LINE_STRIP);
			for(j=20*sines[i];j>=20*sines[360-i];j-=0.1)
				glVertex3f(20*cosines[i]+((int)(j*10)%15)/7,j,0.2);
		glEnd();
	}
	glColor4f(0.1,0.1,0.1,1);
	glBegin(GL_POLYGON);
		for(i=0;i<360;i++)
			glVertex3f(1*cosines[i]-9,1*sines[i]+3,0.3);
	glEnd();
	glColor4f(0.1,0.6,0,1);
	glBegin(GL_POLYGON);
		for(i=0;i<360;i++)
			glVertex3f(2.5*cosines[i]-8,2.5*sines[i]+3,0.3);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(1,0.6,0.1);
		for(i=30;i<150;i++)
			glVertex3f(20*cosines[i],20*sines[i],0.2);
		glVertex3f(-25,3,0.2);
		glVertex3f(-22,0,0.2);
		glVertex3f(-25,-3,0.2);
		for(i=210;i<330;i++)
			glVertex3f(20*cosines[i],20*sines[i],0.2);
		glVertex3f(30,-12,0.2);
		glVertex3f(20,0,0.2);
		glVertex3f(30,15,0.2);
	glEnd();
}
void (*fish[])()={drawFish1,drawFish2};
void drawWater(){
	glColor3f(0,0,1);
	glRectf(-100,-100,100,0);
}
void drawMan(){
	int i;
	glColor3f(0,1,0);
	glBegin(GL_POLYGON);
		for(i=0;i<360;i++)
			glVertex3f(5*cosines[i]-10,5*sines[i]+35,0.2);
	glEnd();
	glRecti(-15,20,-5,30);
	if(!right)
		glRecti(-15,24,-20,26);
	else
		glRecti(-5,24,0,26);
}
void drawTarget(){
	glColor3f(0,0,0);
	int i;
	glBegin(GL_LINE_LOOP);
		for(i=0;i<360;i++)
			glVertex3f(2.5*cosines[i]-35,2.5*sines[i]+sPos,0.5);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(-31,sPos,0.5);
		glVertex3f(-33,sPos,0.5);
		glVertex3f(-35,2+sPos,0.5);
		glVertex3f(-35,4+sPos,0.5);
		glVertex3f(-37,sPos,0.5);
		glVertex3f(-39,sPos,0.5);
		glVertex3f(-35,-2+sPos,0.5);
		glVertex3f(-35,-4+sPos,0.5);
	glEnd();
}
void drawBoat(){
	int i;
	glBegin(GL_QUADS);
		glColor3f(0.1,0.1,0.1);
		glVertex3f(-20,0,0.2);
		glColor3f(1,0,1);
		glVertex3f(20,0,0.2);
		glColor3f(0,1,1);
		glVertex3f(30,20,0.2); 
		glColor3f(1,0,1);
		glVertex3f(-30,20,0.2);
	glEnd();
	glBegin(GL_TRIANGLES);
		glColor3f(1,0,0);
		glVertex3f(0,30,0.2);
		glColor3f(0,1,0);
		glVertex3f(20,30,0.2);
		glColor3f(0,0,1);
		glVertex3f(0,50,0.2);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(0,1,0);
		glVertex3f(0,20,0.2);
		glVertex3f(0,50,0.2);
	glEnd();
	glPushMatrix();
	if(right)
		glTranslatef(right,0,0);
	drawTarget();
	glBegin(GL_LINES);
		glLineWidth(15.0);
		glColor3f(0,1,0);
		if(!right){
			glVertex3f(-20,25,0.2);
			glVertex3f(-35,17,0.2);
		}
		else{
			glVertex3f(-50,25,0.2);
			glVertex3f(-35,17,0.2);
		}
		glVertex3f(-35,17,0.2);
		glVertex3f(-35,sPos,0.2);
	glEnd();
	glBegin(GL_POLYGON);
		for(i=0;i<360;i++)
			glVertex3f(0.5*cosines[i]-35,0.5*sines[i]+17,0.3);
	glEnd();
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_POLYGON);
		for(i=0;i<360;i++)
			glVertex3f(2*cosines[i]-35,2*sines[i]+17,0.3);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	if(right)
		glTranslatef(right-45,0,0);
	drawMan();
	glPopMatrix();
}
void drawStars(){
	glTranslatef(-100,-100,0);
	glScaled(0.2,0.2,1);
	int i;
	for(i = 0; i<20; i++){
		glBegin(GL_POINTS);
			if(starShine[i])
				glVertex3f(star[i][0],star[i][1]+100,0);
		glEnd();
	}
}
bool form = true, carrot = false;
char name[50], name1[50], name2[50], hName[50], hScore[50];
float  c = 1, wy = 75, wx = 20, dwy = 0.1, dwx = 0.1;
void renderBitmapString(float x,float y,float z,void* font,const char*string)
{
	int i;
	glRasterPos3f(x, y,z);
	for(i = 0; string[i] != '\0'; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}
void renderStrokeString(float x, float y, float z, float sx, float sy, float sz, const char* str){
	glPushMatrix();
	glTranslatef(x,y,z);
	glScaled(sx,sy,sz);
	int i;
	for(i = 0; str[i]; i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	glPopMatrix();
}
void drawButton(double x, double y, double dx, double dy, const char* str){
	double x2 = x + dx, y2 = y + dy, x3 = x+dx/10, y3 = y+dy/10, x4 = x+dx-dx/10, y4 = y+dy-dy/10;
	glColor4f(1,0,0,1);
	renderStrokeString(x+dx/3,y+dy/3,0,dy/300,dy/300,1,str);
	glColor4f(0,0,c,1);
	glPushMatrix();
	glTranslatef(0,0,-50);
	glRectd(x,y,x2,y2);
	glPopMatrix();
}
void drawTextArea(double x, double y, double dx, double dy, const char* str1, const char* str2){
	glPushMatrix();
	glTranslatef(0,0,-50);
	glColor3f(0.5,0.5,0.5);
	glRectd(x,y,x+dx,y+dy);
	glPopMatrix();
	glColor3f(1,0,0);
	renderBitmapString(x+dx/10,y+dy/5,0,GLUT_BITMAP_8_BY_13,str1);
	if(carrot)
		renderBitmapString(x+dx/10+strlen(str1)*800/W,y+dy/5,0,GLUT_BITMAP_8_BY_13,"|");
	renderBitmapString(x+dx/10+(strlen(str1)+1)*800/W,y+dy/5,0,GLUT_BITMAP_8_BY_13,str2);
}
void myReshape(int w, int h){
	glViewport(0,0,w,h);
	W=w, H=h;
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	if(form){
		glColor3f(1,0,0);
		renderStrokeString(18,95,0,0.03,0.03,1,"Visvesvaraya Technological University");
		renderStrokeString(45,90,0,0.015,0.015,1,"Belgaum");
		renderStrokeString(22,85,0,0.03,0.03,1,"H.K.B.K. College of Engineering");
		renderStrokeString(wx,wy,0,0.025,0.025,1,"Hello All!!");
		renderStrokeString(32,70,0,0.02,0.02,1,"This is a project in CGV");
		renderStrokeString(47,65,0,0.015,0.015,1,"On");
		renderStrokeString(42,60,0,0.02,0.02,1,"Fish Game");
		renderStrokeString(47,50,0,0.015,0.015,1,"By");
		renderStrokeString(30,45,0,0.03,0.03,1,"USN");
		renderStrokeString(55,45,0,0.03,0.03,1,"Name");
		renderStrokeString(30,40,0,0.02,0.02,1,"1hk14cs016");
		renderStrokeString(55,40,0,0.02,0.02,1,"Athik F.");
		renderStrokeString(30,35,0,0.02,0.02,1,"1hk14cs025");
		renderStrokeString(55,35,0,0.02,0.02,1,"Irfan Basha");
		renderStrokeString(37,25,0,0.02,0.02,1,"Under guidance of");
		renderStrokeString(30,20,0,0.03,0.03,1,"Prof. Tabassum Ara");
		renderStrokeString(10,10,0,0.02,0.02,1,"Enter name : ");
		drawButton(80,10,10,5,"Play");
		drawTextArea(30,10,40,5,name1,name2);
	}
	else if(gameOver){
		glColor3f(1.0,0.0,0.0);
		renderBitmapString(42,70,0,GLUT_BITMAP_HELVETICA_18,"GAME OVER!!!");
		glColor3f(1.0,1.0,1.0);
		renderBitmapString(25,58,0,GLUT_BITMAP_TIMES_ROMAN_24,"You");
		renderBitmapString(45,58,0,GLUT_BITMAP_TIMES_ROMAN_24,"scored:");
		char temp[20];
		sprintf(temp,"%d",score);
		renderBitmapString(70,58,0,GLUT_BITMAP_TIMES_ROMAN_24,temp);
	}
	else{
		glPushMatrix();
		glTranslatef(xpos+100,ypos-50,0);
		fish[x]();
		glPopMatrix();
		drawWater();
		glPushMatrix();
		glTranslatef(bpos,0,0);
		drawBoat();
		glPopMatrix();
		glPushMatrix();
		glColor3f(0.7,0.7,0.7);
		glTranslatef(70,70,10);
		glutSolidSphere(10,100,100);
		glPopMatrix();
		glPushMatrix();
		drawStars();
		glPopMatrix();
		renderStrokeString(70,90,0,0.02,0.02,1,"Score : ");
		char scs[10];
		sprintf(scs,"%d",score);
		renderStrokeString(80,90,0,0.02,0.02,1,scs);
		renderStrokeString(50,90,0,0.02,0.02,1,"Life : ");
		sprintf(scs,"%d",life);
		renderStrokeString(60,90,0,0.02,0.02,1,scs);
	}
	glFlush();
	glutSwapBuffers();
}
void myTimer(int value){
	if(form){
		if(carrot)value --;
		else value++;
		if(value<0)carrot = false;
		else if(value>20) carrot = true;
		wx += dwx, wy += dwy;
		if(wx>60)dwx = -0.1;
		else if(wx<20)dwx = 0.1;
		if(wy>80)dwy = -0.1;
		else if(wy<75)dwy = 0.1;
		glutPostRedisplay();
		glutTimerFunc(20,myTimer,value);
	}
	else{
		xpos -= 0.5;
		if(!((int)(-xpos*2)%16))dy*=-1;
		ypos+=dy;
		if(xpos<-200)life--, xpos = 0, x = rand()%NUM_FISH, ypos = rand()%80-40;
		if(life<0){
			gameOver = true;
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0,100,0,100,-100,100);
			glDisable(GL_BLEND);
			if(score>atoi(hScore)){
				FILE* fp = fopen("highscores.txt","w");
				char scs[10];
				sprintf(scs,"%d",score);
				fprintf(fp,"%s %s",name,scs);
				fclose(fp);
				glutPostRedisplay();
				return;
			}
		}
		int i;
		for(i=0;i<20;i++)starShine[i]=true;
		for(i=0;i<2;i++)starShine[rand()%20]=false;
		glutPostRedisplay();
		glutTimerFunc(20,myTimer,0);
	}
}
void myMouse(int x, int y){
	double X, Y;
	X = x/W*100, Y = (H-y)/H*100;
	if(form){
		if(X>80&&X<90&&Y>10&&Y<15){
			c = 0.8;
		}
		else{
			c = 1;
		}
		glutPostRedisplay();
	}
	else
	{
		Y = (H-y)/H*200 - 100;
		if(Y<-20)sPos=Y;
		else sPos = -20;
		glutPostRedisplay();
	}
}
void keys(unsigned char key,int x,int y)
{
	if(form){
		if(isalpha(key)||key==' '){name1[strlen(name1)+1] = '\0', name1[strlen(name1)] = key;}
		else if(key == 13)
		{
			form = false; myInit();
		}
		else if(key == 8){
			name1[strlen(name1)-1]='\0';
		}
		sprintf(name,"%s%s",name1,name2);
		glutPostRedisplay();
	}
	else if(gameOver){
		exit(0);
	}
	else{
		if(key==' ')
			right = right == 70?0:70;
	}
}
void mouse(int button, int state, int x, int y)
{
	if(form){
		double X = x/W*100, Y = (H-y)/H*100;
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			if(c < 1) {form = false; myInit();}
			else if(X>34&X<66&&Y>10&&Y<15){
				double t = strlen(name1)*800/W - (int)X*800/W+35*800/W;
				t = strcmp(name2,"")==0?34:(t<0)?35:34;
				sprintf(name2,"%s",name +(int)((X-t)*W/800));
				name[(int)((X-t)*W/800)]='\0';
				strcpy(name1,name);
				sprintf(name,"%s%s",name1,name2);
			}
			glutPostRedisplay();
		}
		else{
			if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
				if(fabs(sPos-(ypos-50.0))<5.0&& fabs((bpos+right-35.0)-(xpos+100.0))<5.0){
					xpos= 0, x = rand()%NUM_FISH, score += 20, ypos = rand()%80-40;
				}
		}
	}
}
void special(int key, int x, int y){
	char temp[50];
	if(form){
		switch(key){
			case GLUT_KEY_LEFT:
				strcpy(temp,name1+strlen(name1)-1);
				strcat(temp,name2);
				strcpy(name2,temp);
				name1[strlen(name1)-1]='\0';
				sprintf(name,"%s%s",name1,name2);
				break;
			case GLUT_KEY_RIGHT:
				if(strcmp(name2,"")!=0){
					name1[strlen(name1)]=name2[0];
					sprintf(name2,"%s",name2+1);
					name1[strlen(name1)+1]='\0';
					sprintf(name,"%s%s",name1,name2);
				}
				break;
		}
	}
	else
	{
		switch(key){
			case GLUT_KEY_LEFT:if(bpos>-80)bpos-=1;
				break;
			case GLUT_KEY_RIGHT:if(bpos<80)bpos+=1;
				break;
		}
	}
	glutPostRedisplay();
}
void init(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(0.0, 100.0, 0.0, 100.0, -100.0 , 100.0);
	FILE* fp = fopen("highscores.txt","r");
	if(!fp)return;
	fscanf(fp,"%s %s",hName,hScore);
	fclose(fp);
}
void main(int argc, char* argv[]){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(800,600);
	glutCreateWindow("Fish Game");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(myMouse);
	glutSpecialFunc(special);
	glutKeyboardFunc(keys);
	glutReshapeFunc(myReshape);
	init();
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glutTimerFunc(10,myTimer,0);
	glutMainLoop();
}
