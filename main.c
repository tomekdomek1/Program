#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define and &&
#define or ||

#if defined(_WIN32) || defined(_WIN64)
    #define WINDOWS
#elif defined(_linux_) || defined(linux)
    #define LINUX
#elif defined(__APPLE__) || defined(_APPLE_) || defined(__MACH__)
    #define MACOS
#else
    #define OTHER_OS
#endif

float* pieniadze;

void clearconsole(){
    #ifdef WINDOWS
        system("cls");
    #elif defined(LINUX) || defined(MACOS)
        printf("\033[1;1H\033[2J");
    #endif
}

int safescanf() {
    int zmienna;
    while (scanf("%d", &zmienna) != 1) {
        printf("Błędne dane. Podaj liczbę ponownie: ");
        while (getchar() != '\n');
    }
    return zmienna;
}

int safecharscanf() {
    char zmienna;
    while (getchar() != '\n');
    printf("\n");
    while (scanf("%c", &zmienna) != 1) {
        printf("Bledny format odpowiedzi. Podaj odpowiedz ponownie: ");
        while (getchar() != '\n');
    }
    if(zmienna<'a' or zmienna>'d'){
        printf("Nie ma takiej odpowiedzi. Podaj odpowiedz ponownie: ");
        zmienna = safecharscanf();
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
    Element* nowy = (Element*)malloc(sizeof(Element));
    nowy -> pytanie = pytanie;
    nowy -> next = head;
    head = nowy;
    return head;
}

Element* wczytaj_pytania(Element* head, char* nazwa_pliku, int* liczba_pytan_plik){
    FILE *plik = fopen(nazwa_pliku, "r");
    if(plik == NULL){
        printf("Nie mozna otworzyc pliku z pytaniami");
    }
    int max_rozmiar = 600;
    char *bufer = (char*)malloc(sizeof(char)*max_rozmiar);
    
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
        *liczba_pytan_plik += 1;
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

Element* losuj_pytania(Element* head, int liczba_pytan_plik){
    Element* new_head = NULL;
    srand(time(0));
    int rozmiar = liczba_pytan_plik;
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

void przywroc_fabryczne_w_r(char* nazwa_pliku, char* plik_fabryczny){
    FILE *plik_nadpisywany = fopen(nazwa_pliku, "w");
    if(plik_nadpisywany == NULL){
        printf("Nie mozna otworzyc pliku z pytaniami");
    }
    FILE *plik = fopen(plik_fabryczny, "r");
    if(plik == NULL){
        printf("Nie mozna otworzyc pliku z pytaniami domyslnymi");
    }

    int max_rozmiar = 600;
    char *bufer = (char*)malloc(sizeof(char)*max_rozmiar);
    while(fgets(bufer, max_rozmiar, plik)){
        fputs(bufer, plik_nadpisywany);
    }

    free(bufer);
    fclose(plik);
    fclose(plik_nadpisywany);
}

void usun_znak(char *str, char c) {
    int i, j, len = strlen(str);
    for (i=j=0; i<len; i++) {
        if (str[i] != c) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void dodaj_pytanie_do_pliku(char* nazwa_pliku, char* pytanie, char* odp1, char* odp2, char* odp3, char* odp4, int poprawna){
    usun_znak(pytanie, ':');
    usun_znak(odp1, ':');
    usun_znak(odp2, ':');
    usun_znak(odp3, ':');
    usun_znak(odp4, ':');

    usun_znak(pytanie, '\n');
    usun_znak(odp1, '\n');
    usun_znak(odp2, '\n');
    usun_znak(odp3, '\n');
    usun_znak(odp4, '\n');
    
    FILE *plik = fopen(nazwa_pliku, "a");
    if(plik == NULL){
        printf("Nie mozna otworzyc pliku z pytaniami");
    }
    fputs("\n", plik);
    fputs(pytanie, plik);
    fputs(":", plik);
    fputs(odp1, plik);
    fputs(":", plik);
    fputs(odp2, plik);
    fputs(":", plik);
    fputs(odp3, plik);
    fputs(":", plik);
    fputs(odp4, plik);
    fputs(":", plik);

    char* poprawna_str = malloc(sizeof(char)*2);
    sprintf(poprawna_str, "%d", poprawna);
    fputs(poprawna_str, plik);

    free(poprawna_str);
    fclose(plik);
}

void pisz_regulamin(){
    printf("Przed Toba bedzie 12 pytan. \n");
    printf("Po kazdym pytaniu wyswietla Ci sie 4 odpowiedzi, Twoim zadaniem bedzie zaznaczyc poprawna odpowiedz.\n");
    printf("By ulatwic Ci zostanie milionerem, do dyspozycji bedziesz mial 3 kola ratunkowe:\n");
    printf("Pytanie do publicznosci, 50/50 oraz Telefon do przyjaciela. Kazdego z kol mozna uczyc tylko raz.\n");
    sleep(2);
    printf("Czy zasady gry sa zrozumiale? Tak - 1 / Nie - 0\n");
    printf(":");
}

void pisz_regulamin_extended(){
    printf("Przed Toba bedzie 12 pytan. \n");
    printf("Po kazdym pytaniu wyswietla Ci sie 4 odpowiedzi od a do d.\n");
    printf("Jezeli uznasz, ze nie jestes w stanie na to pytanie odpowiedziec, bedziesz mial 3 rozne kola ratunkowe.\n");
    sleep(6);
    clearconsole();
    printf("Kolo ratunkowe nr 1: Pytanie do publicznosci.\n");
    printf("To kolo przedstawi Ci ile ludzi z widowni zaznaczyloby poszczegolne odpowiedzi\n\n");
    printf("Kolo ratunkowe nr 2: 50/50\n");
    printf("Uzycie tego kola spowoduje losowe odrzucenie dwoch niepoprawnych odpowiedzi.\n\n");
    printf("Kolo ratunkowe nr 3: Telefon do przyjaciela\n");
    printf("To kolo wskaze Ci odpowiedz, ktora na Twoim miejscu zaznaczylby Twoj przyjaciel.\n\n");
    printf("Pamietaj, kazdego kola bedzie mozna uzyc tylko raz!\n");
    sleep(7);
    clearconsole();
    printf("UWAGA!\nPrzed zaznaczeniem odpowiedzi dokladnie sie zastanow!\nGdy odpowiedz bedzie niepoprawna stracisz cala wygrana!\n\n");
    printf("Jezeli zrezygnujesz z udzielenia odpowiedzi, zabierzesz aktualna wygrana do domu.\n");
    sleep(5);
    clearconsole();
    printf("To sa pieniadze, ktore mozesz wygrac odpowiadajac poprawnie na kolejne pytania: \n");
    for(int i=1;i<13;i++){
        printf("Pytanie nr %d, do wygrania: %d zl.\n",i,(int)pieniadze[i]);
    }
    sleep(5);
    clearconsole();
    printf("Czy zasady gry sa zrozumiale? Tak - 1 / Nie - 0\n");
    printf(":");
}

void Zasady_i_akceptacja_regulaminu(){
    clearconsole();
    int test=0;
    int odp=-1;
    while(!test){
        if(odp==-1){
            pisz_regulamin();
        }
        if(odp==0){
            pisz_regulamin_extended();
        }
        odp = safescanf();
        if(odp==1){
            clearconsole();
            test=1;
            printf("\nZatem zacznijmy gre!\n\n");
        }
        else if(odp!=0 and odp!=1){
            clearconsole();
            printf("\nNie ma takiej opcji do wyboru, wyswietle zasady jeszcze raz.\n\n");
        }
        else{
            clearconsole();
            printf("Zatem postaram sie jeszcze raz wytlumaczyc zasady: \n\n");
        }
    }
}

float mnoznik(int liczba){
    if(liczba == 4 )
        return 2.5;
    if(liczba == 8)
        return 1.875;
    return 2.0;
}

void pisz_decyzja_gra(){
    printf("Wybierz jedno: \n");
        printf("1. Chce zaznaczyc odpowiedz.\n");
        printf("2. Chce uzyc kola ratunkowego.\n");
        printf("3. Chce zrezygnowac z dalszej gry.\n");
}

void czy_przegrano(int wartosc, int runda){
    clearconsole();
    if(wartosc==1){
        printf("Bardzo nam przykro.\n");
        printf("Odchodzisz z niczym :(\n");
    }
    if(wartosc==2){
        if(runda!=0)
            printf("Mimo wszystko, gratulujemy, bo wygrales wlasnie: %d zl!\n",(int)pieniadze[runda]);
        if(runda==0){
            printf("Niestety, ale nic nie wygrales\n");
            printf("Mogles chociaz sprobowac, nastepnym razem sie uda!\n");
        }
    }
    sleep(3);
    clearconsole();
    
}

void informacje_i_tresc_pytania(Element* head, int i){
    printf("Pytanie nr %d: \n",i+1);
    printf("Teraz bedziesz walczyc o %d zl.\n",(int)pieniadze[i+1]);
    printf("%s\n\n",head->pytanie.tresc_pytania);
    printf("Tak prezentuja sie odpowiedzi:\n\n");
    for(int i=0;i<4;i++){
        printf("Odpowiedz %c: %s\n",'a'+i,head->pytanie.mozliwe_odpowiedzi[i]);
    }
    printf("\n");
}

void wyswietl_menu(){
    printf("Witamy w grze milionerzy! ");
    printf("Wybierz opcje: \n");
    printf("1. Zagraj.\n");
    printf("2. Dodaj pytanie.\n");
    printf("3. Przywroc domyslne.\n");
    printf("4. Wyjdz.\n");
}

void generuj_pieniadze(){
    pieniadze = (float*)malloc(sizeof(char)*13);
    pieniadze[0]=0.0;
    pieniadze[1]=500.0;
    pieniadze[9]=125000.0;
    for(int i=2;i<13;i++){
        if(i==9) continue;
        //printf("%f \n",mnoznik(i));
        pieniadze[i]=pieniadze[i-1]*mnoznik(i);
    }
}

int wczytaj_i_sprawdz_odpowiedz(Element* head){
    char odpowiedz = safecharscanf();
    int check = (int)odpowiedz - 'a' + 1;
    if(check==head->pytanie.poprawna_odpowiedz)
        return 1;
    else
        return 0;
}

void odpowiedni_komunikat_po_rundzie(int wynik,int i){
    if(wynik){
        czy_przegrano(wynik,i);
    }
    else if(!wynik){
        clearconsole();
        printf("Gratulacje, to poprawna odpowiedz!\n");
        printf("Twoja obecna wygrana wynosi: %d zl\n",(int)pieniadze[i+1]);
        printf("Przechodzimy do pytania nr %d\n",i+2);
        sleep(4);
        clearconsole();
    }
}

void komunikat_zla_odp(Element* head){
    clearconsole();
    printf("Niestety, ale to nie jest poprawna odpowiedz.\n");
    printf("Poprawnie nalezalo zaznaczyc odpowiedz: %c\n",head->pytanie.poprawna_odpowiedz+'a'-1);
    printf("%s\n",head->pytanie.mozliwe_odpowiedzi[head->pytanie.poprawna_odpowiedz - 1]);
    sleep(4);
    clearconsole();
}

void komunikat_rezygnacja(Element* head){
    clearconsole();
    printf("Szkoda, ze zrezygnowales z dalszej rozgrywki.\n");
    printf("Poprawnie nalezalo zaznaczyc odpowiedz: %c\n",head->pytanie.poprawna_odpowiedz+'a'-1);
    printf("%s\n",head->pytanie.mozliwe_odpowiedzi[head->pytanie.poprawna_odpowiedz - 1]);
    sleep(4);
}

void fifty_fifty(int poprawna){
    srand(time(0));
    int druga_odp = rand()%4+1;
    while(druga_odp==poprawna){
        druga_odp = rand()%4+1;
    }
    if(druga_odp>poprawna){
        int pom = druga_odp;
        druga_odp = poprawna;
        poprawna = pom;
    }
    printf("Odrzucilem dwie niepoprawne odpowiedzi.\n");
    printf("Poprawna odpowiedz to: \n");
    char litera1 = druga_odp + 'a' - 1;
    char litera2 = poprawna + 'a' - 1;
    printf("%c lub %c\n",litera1, litera2);
}

void kola_ratunkowe(Element* head, int** ratunek){
    while((*ratunek[0]) + (*ratunek[1]) + (*ratunek[2]) != 0){
        printf("Ktorego z kol chcesz uzyc? \n");
        printf("1. Pytanie do Publicznosci\n");
        printf("2. 50/50\n");
        printf("3. Telefon do przyjaciela");
        int kolo = -1;
        while(kolo<1 or kolo>3){
            kolo = safescanf();
            if(kolo<1 or kolo>3){
                printf("Podaj liczbe z przedzialu 1-3.\n");
            }
            if(!(*ratunek[kolo-1])){
                printf("Uzyles juz danego kola!");
            }
            if(kolo==2){
                fifty_fifty(head->pytanie.poprawna_odpowiedz);
            }
        }
        *ratunek[kolo-1] = 0;
    }
}

void play(Element* head){
    Zasady_i_akceptacja_regulaminu();
    int* ratunek = (int*)malloc(3*sizeof(int));
    ratunek[0]=1;
    ratunek[1]=1;
    ratunek[2]=1;
    int wynik = -1;
    for(int i=0;i<12;i++){ 
        informacje_i_tresc_pytania(head,i);
        int decyzja = -1;
        while(decyzja!=1 and decyzja!=2 and decyzja!=3){
            pisz_decyzja_gra();
            decyzja = safescanf();
            if(decyzja==1){
                printf("Podaj jedna z odpowiedzi a-d");
                if(wczytaj_i_sprawdz_odpowiedz(head)){
                    wynik = 0;
                }
                else{
                    wynik=1;
                    komunikat_zla_odp(head);
                }
            }
            else if(decyzja==2){
                printf("Ktorego z kol chcesz uzyc? \n");
                printf("1. Pytanie do Publicznosci\n");
                printf("2. 50/50\n");
                printf("3. Telefon do przyjaciela");
                int kolo = -1;
                while(kolo<1 or kolo>3){
                    kolo = safescanf();
                    if(kolo<1 or kolo>3){
                        printf("Podaj liczbe z przedzialu 1-3.\n");
                    }
                    if(!ratunek[kolo-1]){
                        printf("Uzyles juz danego kola!");
                    }
                    if(kolo==2){
                        fifty_fifty(head->pytanie.poprawna_odpowiedz);
                    }
                }
                //kola_ratunkowe(head, &ratunek);
            }
            else if(decyzja==3){
                wynik = 2;
                komunikat_rezygnacja(head);
            }
            else{
                printf("Podaj liczbe z przedzialu 1-3.\n");
            }
        }
        odpowiedni_komunikat_po_rundzie(wynik,i);
        if(wynik){
            break;
        }
        head = head -> next;
    }
    free(ratunek);
}

int main(){
    clearconsole();
    int liczba_pytan_plik = 0;
    char *nazwa_pliku = "lista_pytan.txt";
    char *nazwa_fabryczna = "lista_fabryczna.txt";
    Element* head = NULL;
    generuj_pieniadze();
    while(1){
        wyswietl_menu();
        int wybor;
        wybor = safescanf();
        switch(wybor){
            case 1: 
                //printf("%d\n",liczba_pytan_plik);
                liczba_pytan_plik = 0;
                head = wczytaj_pytania(head, nazwa_pliku, &liczba_pytan_plik);
                head = losuj_pytania(head, liczba_pytan_plik);
                //printf("%d\n",liczba_pytan_plik);
                printf("Rozpocznijmy gre!\n");
                play(head);
                break;
            case 2:
                //ten getchar() jest wazny zeby pytanie_uzytkownika nie bylo samym enterem
                getchar();
                int max_rozmiar = 200;
                char* pytanie_uzytkownika = malloc(sizeof(char) * max_rozmiar);
                printf("Wpisz tresc pytania\n");
                fgets(pytanie_uzytkownika, max_rozmiar, stdin);

                char* odp1_uzytkownika = malloc(sizeof(char) * max_rozmiar/2);
                printf("Wpisz tresc pierwszej odpowiedzi\n");
                fgets(odp1_uzytkownika, max_rozmiar/2, stdin);
                char* odp2_uzytkownika = malloc(sizeof(char) * max_rozmiar/2);
                printf("Wpisz tresc drugiej odpowiedzi\n");
                fgets(odp2_uzytkownika, max_rozmiar/2, stdin);
                char* odp3_uzytkownika = malloc(sizeof(char) * max_rozmiar/2);
                printf("Wpisz tresc trzeciej odpowiedzi\n");
                fgets(odp3_uzytkownika, max_rozmiar/2, stdin);
                char* odp4_uzytkownika = malloc(sizeof(char) * max_rozmiar/2);
                printf("Wpisz tresc czwartej odpowiedzi\n");
                fgets(odp4_uzytkownika, max_rozmiar/2, stdin);

                // tutaj czytam inta w niebezpieczny sposob :)
                int poprawna_uzytkownika = 0;
                printf("Podaj numer poprawnej odpowiedzi [1 - 4]\n");
                scanf("%d", &poprawna_uzytkownika);

                dodaj_pytanie_do_pliku(nazwa_pliku, pytanie_uzytkownika, odp1_uzytkownika, odp2_uzytkownika, odp3_uzytkownika, odp4_uzytkownika, poprawna_uzytkownika);

                free(pytanie_uzytkownika);
                free(odp1_uzytkownika);
                free(odp2_uzytkownika);
                free(odp3_uzytkownika);
                free(odp4_uzytkownika);
                break;
            case 3:
                przywroc_fabryczne_w_r(nazwa_pliku,nazwa_fabryczna);
                clearconsole();
                printf("Przywrocono ustawienia fabryczne.\n");
                sleep(1);
                printf("W puli znajduje sie teraz %d pytan.\n", liczba_pytan_plik);
                break;
            case 4:
                clearconsole();
                printf("Do zobaczenia!");
                return 0;
                break;
            default:
                printf("Nie ma takiej opcji! Sprobuj jeszcze raz!\n");
                break;
        }
    }  

    //fajnie byloby losuj_pytania zapisac do innej zmiennej, zeby nie zgubic heada i zwolnic pamiec
    //head = losuj_pytania(head);
    //wypisz_liste(head);
    //head = usun_indeks(head, 0);
    //wypisz_liste(head);
    free(pieniadze);
    return 0;
}
// a b c d
//