#include <GL/glut.h>
#include <stdio.h>
#include "ode.h"

int mouseDown = 0;
int active = 1;
int draw_shadow = 0;

int width = 640;
int height = 480;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

plist *list = NULL;
plist *shadow = NULL;

float scale = 1.5f;
float shift = 0.05f;
float scaling = 1.0f;
plist translation = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
plist haxis = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
plist vaxis = {.x = 0.0f, .y = 1.0f, .z = 0.0f};

void savePPM(unsigned char *frame)
{
    FILE *f = fopen("ode.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    int i,j;
    for(i = height-1; i >= 0; i--)
        for(j = 0; j < width; j++)
            fwrite(&frame[(i*width+j)*3], sizeof(unsigned char), 3, f);
    fclose(f);
}

void saveData(plist *trail,plist *shadow)
{
    FILE *f = fopen("ode.dat", "w");
    while(trail != NULL && trail->next != NULL && shadow != NULL){
        fprintf(f, "%14.10e\t%14.10e\t%14.10e\t%14.10e\t%14.10e\t%14.10e\t%14.10e\n", trail->x, trail->y, trail->z, shadow->x, shadow->y, shadow->z, shadow->t);
        trail = trail->next->next;
        shadow = shadow->next;
    }
    fclose(f);
}

void drawLine(plist *trail, double *color)
{
    glBegin(GL_LINE_STRIP);
    while(trail != NULL){
        glColor3d(color[0],color[1],color[2]);
        glVertex3d(trail->x,trail->y,trail->z);
        trail = trail->next;
    }
    glEnd();
}

int init()
{
    plist_add_front(&list,-1.0f,0.0f,0.5f,0.0f);
    plist_add_front(&shadow,-1.0f,0.0f,0.5f,0.0f);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);

    return 1;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
    0.0f, 0.0f, 3.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);

    glScalef(scaling,scaling,scaling);
    glTranslatef(translation.x,translation.y,translation.z);
    glRotatef(xrot, haxis.x, haxis.y, haxis.z);
    glRotatef(yrot, vaxis.x, vaxis.y, vaxis.z);

    double color[3] = {0.0f, 0.134f, 1.0f};
    drawLine(list, color);
    if(draw_shadow){
        color[0] = 1.0f;
        color[1] = 0.134f;
        color[2] = 0.0f;
        drawLine(shadow, color);
    }
    glFlush();
    glutSwapBuffers();
}

void resize(int w, int h)
{
    width = w;
    height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(45.0f, 1.0f * w / h, 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idle()
{
    if(active){
        plist_evolve_ode(&list, NULL, ODE_CHUA, 1e-3);
        plist_evolve_ode(&list, NULL, ODE_CHUA, 1e-3);
        plist_evolve_ode(&shadow, NULL, ODE_CHUA, 2e-3);
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '+':
            shift /= scale;
            scaling *= scale;
            break;
        case '-':
            shift *= scale;
            scaling /= scale;
            break;
        case 'p': case 'P': case ' ':
            active = !active;
            break;
        case 's': case 'S':
            draw_shadow = !draw_shadow;
            break;
        case 'q': case 'Q': case 27:
            plist_erase(&list);
            plist_erase(&shadow);
            exit(0);
            break;
    }
}

void specialKeyboard(int key, int x, int y)
{
    unsigned char *frame = NULL;
    switch(key)
    {
        case GLUT_KEY_F1:
            frame = (unsigned char*)malloc(3*width*height*sizeof(unsigned char));
            glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,frame);
            savePPM(frame);
            free(frame);
            break;
        case GLUT_KEY_F2:
            saveData(list,shadow);
            break;
        case GLUT_KEY_UP:
            translation.y -= shift;
            break;
        case GLUT_KEY_DOWN:
            translation.y += shift;
            break;
        case GLUT_KEY_LEFT:
            translation.x += shift;
            break;
        case GLUT_KEY_RIGHT:
            translation.x -= shift;
            break;
        case GLUT_KEY_PAGE_UP:
            translation.z -= shift;
            break;
        case GLUT_KEY_PAGE_DOWN:
            translation.z += shift;
            break;
    }
}

plist getPosition(int x, int y)
{
    plist tmp;
    tmp.x = scaling * ((x*2.0/width-1)*width)/height + translation.x;
    tmp.y = scaling * (y*2.0/height-1) + translation.y;
    tmp.z = translation.z;
    tmp.t = 0;
    return tmp;
}

void mouse(int button, int state, int x, int y)
{
    if ((mouseDown = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)))
    {
        xdiff = x - yrot;
        ydiff = -y + xrot;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        plist tmp = getPosition(x,height-y);
        plist_erase(&list);
        plist_erase(&shadow);
        plist_add_front(&list, tmp.x, tmp.y, tmp.z, tmp.t);
        plist_add_front(&shadow, tmp.x, tmp.y, tmp.z, tmp.t);
    }
}

void mouseMotion(int x, int y)
{
    if (mouseDown)
    {
        yrot = x - xdiff;
        xrot = y + ydiff;

        glutPostRedisplay();
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(width, height);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("ODE");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);

    if (!init())
        return 1;

    glutMainLoop();

    return 0;
}
