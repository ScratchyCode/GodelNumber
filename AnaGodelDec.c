// Coded by ScratchyCode
// Calcola il numero di Godel di una frase inserita usando l'ordinamento ASCII dei char come alfabeto;
// NB: i caratteri accentati causano problemi perchè non sono char ASCII.
// Compilare con: -lm -lgmp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gmp.h>
#include <stdbool.h>

#define LIM 10000ULL

int test(unsigned long long int num);
unsigned long long int randInclusivo(unsigned long long int begin, unsigned long long int end);

int main(){
    unsigned long long int i, j, index, len, pim, transiente;
    unsigned long long int *primi = calloc(LIM,sizeof(unsigned long long int));
    char *esponenti = calloc(LIM,sizeof(char));
    char *passwd = calloc(LIM,sizeof(char));
    char *file = calloc(LIM,sizeof(char));
    
    // input
    printf("Inserisci il nome del file: ");
    fgets(file,LIM,stdin);
    file[strlen(file)-1] = '\0';
    
    printf("Inserisci la password (limite di %llu char): ",LIM);
    fgets(passwd,LIM,stdin);
    passwd[strlen(passwd)-1] = '\0';
    
    printf("Inserisci il codice PIM: ");
    scanf("%llu",&pim);
    
    // calcolo un seed a partire dalla chiave
    unsigned long long int seed = 0;
    for(i=0; i<strlen(passwd); i++){
        seed += passwd[i];
    }
    
    // lettura file
    FILE *pf = fopen(file,"r");
    mpz_t g;
    mpz_init(g);
    mpz_inp_str(g,pf,10);
    fclose(pf);
    
    bool *mask = calloc(LIM,sizeof(bool));
    
    // cerco i numeri primi
    fprintf(stderr,"Ricerca numeri primi necessari...\n");
    i = 2;    // numero primo
    j = 0;    // indice array dei numeri primi
    primi[j] = i;
    while(1){
        i++;
        
	    if(i%2 == 0){
	    // esclusione dei numeri pari
            continue;
        }else{
        // test primalità 
            if(test(i)){
                j++;
                primi[j] = i;
            }
        }
        
        // termino la ricerca quando ho i numeri primi sufficienti
        if(j >= LIM){
            break;
        }
    }
    
    // fattorizzazione
    fprintf(stderr,"Fattorizzazione numero di Godel...\n");
    i = 0;
    while(1){
        // divido per un numero primo fino a quando è possibile
        if(mpz_divisible_ui_p(g,primi[i]) != 0){
            mpz_cdiv_q_ui(g,g,primi[i]);
            esponenti[i]++;
        }else{
            // vado al primo successivo
            i++;
            
            // uscita
            if(i == LIM){
                break;
            }
        }
    }
    
    // inizializzo la sequenza caratteristica
    srand(seed);
    for(i=0; i<pim; i++){
        transiente = rand();
    }
    
    // ricostruiamo la lunghezza del messaggio e l'ordine degli esponenti per decifrare l'anagramma
    len = 0;
    for(i=0; i<LIM; i++){
        if(esponenti[i] != 0){
            len++;
        }
    }
    
    fprintf(stderr,"\nMessaggio:\n");
    for(i=0; i<len; i++){
        index = randInclusivo(0,len);
        
        while(mask[index] == 1){
            index = randInclusivo(0,len);
        }
        
        mask[index] = 1;
        fprintf(stderr,"%c",esponenti[index]);
    }
    fprintf(stderr,"\n");
    
    primi = NULL;
    esponenti = NULL;
    free(primi);
    free(esponenti);
    free(passwd);
    free(file);
    free(mask);
    mpz_clear(g);
    
    return 0;
}

int test(unsigned long long int num){
    unsigned long long int i;
    unsigned long long int lim;

    // il controllo di primalità ha senso eseguirlo fino alla radice del numero
    lim = (unsigned long long int)(sqrtl((long double)num));
    
    // tmp è un numero naturale --> i è un numero dispari
    unsigned long long int tmp = 1;
    do{
        // controllo la divisione solo per numeri dispari
        i = 2*tmp+1;
        if(num%i == 0){
            return 0;
        }
        tmp++;
    }while(i <= lim);
    
    return 1;
}

unsigned long long int randInclusivo(unsigned long long int begin, unsigned long long int end){
    unsigned long long int range = (end - begin) + 1;
    unsigned long long int limit = ((unsigned long long int)RAND_MAX + 1) - (((unsigned long long int)RAND_MAX + 1) % range);
    
    unsigned long long int randVal = rand();
    while (randVal >= limit) randVal = rand();

    return (randVal % range) + begin;
}
