#ifndef ODE
#define ODE

#include <math.h>

#define dt (1e-3)

typedef enum { ODE_CHAOTIC, ODE_VANDERPOL, ODE_DUFFING, ODE_LORENZ, ODE_ROSSLER, ODE_RABINOVICH_FABRIKANT, ODE_CHUA } ODE_TYPE;

double dummy(double x,double y, double z,double t) {return 0;}

double chaotic_x(double x,double y, double z,double t) {return y;}
double chaotic_y(double x,double y, double z,double t) {return -sin(x)-0.3*y+1.4*cos(2*t/3);}

double vanderpol_x(double x,double y, double z,double t) {return y;}
double vanderpol_y(double x,double y, double z,double t) {return 4*(1-x*x)*y-x;}

double duffing_x(double x,double y, double z,double t) {return y;}
double duffing_y(double x,double y, double z,double t) {return 0.3*cos(t)-0.2*y+x-pow(x,3);}

double lorenz_x(double x,double y,double z, double t) {return 10*(y-x);}
double lorenz_y(double x,double y,double z, double t) {return x*(28-z)-y;}
double lorenz_z(double x,double y,double z, double t) {return x*y-8*z/3;}

double rossler_x(double x,double y,double z, double t) {return -y-z;}
double rossler_y(double x,double y,double z, double t) {return x+0.1*y;}
double rossler_z(double x,double y,double z, double t) {return 0.1+z*(x-14);}

double rabinovich_fabrikant_x(double x,double y,double z, double t) {return y*(z-1+pow(x,2))+0.87*x;}
double rabinovich_fabrikant_y(double x,double y,double z, double t) {return x*(3*z+1-pow(x,2))+0.87*y;}
double rabinovich_fabrikant_z(double x,double y,double z, double t) {return -2*z*(1.1+x*y);}

double chua_x(double x,double y,double z, double t) {return 9.5*(y+(x-2*pow(x,3))/7);}
double chua_y(double x,double y,double z, double t) {return x-y+z;}
double chua_z(double x,double y,double z, double t) {return -100*y/7;}

typedef struct _plist{
    double x,y,z,t;
    struct _plist *prev;
    struct _plist *next;
}plist;

void plist_add_front(plist **head_ptr, double x, double y, double z, double t){
    plist *tmp = (plist*)malloc(sizeof(plist));
    tmp->x = x;
    tmp->y = y;
    tmp->z = z;
    tmp->t = t;
    tmp->next = *head_ptr;
    tmp->prev = NULL;
    *head_ptr = tmp;
    if(tmp->next != NULL)
        (tmp->next)->prev = tmp;
}

void plist_remove_last(plist **tail_ptr){
    if(*tail_ptr != NULL){
        *tail_ptr = (*tail_ptr)->prev;
        free((*tail_ptr)->next);
        (*tail_ptr)->next = NULL;
    }
}

void plist_erase(plist **head){
    while(*head != NULL){
        plist *tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

void plist_evolve_ode(plist **head_ptr, plist **tail_ptr, ODE_TYPE type){
    double x = 0, y = 0, z = 0, k1, k2, k3, k4;
    double (*function_x)(double,double,double,double) = NULL;
    double (*function_y)(double,double,double,double) = NULL;
    double (*function_z)(double,double,double,double) = NULL;
    switch(type){
        case ODE_CHAOTIC:
            function_x = chaotic_x;
            function_y = chaotic_y;
            function_z = dummy;
            break;
        case ODE_VANDERPOL:
            function_x = vanderpol_x;
            function_y = vanderpol_y;
            function_z = dummy;
            break;
        case ODE_DUFFING:
            function_x = duffing_x;
            function_y = duffing_y;
            function_z = dummy;
            break;
        case ODE_LORENZ:
            function_x = lorenz_x;
            function_y = lorenz_y;
            function_z = lorenz_z;
            break;
        case ODE_ROSSLER:
            function_x = rossler_x;
            function_y = rossler_y;
            function_z = rossler_z;
            break;
        case ODE_RABINOVICH_FABRIKANT:
            function_x = rabinovich_fabrikant_x;
            function_y = rabinovich_fabrikant_y;
            function_z = rabinovich_fabrikant_z;
            break;
        case ODE_CHUA:
            function_x = chua_x;
            function_y = chua_y;
            function_z = chua_z;
            break;
    }

    k1 = function_x((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z,(*head_ptr)->t)*dt;
    k2 = function_x((*head_ptr)->x+k1/2,(*head_ptr)->y,(*head_ptr)->z,(*head_ptr)->t+dt/2)*dt;
    k3 = function_x((*head_ptr)->x+k2/2,(*head_ptr)->y,(*head_ptr)->z,(*head_ptr)->t+dt/2)*dt;
    k4 = function_x((*head_ptr)->x+k3,(*head_ptr)->y,(*head_ptr)->z,(*head_ptr)->t+dt)*dt;
    x = (*head_ptr)->x + k1/6+k2/3+k3/3+k4/6;

    k1 = function_y((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z,(*head_ptr)->t)*dt;
    k2 = function_y((*head_ptr)->x,(*head_ptr)->y+k1/2,(*head_ptr)->z,(*head_ptr)->t+dt/2)*dt;
    k3 = function_y((*head_ptr)->x,(*head_ptr)->y+k2/2,(*head_ptr)->z,(*head_ptr)->t+dt/2)*dt;
    k4 = function_y((*head_ptr)->x,(*head_ptr)->y+k3,(*head_ptr)->z,(*head_ptr)->t+dt)*dt;
    y = (*head_ptr)->y + k1/6+k2/3+k3/3+k4/6;

    k1 = function_z((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z,(*head_ptr)->t)*dt;
    k2 = function_z((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z+k1/2,(*head_ptr)->t+dt/2)*dt;
    k3 = function_z((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z+k2/2,(*head_ptr)->t+dt/2)*dt;
    k4 = function_z((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z+k3,(*head_ptr)->t+dt)*dt;
    z = (*head_ptr)->z + k1/6+k2/3+k3/3+k4/6;

    plist_add_front(head_ptr, x, y, z, (*head_ptr)->t+dt);
}

#endif
