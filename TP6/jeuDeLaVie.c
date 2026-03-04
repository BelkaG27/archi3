# define NB_COLONNES 20
# define NB_LIGNES 10

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

int sem_init (sem_t * sem , int pshared , unsigned int value ){}
int sem_destroy (sem_t * sem ){}
int sem_wait (sem_t * sem ){}
int sem_post (sem_t * sem ){}


char tab [NB_LIGNES][NB_COLONNES];

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
                if(( I<NB_LIGNES || I>=0 ) && ( J<NB_COLONNES || J>=0 )){
                    if(tab[I][J]=='x')compteur++;
                }
            }
        }
       if(compteur>=3) return 'x';
    
    }else if(tab[i][j]=='x'){
        for(int I=i-1;I<=i+1;I++){
            for(int J=j-1;J<=j+1;J++){
                if(( I<NB_LIGNES || I>=0 ) && ( J<NB_COLONNES || J>=0 )){
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
        for(int j=0;j<NB_LIGNES;j++){
            printf("%c ",tab[i][j]);
        }
        printf("\n");
    }
}


int main() {
    srand(time(NULL));

    init(tab);
    afficher(tab);

    return 0;
}


