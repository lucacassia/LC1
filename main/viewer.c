#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "ode.h"

int active = 1;
int width = 500;
int height = 500;

plist *list = NULL;

float scalef = 1.5;
float ds;
float a;
plist b;

void savePPM(int w, int h)
{
    unsigned char *frame = (unsigned char*) malloc (3 * w * h * sizeof(float));
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, frame);
    FILE *file = fopen("image.ppm", "wb");
    fprintf(file, "P6\n%d %d\n255\n", w, h);
    fwrite(frame, 1, 3 * w * h, file);
    fclose(file);
    free(frame);
}

void saveData(plist *trail)
{
    FILE *f = fopen("image.dat", "w");
    while(trail != NULL){
        fprintf(f, "%lf\t%lf\t%lf\n", trail->x, trail->y, trail->z);
        trail = trail->next;
    }
    fclose(f);
}

void drawLine(plist *trail, double *color)
{
    glBegin(GL_LINE_STRIP);
    while(trail != NULL){
        double factor = 1;
        if(trail->z <= 0)
            factor = 1;
        if(trail->z > 0 && trail->z < 50)
            factor = 1-trail->z/50;
        glColor3d(color[0]*factor,color[1]*factor,color[2]*factor);
        glVertex3d(trail->x,trail->y,trail->z);
        trail = trail->next;
    }
    glEnd();
}

void drawCircle(plist head, double r, double *color)
{
    glColor3d(color[0],color[1],color[2]);
    glBegin(GL_POLYGON);
    int i;
    for(i = 0; i < 360; i++)
        glVertex3d(head.x+r*cos(i*3.14159265/180), head.y+r*sin(i*3.14159265/180), head.z);
    glEnd();
}

void displayF()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    double color[3] = {0, 96/713.0, 1};
    drawLine(list, color);
    drawCircle(*list, 0.02, color);
    glutSwapBuffers();
}

void reshapeF(int w,int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w*a/h+b.x,w*a/h+b.x,-a+b.y,a+b.y,a+b.z,-a+b.z);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,0);
}

void init()
{
    plist_add_front(&list,0,0,0,0);

    active = 1;
    b.x = b.y = b.z = 0;
    ds = 0.05;
    a = 1;

    reshapeF(width,height);
}

void keyboardF(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '+':
            glTranslatef(b.x,b.y,b.z);
            glScalef(scalef,scalef,scalef);
            glTranslatef(-b.x,-b.y,-b.z);
            ds/=scalef;
            a/=scalef;
            break;
        case '-':
            glTranslatef(b.x,b.y,b.z);
            glScalef(1/scalef,1/scalef,1/scalef);
            glTranslatef(-b.x,-b.y,-b.z);
            ds*=scalef;
            a*=scalef;
            break;
        case 'p': case 'P': case ' ':
            active=!active;
            break;
        case 'f': case 'F':
            glutFullScreenToggle();
            break;
        case 'q': case 'Q': case 27:
            exit(0);
            break;
    }
}

void specialKeyboardF(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F1:
            savePPM(width-2,height-2);
            break;
        case GLUT_KEY_F2:
            saveData(list);
            break;
        case GLUT_KEY_F11:
            glutFullScreenToggle();
            break;
        case GLUT_KEY_UP:
            glTranslatef(0,-ds,0);
            b.y+=ds;
            break;
        case GLUT_KEY_DOWN:
            glTranslatef(0,ds,0);
            b.y-=ds;
            break;
        case GLUT_KEY_LEFT:
            glTranslatef(ds,0,0);
            b.x-=ds;
            break;
        case GLUT_KEY_RIGHT:
            glTranslatef(-ds,0,0);
            b.x+=ds;
            break;
        case GLUT_KEY_PAGE_UP:
            glTranslatef(0,0,-ds);
            b.z+=ds;
            break;
        case GLUT_KEY_PAGE_DOWN:
            glTranslatef(0,0,ds);
            b.z-=ds;
            break;
    }
}

plist getPosition(int x, int y)
{
    plist tmp;
    tmp.x = a * ((x*2.0/width-1)*width)/height + b.x;
    tmp.y = a * (y*2.0/height-1) + b.y;
    tmp.z = b.z;
    tmp.t = 0;
    return tmp;
}

void mouseF(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        plist tmp = getPosition(x,height-y);
        plist_erase(&list);
        plist_add_front(&list, tmp.x, tmp.y, tmp.z, tmp.t);
    }
}

void idleF(void)
{
    if(active)
        plist_evolve_ode(&list, NULL);
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("ODE Viewer");

    init();

    glutDisplayFunc(displayF);
    glutIdleFunc(idleF);
    glutKeyboardFunc(keyboardF);
    glutSpecialFunc(specialKeyboardF);
    glutMouseFunc(mouseF);
    glutReshapeFunc(reshapeF);

    glutMainLoop();
    return 0;
}

