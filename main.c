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
        //dodaje koniec str zeby moc printowac %s
        pytanie.tresc_pytania[i] = '\0';
        i=0;
        k++;
        for(int j=0; j<4; j++){
            while(bufer[k] != ':'){
                pytanie.mozliwe_odpowiedzi[j][i] = bufer[k];
                i++;
                k++;
            }
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

Element* usun_indeks(Element* head, int index){
    Element* temp = head;
    int i;
    if (index == 0) {
        head = head->next;
        temp->next = NULL;
        free(temp);
    }
    else {
        for (i = 0; i < index - 1; i++) {
            temp = temp->next;
        }
        Element* del = temp->next;
        temp->next = temp->next->next;
        del->next = NULL;
        free(del);
    }
    return head;
}

//zakladam ze znam rozmiar listy (19) - do pobierania z funkcji wczytujacej +1 po ewentualnym dodaniu pytania
Element* losuj_pytania(Element* head){
    Element* new_head = NULL;
    srand(time(0));
    int rozmiar = 19;
    int liczba_pytan = 12;
    for(int i=0; i<liczba_pytan; i++){
        Element* temp = head;
        int j, losowa = rand()%rozmiar;
        for(j=0; j<losowa - i; j++){
            temp = temp->next;
        }
        new_head = dodaj_do_listy(new_head, temp->pytanie);
        head = usun_indeks(head, j);
    }
    return new_head;
}

int main(){
    char *nazwa_pliku = "lista_pytan.txt";
    Element* head = NULL;

    //NAJPIERW TRZEBA WCZYTAC PYTANIA A DOPIERO POZNIEJ LOSUJ_PYTANIA
    //NAJPIERW TRZEBA WCZYTAC PYTANIA A DOPIERO POZNIEJ LOSUJ_PYTANIA
    //NAJPIERW TRZEBA WCZYTAC PYTANIA A DOPIERO POZNIEJ LOSUJ_PYTANIA
    head = wczytaj_pytania(head, nazwa_pliku);
    //fajnie byloby losuj_pytania zapisac do innej zmiennej, zeby nie zgubic heada i zwolnic pamiec
    head = losuj_pytania(head);
    wypisz_liste(head);

    return 0;
}