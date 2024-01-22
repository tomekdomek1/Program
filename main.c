#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Pytanie{
    char tresc_pytania[200];
    char mozliwe_odpowiedzi[4][100];
    int poprawna_odpowiedz;
} Pytanie;

typedef struct Element{
    Pytanie pytanie;
    struct Element* next;
} Element;

Element* dodaj_do_listy(Element* head, Pytanie pytanie){
    Element* nowy = malloc(sizeof(Element));
    nowy -> pytanie = pytanie;
    nowy -> next = head;
    head = nowy;
    return head;
}

Element* wczytaj_pytania(Element* head, char* nazwa_pliku){
    FILE *plik = fopen(nazwa_pliku, "r");
    if(plik == NULL){
        printf("Nie mozna otworzyc pliku");
    }
    int max_rozmiar = 600;
    char *bufer = malloc(sizeof(char)*max_rozmiar);
    
    //biore linie, tworze na jej podstawie pytanie, dodaje pytanie do listy
    while(fgets(bufer, max_rozmiar, plik)){
        Pytanie pytanie;
        
        int k=0, i=0;
        while(bufer[k] != ':'){
            pytanie.tresc_pytania[i] = bufer[k];
            i++;
            k++;
        }
        pytanie.tresc_pytania[i] = '\0';
        i=0;
        k++;
        for(int j=0; j<4; j++){
            while(bufer[k] != ':'){
                pytanie.mozliwe_odpowiedzi[j][i] = bufer[k];
                i++;
                k++;
            }
            //dodaje koniec str zeby moc printowac %s
            pytanie.mozliwe_odpowiedzi[j][i] = '\0';
            i=0;
            k++;
        }
        pytanie.poprawna_odpowiedz = (int)bufer[k] - 48;

        head = dodaj_do_listy(head, pytanie);
    }

    free(bufer);
    fclose(plik);
    
    return head;
}

//testowa (nie bedzie uzywana w programie)
void wypisz_liste(Element* head){
    while(head != NULL){
        printf("%s\n", head -> pytanie.tresc_pytania);
        for(int i=0; i<4; i++){
            printf("%s\n", head -> pytanie.mozliwe_odpowiedzi[i]);
        }
        printf("Poprawna odpowiedz to: %s\n", head -> pytanie.mozliwe_odpowiedzi[head -> pytanie.poprawna_odpowiedz - 1]);
        head = head -> next;
    }
}

int main(){
    char *nazwa_pliku = "lista_pytan.txt";
    Element* head = NULL;
    
    //TESTY
    /*
    Pytanie pierwsze;
    strcpy(pierwsze.tresc_pytania, "tresc");
    pierwsze.poprawna_odpowiedz = 2;
    strcpy(pierwsze.mozliwe_odpowiedzi[0], "Odpowiedz pierwsza");
    strcpy(pierwsze.mozliwe_odpowiedzi[1], "Odpowiedz druga");
    strcpy(pierwsze.mozliwe_odpowiedzi[2], "Odpowiedz trzecia");
    strcpy(pierwsze.mozliwe_odpowiedzi[3], "Odpowiedz czwarta");
    Pytanie drugie;
    strcpy(drugie.tresc_pytania, "tresc");
    drugie.poprawna_odpowiedz = 1;
    strcpy(drugie.mozliwe_odpowiedzi[0], "odp pierwsza");
    strcpy(drugie.mozliwe_odpowiedzi[1], "odp druga");
    strcpy(drugie.mozliwe_odpowiedzi[2], "odp trzecia");
    strcpy(drugie.mozliwe_odpowiedzi[3], "odp czwarta");
    head = dodaj_do_listy(head, pierwsze);
    head = dodaj_do_listy(head, drugie);
    */

    head = wczytaj_pytania(head, nazwa_pliku);
    wypisz_liste(head);

    return 0;
}