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
    unsigned long long int i, j, index, pim, transiente;
    unsigned long long int *primi = calloc(LIM,sizeof(unsigned long long int));
    char *messaggio = calloc(LIM,sizeof(char));
    char *passwd = calloc(LIM,sizeof(char));
    mpz_t *fattori = calloc(LIM,sizeof(mpz_t));
    
    for(i=0; i<LIM; i++){
        mpz_init(fattori[i]);
    }
    
    mpz_t g;
    mpz_init(g);
    mpz_add_ui(g,g,1);
    
    // input
    printf("Inserisci il messaggio (limite di %llu char): ",LIM);
    fgets(messaggio,LIM,stdin);
    messaggio[strlen(messaggio)-1] = '\0'; // perchè salva anche il char '\n' per ultimo
    
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
    
    // inizializzo la sequenza caratteristica e calcolo un transiente con il codice PIM
    srand(seed);
    for(i=0; i<pim; i++){
        transiente = rand();
    }
    
    bool *mask = calloc(strlen(messaggio),sizeof(bool));
    
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
        if(j >= strlen(messaggio)){
            break;
        }
    }
    
    // calcolo del numero di godel associato al messaggio
    fprintf(stderr,"Calcolo fattori moltiplicativi anagrammati...\n");
    for(i=0; i<strlen(messaggio); i++){
        // scambiamo l'ordine dei numeri primi per creare un anagramma casuale
        index = randInclusivo(0,strlen(messaggio));
        
        while(mask[index] == 1){
            index = randInclusivo(0,strlen(messaggio));
        }
        
        mask[index] = 1;
        mpz_ui_pow_ui(fattori[i],primi[index],messaggio[i]);
        //mpz_out_str(stdout,10,fattori[i]);
        //fprintf(stdout,"\n\n");
    }
    
    mpz_t tmp;
    mpz_init(tmp);
    
    fprintf(stderr,"Calcolo numero di Godel...\n");
    for(i=0; i<strlen(messaggio); i++){
        mpz_set(tmp,fattori[i]);
        mpz_mul(g,tmp,g);
    }
    
    // scrittura su file del numero di godel
    FILE *output = fopen("NumGodel.txt","w");
    mpz_out_str(output,10,g);
    fclose(output);
    
    free(primi);
    free(messaggio);
    free(fattori);
    free(passwd);
    free(mask);
    mpz_clear(g);
    mpz_clear(tmp);
    
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
