# define NB_COLONNES 20
# define NB_LIGNES 10

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
typedef unsigned int uint;



typedef sem_t semaphore;

pthread_mutex_t mutex_compteur=PTHREAD_MUTEX_INITIALIZER;

semaphore tab_sem[NB_LIGNES][NB_COLONNES];
pthread_t tab_thread[NB_LIGNES][NB_COLONNES];

char tab [NB_LIGNES][NB_COLONNES];
char nouv_tab [NB_LIGNES][NB_COLONNES];

int stop=0;

int compteur_case;

void init(char tab[NB_LIGNES][NB_COLONNES]){
    int r;

    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            r = rand() % 2;
            if(r==0){
                tab[i][j]='x';
            }else{
                tab[i][j]='-';
            }
        }
    }
}

char etat_suivant(int i,int j,char tab[NB_LIGNES][NB_COLONNES]){
    int compteur= 0 ;
    if(tab[i][j]=='-'){
        for(int I=i-1;I<=i+1;I++){
            for(int J=j-1;J<=j+1;J++){
                if(I!=i && J!=j &&( I<NB_LIGNES && I>=0 ) && ( J<NB_COLONNES && J>=0 )){
                    if(tab[I][J]=='x')compteur++;
                }
            }
        }
       if(compteur==3) return 'x';

    }else if(tab[i][j]=='x'){
        for(int I=i-1;I<=i+1;I++){
            for(int J=j-1;J<=j+1;J++){
                if((I!=i || J!=j) && ( I<NB_LIGNES && I>=0 ) && ( J<NB_COLONNES && J>=0 )){
                    if(tab[I][J]=='x')compteur++;
                }
            }
        }
        if(compteur==3 || compteur==2) return 'x';
    }

    return '-';
}

void afficher(char tab[NB_LIGNES][NB_COLONNES]){
    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            printf("%c ",tab[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
}

void* thread(void* arg){
    int casee = *((int*)arg);
    int i=casee / NB_COLONNES;
    int j=casee % NB_COLONNES;
    while(stop==0){
        char etat = etat_suivant(i,j,tab);
        nouv_tab[i][j]=etat;
        pthread_mutex_lock(&mutex_compteur);
        compteur_case++;
        pthread_mutex_unlock(&mutex_compteur);
        sem_wait(&tab_sem[i][j]);
    } 
}

void* thread_centrale(void* arg){
    while(stop==0){
        pthread_mutex_lock(&mutex_compteur);
        if(compteur_case==NB_COLONNES*NB_LIGNES){
            compteur_case=0;
            pthread_mutex_unlock(&mutex_compteur);
            stop=1;
            for(int i=0;i<NB_LIGNES;i++){
                for(int j=0;j<NB_COLONNES;j++){
                    if(tab[i][j]!=nouv_tab[i][j]){
                        stop=0;
                    }
                    tab[i][j]=nouv_tab[i][j];
                }
            }
            afficher(tab);
            for(int i=0;i<NB_LIGNES;i++){
                for(int j=0;j<NB_COLONNES;j++){
                    sem_post(&tab_sem[i][j]);
                }
            }  
        }else{
        pthread_mutex_unlock(&mutex_compteur);
        }
    }
}


int main() {
    srand(time(NULL));

    init(tab);
    afficher(tab);


    uint tab_cmptr[NB_LIGNES*NB_COLONNES];
    pthread_t central;
    int cmptr=0;

    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            sem_init(&tab_sem[i][j],0,0);
            tab_cmptr[cmptr]=cmptr;
            pthread_create(&tab_thread[i][j], NULL,thread,&tab_cmptr[cmptr]);
            cmptr++;
        }
    }

    pthread_create(&central,NULL,thread_centrale,NULL);

    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            pthread_join(tab_thread[i][j],NULL);
        }
    }
    pthread_join(central,NULL);

    

    return 0;
}


