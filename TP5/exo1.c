#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
typedef unsigned int uint;
#define MAX_NUM 30 // borne sup des valeurs du tableau
#define MAX_SIZE 100 //taille maximale du tableau

int tab[MAX_SIZE];

int veut_entrer[2];

int tour;

int size_tab=0;

void init(){
    veut_entrer[0]=0;
    veut_entrer[1]=0;
    tour=0;
}

int compare(const void* a, const void* b) {
    int const* pa = a;
    int const* pb = b;
    return (*pb- *pa);
}

void lock(int index){
    veut_entrer[index]=1;
    tour=1-index;
    while(veut_entrer[1-index]==1 && tour==1-index){
        //attendre
    }
}
void unlock(int index){
    veut_entrer[index]=0;
}

void* produce(void* arg) {
    int index = *((int*)arg);
    uint count = 0;
    while (count < 5) {
        // debut section critique
        lock(index);
        tab[size_tab++] = rand() % MAX_NUM;
        printf("produce: %d\n", tab[size_tab- 1]);
        // fin section critique
        unlock(index);
        count++;
    }
    return NULL; 
}

void* consume(void* arg) {
    int index = *((int*)arg);
    uint count = 0;
    while (count != 5) {
        if (count < size_tab) {
            // debut section critique
            lock(index);
            qsort(tab, MAX_SIZE / sizeof(int), sizeof(int), compare);
            printf("consume: sort [%d elements] =>", size_tab);
            for (uint i = 0; i < size_tab; i++)
                printf(" %d", tab[i]);
                printf("\n");
                count = size_tab;
                // fin section critique
                unlock(index);
        }
    }
    return NULL;
}




int main (){
    pthread_t thread1;
    pthread_t thread2;
    uint par1=0;
    uint par2=1;
    init();
    pthread_create (&thread1 , NULL , produce ,&par1);
    pthread_create (&thread2 , NULL , consume ,&par2);
    pthread_join (thread1 , NULL );
    pthread_join (thread2 , NULL );
    return 0;
}