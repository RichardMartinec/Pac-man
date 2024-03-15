#include <iostream>  
#include <stdio.h>  
#include <windows.h>  
#include <string>  
#include <vector>  
using namespace std;
char tmp_map[18][32];
char map[18][32] = {
    "+#############################+",
    "|              |              |",
    "| ###  #####   |   #####  ### |",
    "|                             |",
    "|   |                         |",
    "| | |### |  |           |     |",
    "| |      |  | |###  |   |  |  |",
    "| | #####|  | |      ## |     |",
    "| |           |###  |      |  |",
    "| |##### ###         ##       |",
    "|          ######  ####### ###|",
    "|                             |",
    "|# ### ####      ###   #######|",
    "|                             |",
    "|                             |",
    "|                             |",
    "|                             |",
    "+#############################+" };

void ShowMap()
{
    for (int i = 0; i < 18; i++)
    {
        printf("%s\n", map[i]);
    }
}

void gotoxy(short x, short y)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };

    SetConsoleCursorPosition(hStdout, position);
}

class entity
{
public:
    entity(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void move_x(int p)
    {
        if (map[y][x + p] == ' ')
            x += p;
    }

    void move_y(int p)
    {
        if (map[y + p][x] == ' ')
            y += p;
    }

    void move(int p, int q)
    {
        x += p;
        y += q;
    }

    int get_x() { return x; }
    int get_y() { return y; }

    void draw(char p)
    {
        map[x][y] = p;
        gotoxy(x, y);
        printf("%c", p);
    }

private:
    int x;
    int y;
};

struct walk
{
    short walk_count;
    short x;
    short y;
    short back;
};

struct target
{
    short x;
    short y;
};

vector<target> walk_queue;

vector<walk> BFSArray;
vector<target>  pomoc;

void AddArray(int x, int y, int wc, int back)
{
    if (tmp_map[y][x] == ' ' || tmp_map[y][x] == '.')
    {
        tmp_map[y][x] = '#';
        walk tmp;
        tmp.x = x;
        tmp.y = y;
        tmp.walk_count = wc;
        tmp.back = back;
        BFSArray.push_back(tmp);
    }
}

void FindPath(int sx, int sy, int x, int y)
{
    memcpy(tmp_map, map, sizeof(map));
    BFSArray.clear();
    walk tmp;
    tmp.x = sx;
    tmp.y = sy;
    tmp.walk_count = 0;
    tmp.back = -1;
    BFSArray.push_back(tmp);

    int i = 0;
    while (i < BFSArray.size())
    {
        if (BFSArray[i].x == x && BFSArray[i].y == y)
        {
            walk_queue.clear();
            target tmp2;
            while (BFSArray[i].walk_count != 0)
            {
                tmp2.x = BFSArray[i].x;
                tmp2.y = BFSArray[i].y;
                walk_queue.push_back(tmp2);

                i = BFSArray[i].back;
            }

            break;
        }

        AddArray(BFSArray[i].x + 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
        AddArray(BFSArray[i].x - 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
        AddArray(BFSArray[i].x, BFSArray[i].y + 1, BFSArray[i].walk_count + 1, i);
        AddArray(BFSArray[i].x, BFSArray[i].y - 1, BFSArray[i].walk_count + 1, i);

        /*
            AddArray( BFSArray[i].x+1, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
            AddArray( BFSArray[i].x-1, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
            AddArray( BFSArray[i].x+1, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
            AddArray( BFSArray[i].x+1, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );

            AddArray( BFSArray[i].x+1, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );
            AddArray( BFSArray[i].x-1, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );
            AddArray( BFSArray[i].x-1, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
            AddArray( BFSArray[i].x-1, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );
        */
        i++;
    }

    BFSArray.clear();
}

int main()
{
    bool beh = true; // zapneme premennu running
    int x = 15; // pociatocna poloha pac-mana x  
    int y = 16; // pociatocna poloha pac-mana y  
    int old_x;
    int old_y;

    int ex = 1; // pociatocna poloha duch
    int ey = 1; // pociatocna poloha duch

    int skore = 0; // pocitadlo skore

    printf("Instrukcie:\n1. Sipky sluzia na pohyb Pac-mana\n2. Ziskavaj body jedenim bodiek vytvorenych duchom\n3. Nenechaj sa chytit duchom\n\n");
    printf("T -> Tazka\nN -> Normalna\nL -> Lahka\n\nZadaj obtiaznost: ");

    char obtiaznost;
    int rychlost = 2;

    cin >> obtiaznost;

    if (obtiaznost == 'N')
    {
        rychlost = 2;
    }
    else if (obtiaznost == 'T')
    {
        rychlost = 1;
    }

    system("cls"); // vymazanie obrazovky
    ShowMap(); //funkcia na zobrazenie mapy
    

    gotoxy(x, y); // funckia nastavi kurzor na poziciu x,y
    cout << "H"; // Vytvori Pac-mana

    int frame = 0;

    FindPath(ex, ey, x, y); // najde cestu medzi duchom a pac-manom

    while (beh) 
    {
        gotoxy(x, y); //funckia nastavi kurzor na poziciu x,y
        cout << " "; // zmazanie H na povodnych suradniciach

        old_x = x; // zapametame si povodne x ako old_x
        old_y = y; // zapametame si povodne y ako old_y

        if (GetAsyncKeyState(VK_UP)) // funkcia nacita posledne zmacknutu klavesu, ak je to sipka dohora, je to smer dohora 
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

        if (old_x != x || old_y != y) // ak sa povodna poloha zmenila
        {
            FindPath(ex, ey, x, y); // najdi novu cestu medzi duchom a pac-manom
        }

        gotoxy(x, y); // nastavenie na zmenene suradnice
        cout << "H"; // na novych suradniciach vytvori Pac-mana

        map[ey][ex] = '.'; // nastavenie na zmenene suradnice
        gotoxy(ex, ey); // nastavenie na zmenene suradnice
        cout << "."; // vytvori na novych suradniciah bodku
       // cout << frame << endl;
       // cout << rychlost << endl;
        //cout << frame % rychlost << endl;
       // cout << walk_queue.size() << endl;
       // cin.get();

        if (frame % rychlost == 0 && walk_queue.size() != 0) // urcuje, ako rychlo sa duch hybe
        {
            ex = walk_queue.back().x; // zmena x suradnice ducha
            ey = walk_queue.back().y; // zmena y suradnice ducha
            walk_queue.pop_back();    // zmazanie poslednych hodnot x a y vo walk_queue

        }

        gotoxy(ex, ey); // presunie sa na suradnice ex, ey
        cout << "E"; // vytvori na novych suradniciah Ducha

        if (ex == x && ey == y) // ak sa suradnice ex, ey rovanju suradniciam x, y
        {
            break; // koniec hry
        }

        gotoxy(32, 18);
        gotoxy(32, 1);
        cout << skore;
        Sleep(100);
        frame++;
    }

    system("cls");
    printf("You Lose and your score is : %i", skore);
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