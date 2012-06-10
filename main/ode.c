#include <GL/glut.h>
#include <stdio.h>
#include "ode.h"

int mouse_down = 0;
int active = 1;

int width = 640;
int height = 480;

float xrot = 0.0f;
float yrot = 0.0f;
float xangle = 0.0f;
float yangle = 0.0f;
float mouse_x = 0.0f;
float mouse_y = 0.0f;

plist *list = NULL;

float scale = 1.5f;
float shift = 0.05f;
float scaling = 1.0f;
plist translation = {.x = 0.0f, .y = 0.0f, .z = 0.0f};

void savePPM(unsigned char *frame)
{
    FILE *f = fopen("image.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    int i,j;
    for(i = height-1; i >= 0; i--)
        for(j = 0; j < width; j++)
            fwrite(&frame[(i*width+j)*3], sizeof(unsigned char), 3, f);
    fclose(f);
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
        glColor3d(color[0],color[1],color[2]);
        glVertex3d(trail->x,trail->y,trail->z);
        trail = trail->next;
    }
    glEnd();
}

int init()
{
    plist_add_front(&list,-1,0,0.5,0);

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
    0.0f, 0.0f, -3.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);

    glScalef(scaling,scaling,scaling);
    glTranslatef(translation.x,translation.y,translation.z);
    glRotatef(yangle + yrot, 1.0f, 0.0f, 0.0f);
    glRotatef(xangle + xrot, 0.0f, 1.0f, 0.0f);

    double color[3] = {0, 0.134, 1};
    drawLine(list, color);

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
    if(active)
        plist_evolve_ode(&list, NULL, ODE_CHUA, 1e-3);

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
        case 'q': case 'Q': case 27:
            plist_erase(&list);
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
            saveData(list);
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
    if (!mouse_down && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouse_down = 1;
        mouse_x = x;
        mouse_y = y;

    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        mouse_down = 0;
        xangle += xrot;
        yangle += yrot;
        xrot = 0.0;
        yrot = 0.0;
    }
}

void motion(int x, int y)
{
    if (mouse_down) {
        xrot = (x-mouse_x)*45.0/500;
        yrot = (y-mouse_y)*45.0/500;
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
    glutMotionFunc(motion);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);

    if (!init())
        return 1;

    glutMainLoop();

    return 0;
}
