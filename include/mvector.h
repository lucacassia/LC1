#ifndef MVECTOR
#define MVECTOR

#include<stdio.h>
#include<stdlib.h>

typedef struct {
    FILE* dump;
    size_t size;
} mvector;

mvector* mopen(size_t size){
    mvector* obj = malloc(sizeof(mvector));
    obj->size = size;
    obj->dump = tmpfile();
    int i;
    double zero = 0.0;
    for(i=0;i<size;i++)
        fwrite(&zero,sizeof(double),1,obj->dump);
    return obj;
}

int mclose(mvector* obj){
    return fclose(obj->dump);
}

double mget(mvector* obj,unsigned index){
    double tmp = 0;
    if(index < obj->size){
        fseek(obj->dump,index*sizeof(double),SEEK_SET);
        fread(&tmp,sizeof(double),1,obj->dump);
    }
    return tmp;
}

void mset(mvector* obj,unsigned index,double arg){
    if(index < obj->size){
        fseek(obj->dump,index*sizeof(double),SEEK_SET);
        fwrite(&arg,sizeof(double),1,obj->dump);
    }
}

void madd(mvector* obj,double arg){
    fseek(obj->dump,obj->size*sizeof(double),SEEK_SET);
    fwrite(&arg,sizeof(double),1,obj->dump);
    obj->size++;
}

#endif

