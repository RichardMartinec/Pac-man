#include <iostream>  
#include <stdio.h>  
#include <windows.h>  
#include <string>  
#include <vector>  
using namespace std;
char tmp_map[21][32];
char map[21][32] = {
    "+#############################+",
    "|              #              |",
    "| ###  #####   #   #####  ### |",
    "|                             |",
    "| ###  #   #########   #  ### |",
    "|      #       #       #      |",
    "|####  ####    #    ####  ####|",
    "    #  #               #  #    ",
    "|####  #  ##### #####  #  ####|",
    "          #         #          ",

    "|####  #  ###########  #  ####|",
    "    #  #               #  #    ",
    "|####  #   #########   #  ####|",
    "|              #              |",
    "|  ##  ####    #    ####  ##  |",
    "|#  #                     #  #|",
    "|   #  #   #########   #  #   |",
    "|      #       #       #      |",
    "|  #########   #   #########  |",
    "|                             |",
    "+#############################+" };

void ShowMap() //vypisanie obsahu pola map na obrazovku
{
    for (int i = 0; i < 21; i++) //cyklus prechadza po riadkoch
    {
        printf("%s\n", map[i]);//vypis jedneho celeho riadku na obrazovku
    }
}

void gotoxy(short x, short y) //nastavenie polohy na obrazovke
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); //zistenie handle obrazovky
    COORD position = { x, y }; //vlozenie polohy kurzora x, y na obrazovke do struktury position
    SetConsoleCursorPosition(hStdout, position); //nastavenie kurzora na obrazovke na polohu x,y
}


struct walk //definovanie struktury pre BFS prehladavanie
{
    short walk_count;
    short x;
    short y;
    short back;
};

struct target //definovanie struktury pre cestuod ducha k pac-manovi
{
    short x;
    short y;
};

vector<target> najdena_cesta; //zadefinovanie premennej najdena_cesta ako vector so strukturou target

vector<walk> BFSPrehladavanie; //zadefinovanie premennej BFSPrehladavanie ako vector so strukturou walk
//vector<target>  pomoc;

void PrehladajPolicko(int x, int y, int wc, int back) //funckia na pridanie volneho policka pri BFS prehladavani
{
    if (tmp_map[y][x] == ' ' || tmp_map[y][x] == '.')// ak je policko s polohou x,y v tmp_map volnem nieje tam stena #
    {
        tmp_map[y][x] = '#';//oznac volne policko x,y ako navstivene v tmp_map
        walk tmp; //zadefinovanie pomocnej premennej tmp pre ulozenie polohy x,y a walk_count a back volneho policka
        tmp.x = x; //poloha x volneho policka
        tmp.y = y; //poloha y volneho policka
        tmp.walk_count = wc; //walk_count volneho policka
        tmp.back = back; //back volneho policka
        BFSPrehladavanie.push_back(tmp); //prekopirovanie polohy x,y, walk_count a back volneho policka z tmp do BFSPrehladavanie
    }
}

void NajdiCestu(int sx, int sy, int x, int y)//vstupy sx, sy su polha ducha, suradnice x,y su poloha pacmanna
{
    memcpy(tmp_map, map, sizeof(map)); //kopirovanie v pamati pola map do pola tmp_map
    BFSPrehladavanie.clear(); //vycistenie struktury BFSPrehladavanie
    walk tmp; //zadefinovanie pomocnej struktury formatu BFSPrehladavanie
    tmp.x = sx; //zapamatanie polohy x ducha 
    tmp.y = sy; //zapamatanie polohy y ducha 
    tmp.walk_count = 0; //nastavenie pocitadla poctu stvorcekov ducha od pacmana 
    tmp.back = -1; //nastavenie pocitadla cesty od ducha k pacmannovi
    BFSPrehladavanie.push_back(tmp);//prekopirovanie struktury tmp do BFSPrehladavanie

    int i = 0;
    while (i < BFSPrehladavanie.size())
    {
        if (BFSPrehladavanie[i].x == x && BFSPrehladavanie[i].y == y)// je poloha ducha rovnaka ako poloha pacmanna?
        {
            najdena_cesta.clear(); //vycistenie struktury najdena_cesta
            target tmp2;//zadefinovanie pomocnej struktury formatu najdena_cesta
            while (BFSPrehladavanie[i].walk_count != 0)//rob pokial nie je cesta od ducha k pacmannovi prazdna
            {
                tmp2.x = BFSPrehladavanie[i].x; //do pomocnej premennej tmp2.x vloz i-tu polohu x cesty od ducha k pacmannovi
                tmp2.y = BFSPrehladavanie[i].y; //do pomocnej premennej tmp2.y vloz i-tu polohu y cesty od ducha k pacmannovi
                najdena_cesta.push_back(tmp2); //prekopiruj i-tu polohu x,y cesty od ducha k pacmannovi do najdena_cesta

                i = BFSPrehladavanie[i].back; //nastavenie dalsieho policka na ceste v smere od ducha k pacmannovi 
            }

            break;
        }

        PrehladajPolicko(BFSPrehladavanie[i].x + 1, BFSPrehladavanie[i].y, BFSPrehladavanie[i].walk_count + 1, i); //zistovanie ci policko s polohou x+1, y (vpravo) je volne
        PrehladajPolicko(BFSPrehladavanie[i].x - 1, BFSPrehladavanie[i].y, BFSPrehladavanie[i].walk_count + 1, i); //zistovanie ci policko s polohou x-1, y (vlavo) je volne
        PrehladajPolicko(BFSPrehladavanie[i].x, BFSPrehladavanie[i].y + 1, BFSPrehladavanie[i].walk_count + 1, i); //zistovanie ci policko s polohou y+1, y (dole) je volne
        PrehladajPolicko(BFSPrehladavanie[i].x, BFSPrehladavanie[i].y - 1, BFSPrehladavanie[i].walk_count + 1, i); //zistovanie ci policko s polohou y-1, y (hore) je volne


        i++;
    }

    BFSPrehladavanie.clear(); //vycisti BFSPrehladavanie
}

int main()
{
    bool beh = true; // zapneme premennu running
    int x = 15; // pociatocna poloha pac-mana x  
    int y = 15; // pociatocna poloha pac-mana y  
    int old_x; // pomocne premenne na zapam‰tanie starej polohy x
    int old_y; //pomocne premenne na zapam‰tanie starej polohy y

    int ex = 15; // pociatocna poloha duch
    int ey = 8; // pociatocna poloha duch

    int skore = 0; // pocitadlo skore

    printf("Instrukcie:\n1. Sipky sluzia na pohyb Pac-mana\n2. Ziskavaj body jedenim bodiek vytvorenych duchom\n3. Nenechaj sa chytit duchom\n\n");
    printf("T -> Tazka\nN -> Normalna\nL -> Lahka\n\nZadaj obtiaznost: ");

    char obtiaznost; 
    int rychlost = 3; //najpomalsia r˝chlosù

    cin >> obtiaznost;

    if (obtiaznost == 'N')
    {
        rychlost = 2; //stredn· r˝chlosù
    }
    else if (obtiaznost == 'T')
    {
        rychlost = 1; //najv‰Ëöia r˝chlosù
    }

    system("cls"); // vymazanie obrazovky
    ShowMap(); //funkcia na zobrazenie mapy na obrazovke
    

    gotoxy(x, y); // funckia nastavi kurzor na poziciu x,y na obrazovke
    cout << "H"; // Vytvori Pac-mana

    int frame = 0;

    NajdiCestu(ex, ey, x, y); // najde cestu medzi duchom a pac-manom

    while (beh) 
    {
        gotoxy(x, y); //funckia nastavi kurzor na poziciu x,y na obrazovke
        cout << " "; // zmazanie H na povodnych suradniciach

        old_x = x; // zapametame si povodne x ako old_x
        old_y = y; // zapametame si povodne y ako old_y

        if (GetAsyncKeyState(VK_UP)) // funkcia nacita posledne zmacknutu klavesu,  ak je to sipka dohora, je to smer dohora 
        {
            if (map[y - 1][x] == '.') // ak sa y zmensi a je pred nim bodka
            {
                y--; // y sa zmensi o 1, pohyb o jedno pole hore
                skore++; // skore sa zvacsi o 1
            }
            else if (map[y - 1][x] == ' ') // ak sa y zmensi a nie je pred nim bodka
                y--; // zmensi sa o 1 y, pohyb o jedno pole hore
        }
        if (GetAsyncKeyState(VK_DOWN)) // funkcia nacita posledne zmacknutu klavesu, ak je to sipka dodola, je to smer dodola
        {
            if (map[y + 1][x] == '.') // ak sa y zvacsi a je pred nim bodka
            {
                y++; // y sa zvacsi o 1, pohyb o jedno pole dole
                skore++; // skore sa zvacsi o 1
            }
            else if (map[y + 1][x] == ' ') // ak sa y zvacsi a nie je pred nim bodka
                y++; // y sa zvacsi o 1, pohyb o jedno pole dole
        }
        if (GetAsyncKeyState(VK_LEFT)) // funkcia nacita posledne zmacknutu klavesu, ak je to sipka dolava, je to smer dolava
        {
            if (map[y][x - 1] == '.') // ak sa x zvacsi a je pred nim bodka
            {
                x--; // x sa zmensi o 1, pohyb o jedno pole dolava
                skore++; // skore sa zvacsi o 1
            }
            else if (map[y][x - 1] == ' ') // ak sa x zmensi a nie je pred nim bodka
                x--; // x sa zmensi o 1, pohyb o jedno pole dolava
        }
        if (GetAsyncKeyState(VK_RIGHT)) // funkcia nacita posledne zmacknutu klavesu, ak je to sipka doprava, je to smer doprava
        {
            if (map[y][x + 1] == '.') // ak sa x zvacsi a je pred nim bodka
            {
                x++; // x sa zvacsi o 1, pohyb o jedno pole doprava
                skore++; // skore sa zvacsi o 1
            }
            else if (map[y][x + 1] == ' ') // ak sa x zvacsi a nie je pred nim bodka
                x++; // x sa zvacsi o 1, pohyb o jedno pole doprava
        }

        if (old_x != x || old_y != y) // ak sa povodna poloha pac-mana zmenila
        {
            NajdiCestu(ex, ey, x, y); // najdi novu cestu medzi duchom a pac-manom
        }

        gotoxy(x, y); // nastavenie na zmenene suradnice
        cout << "H"; // na novych suradniciach vytvori Pac-mana

        map[ey][ex] = '.'; // nastavenie na zmenene suradnice ducha v poli map
        gotoxy(ex, ey); // nastavenie na zmenene suradnice ducha na obrazovke
        cout << "."; // vytvori na novych suradniciah bodku na obrazovke
       // cout << frame << endl;
       // cout << rychlost << endl;
        //cout << frame % rychlost << endl;
       // cout << najdena_cesta.size() << endl;
       // cin.get();

        if (frame % rychlost == 0 && najdena_cesta.size() != 0) // urcuje, ako rychlo sa duch hybe
        {
            ex = najdena_cesta.back().x; // vlozenie novej polohy x ducha na najdenej ceste k pac-manovi
            ey = najdena_cesta.back().y; // vlozenie novej polohy y ducha na najdenej ceste k pac-manovi
            najdena_cesta.pop_back();    // zmazanie poslednych hodnot x a y vo najdena_cesta

        }

        gotoxy(ex, ey); // presunie sa na suradnice ex, ey
        cout << "E"; // vytvori na novych suradniciah Ducha

        if (ex == x && ey == y) // ak sa suradnice ex, ey rovnaju suradniciam x, y duch nasiel pacmana
        {
            break; // koniec hry
        }

        
        gotoxy(32, 1);//nastavenie polohy na vypisanie skore
        cout << skore;//vypis skore
        Sleep(100);//oneskorenie, aby program bezal pomalsie
        frame++; //zvysenie frame o 1
    }

    system("cls"); //vymaz obrazovku
    printf("You Lose and your score is : %i", skore); // vypis konecne skore
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();

    return 0;
}