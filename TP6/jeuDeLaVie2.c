# define NB_COLONNES 20
# define NB_LIGNES 10

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
typedef unsigned int uint;

typedef sem_t semaphore;

pthread_mutex_t mutex_compteur;
pthread_cond_t cond;

pthread_t tab_thread[NB_LIGNES][NB_COLONNES];

char tab [NB_LIGNES][NB_COLONNES];
char nouv_tab [NB_LIGNES][NB_COLONNES];

int stop=0;

int compteur_case=0;

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
                if((I!=i || J!=j) &&( I<NB_LIGNES && I>=0 ) && ( J<NB_COLONNES && J>=0 )){
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
        if(compteur_case<NB_LIGNES*NB_COLONNES){
            pthread_cond_wait(&cond,&mutex_compteur);
        }else{
            compteur_case=0;
            stop=1;
            for(int I=0; I<NB_LIGNES; I++){
                for(int J=0; J<NB_COLONNES; J++){
                    if(tab[I][J] != nouv_tab[I][J]){
                        stop = 0;
                    }
                    tab[I][J] = nouv_tab[I][J];
                }
            }
            afficher(tab);
            pthread_cond_broadcast(&cond);
        }
        pthread_mutex_unlock(&mutex_compteur);
    } 
    return NULL;
}



int main() {
    srand(time(NULL));

    init(tab);
    afficher(tab);

    pthread_mutex_init(&mutex_compteur,NULL);
    pthread_cond_init(&cond,NULL);


    uint tab_cmptr[NB_LIGNES*NB_COLONNES];
    int cmptr=0;

    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            tab_cmptr[cmptr]=cmptr;
            pthread_create(&tab_thread[i][j],NULL,thread,&tab_cmptr[cmptr]);
            cmptr++;
        }
    }


    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            pthread_join(tab_thread[i][j],NULL);
        }
    }

    pthread_mutex_destroy(&mutex_compteur);
    pthread_cond_destroy(&cond);

    return 0;
}


