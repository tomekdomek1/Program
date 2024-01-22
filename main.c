#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define and &&
#define or ||

int safescanf() {
    int zmienna;
    while (scanf("%d", &zmienna) != 1) {
        printf("Błędne dane. Podaj liczbę ponownie: ");
        while (getchar() != '\n');
    }
    return zmienna;
}

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

//NIE DZIALA
//zakladam ze znam rozmiar listy (19) - do pobierania z funkcji wczytujacej +1 po ewentualnym dodaniu pytania
Element* losuj_pytania(Element* head){
    Element* new_head = NULL;
    srand(time(0));
    int rozmiar = 4;
    int liczba_pytan = 2;
    for(int i=0; i<liczba_pytan; i++){
        Element* temp = head;
        int j, losowa = rand()%rozmiar;
        for(j=0; j<losowa; j++){
            temp = temp->next;
        }
        new_head = dodaj_do_listy(new_head, temp->pytanie);
        // dodac usuwanie wykorzystanych nodow
        head = usun_indeks(head, j);
    }
    return new_head;
}
void play(){
    int test=0;
    while(!test){
        printf("Przed Toba bedzie 12 pytan. \n");
        printf("Po kazdym pytaniu wyswietla Ci sie 4 odpowiedzi, Twoim zadaniem bedzie zaznaczyc poprawna odpowiedz.\n");
        printf("By ulatwic Ci zostanie milionerem, do dyspozycji bedziesz mial 3 kola ratunkowe:\n");
        printf("Pytanie do publicznosci, 50/50 oraz Telefon do przyjaciela. Kazdego z kol mozna uczyc tylko raz.\n");
        printf("Czy zasady gry sa zrozumiale? Tak - 1 / Nie - 0\n");
        printf(":");
        int odp;
        odp = safescanf();
        if(odp==1){
            test=1;
        }
        else if(odp!=0 and odp!=1){
            printf("\nNie ma takiej opcji do wyboru, wyswietle zasady jeszcze raz.\n\n");
        }
        else{
            printf("\nZatem powtorze zasady: \n\n");
        }
    }
    printf("\nZatem zacznijmy gre!\n\n");
}


int main(){
    char *nazwa_pliku = "test.txt";
    Element* head = NULL;
    while(1){
        printf("Witamy w grze milionerzy!");
        printf("Wybierz opcje: \n");
        printf("1. Zagraj.\n");
        printf("2. Dodaj pytanie.\n");
        printf("3. Przywroc domyslne.\n");
        printf("4. Wyjdz.\n");
        int wybor;
        wybor = safescanf();
        switch(wybor){
            case 1: 
                printf("Rozpocznijmy gre!\n");
                play();
                break;
            case 2:
                printf("Tutaj dodamy pytanie.\n");
                break;
            case 3:
                printf("Tutaj przywrocimy ustawienia domyslne.\n");
                break;
            case 4:
                return 0;
                break;
            default:
                printf("Nie ma takiej opcji! Sprobuj jeszcze raz!\n");
                break;
        }
    
    }
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
    //fajnie byloby losuj_pytania zapisac do innej zmiennej, zeby nie zgubic heada i zwolnic pamiec
    //head = losuj_pytania(head);
    //wypisz_liste(head);
    //head = usun_indeks(head, 0);
    //wypisz_liste(head);

    return 0;
}