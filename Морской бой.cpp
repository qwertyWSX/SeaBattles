// Морской бой.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <ctime>
using namespace std;

const int error = 0xFF;
const int EMPTY = 0;
const int NEARship = 9;
const int SHIP = 1;
const int MISSED = 2;
const int HIT = 3;
const int DESTROYED = 4;

int logX = 80;
int logY = 5;

int allyFleet[10][10] = { }; //a - строки, b - столбцы allyFleet[a][b]!!! a = pos % 10, b = pos/10
int allyShots[10][10] = { };
int enemyFleet[10][10] = { };
int enemyShots[10][10] = { };

//переместить курсор на заданную позицию
void putCursorPos(int x, int y)
{
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD point;
    point.X = x;
    point.Y = y;
    SetConsoleCursorPosition(hout, point);
}
//определение положения курсора
int getXcoord()
{
    CONSOLE_SCREEN_BUFFER_INFO info_x;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_x);
    return info_x.dwCursorPosition.X;
}
int getYcoord()
{
    CONSOLE_SCREEN_BUFFER_INFO info_y;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_y);
    return info_y.dwCursorPosition.Y;
}
//проверка ввода ДА/НЕТ
bool yesNoInput()
{
    while (true)
    {
        int x0 = getXcoord();
        int y0 = getYcoord();
        string str;
        cin >> str;
        if (str.size() == 1 && str[0] == '1') return 1;
        else if (str.size() == 1 && str[0] == '0') return 0;
        else
        {
            putCursorPos(x0, y0);
            for (unsigned int i = 0; i <= str.size(); i++) { cout << " "; } //Затирание введенных символов
            putCursorPos(x0, y0);
        }
    }
}

// нарисовать поле боя 
void drawMap()
{
    const int I = 1; // для отрисовка только своего поля I = 1, чтобы было видно поле противника I=3

    int x, y;
    int plan[10][10] = {};

    for (int i = 0; i <= I; i++)
    {
        int x0 = getXcoord();
        int y0 = getYcoord();

        switch (i)
        {
        case 0:
            x = 0;
            y = 5;
            for (int i = 0; i < 100; i++)
            {
                plan[i / 10][i % 10] = allyFleet[i / 10][i % 10];
            }
            break;

        case 1:
            x = 40;
            y = 5;
            for (int i = 0; i < 100; i++)
            {
                plan[i / 10][i % 10] = allyShots[i / 10][i % 10];
            }
            break;

        case 2:
            x = 0;
            y = 35;
            for (int i = 0; i < 100; i++)
            {
                plan[i / 10][i % 10] = enemyFleet[i / 10][i % 10];
            }
            break;

        case 3:
            x = 40;
            y = 35;
            for (int i = 0; i < 100; i++)
            {
                plan[i / 10][i % 10] = enemyShots[i / 10][i % 10];
            }
            break;
        }
        putCursorPos(x, y);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //для подключения цветов в косоль
        SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); // изменение цвета шрифта на зеленый
        cout << "    А Б В Г Д Е Ж З И К";
        putCursorPos(x, y + 1);
        cout << "  =======================";
        for (int i = 1; i < 10; i++)            //выввод наименования строк с 1 по 9
        {
            putCursorPos(x, y + 1 + i);
            cout << " " << i << "| ";
            putCursorPos(x + 24, y + 1 + i); cout << "|";
        }
        putCursorPos(x, y + 11);
        cout << "10| ";                       //выввод намименования 10-й строки
        putCursorPos(x + 24, y + 11);
        cout << "|";
        putCursorPos(x, y + 12);
        cout << "  ";
        for (int i = 0; i <= 22; i++)      //выввод нижней разделительной строки
        {
            cout << "=";
        }

        for (int i = 0; i < 10; i++)       //прорисовака основного поля
        {
            for (int j = 0; j < 10; j++)
            {
                putCursorPos(x + 4 + j * 2, y + 2 + i);;
                switch (plan[i][j])
                {
                case EMPTY: 
                    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); // изменение цвета шрифта на зеленый
                    cout << char(183) << " ";  
                    break;                      //пусто
                case NEARship:                        //клетка соседняя с кораблем 
                    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); // изменение цвета шрифта на зеленый
                    cout << char(183) << " ";
                    break;
                case SHIP: 
                    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 1)); // изменение цвета шрифта на синий
                    cout << char(127) << " "; // корабль
                    break;
                case MISSED: 
                    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6)); // изменение цвета шрифта на желтый
                    cout << char(111) << " "; // мимо
                    break;
                case HIT: 
                    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 5)); // изменение цвета шрифта на фиолетовый
                    cout << char(043) << " "; // ранил
                    break;
                case DESTROYED: 
                    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 4)); // изменение цвета шрифта на красный
                    cout << char(245) << " "; // убил
                    break;
                }
            }
        }
        SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15)); // изменение цвета шрифта на белый
        putCursorPos(x0, y0);
    }
}
// очистка значений в массивах
void clearData()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            allyFleet[i][j] = 0;
            enemyFleet[i][j] = 0;
            allyShots[i][j] = 0;
            enemyShots[i][j] = 0;
        }
    }
}
// очистки экрана
void clearScreen()
{
    int y = getYcoord();
    putCursorPos(0, 5);
    for (int i = 0; i <= y; i++)
    {
        cout << "                                                                  \n";
    }
}
// очистка лога
void clearLog()
{
    putCursorPos(logX, 0);
    for (int i = 0; i <= logY; i++)
    {
        putCursorPos(logX, i);
        cout << "                                       \n";
    }
}
// преобразование символов А, Б, В и т.д. в координату массива (error - ошибка)
int getChar(char a)
{
    switch (a) 
    {
    case 'А':
    case 'а':
         return (0);
         break;
    case 'Б':
    case 'б':
        return (1);
        break;
    case 'В':
    case 'в':
        return (2);
        break;
    case 'Г':
    case 'г':
        return (3);
        break;
    case 'Д':
    case 'д':
        return (4);
        break;
    case 'Е':
    case 'е':
        return (5);
        break;
    case 'Ж':
    case 'ж':
        return (6);
        break;
    case 'З':
    case 'з':
        return (7);
        break;
    case 'И':
    case 'и':
        return (8);
        break;
    case 'К':
    case 'к':
        return (9);
        break;
    default:
        return error;
        break;
    }
} 
// проверка правильности постановки 1,2,3 и 4 ячейки корабля
bool checkShipPos(int * shipPos, int k, int (&fleet)[10][10])  //k определяет какую координату проверяем начиная с 0
{
    int a = shipPos[k] % 10;
    int b = shipPos[k] / 10;

switch (k)
{
case 0:
    if (fleet[a][b] != 0) return false;
    else  return true;
    break;

case 1:
    if (fleet[a][b] != 0) return false;
    else if (shipPos[1]%10 == shipPos[0]%10)
    {
        if (abs(shipPos[1] - shipPos[0]) == 10) return true;
        else return false;
    }
    else if (shipPos[1] / 10 == shipPos[0] / 10)
    {
        if (abs(shipPos[1] - shipPos[0]) == 1) return true;
        else return false;
    }      
    else return false;
    break;

case 2:
case 3:
    if (fleet[a][b] != 0) return false;
    else if ((shipPos[1] % 10 == shipPos[0] % 10) && (shipPos[k] % 10 == shipPos[0] % 10))
    {
        if ((abs(shipPos[k] - shipPos[0]) == 10) || (abs(shipPos[k] - shipPos[k-1]) == 10)) return true;
        else return false;
    }
    else if ((shipPos[1] / 10 == shipPos[0] / 10) && (shipPos[k] / 10 == shipPos[0] / 10))
    {
        if ((abs(shipPos[k] - shipPos[0]) == 1) || (abs(shipPos[k] - shipPos[k - 1]) == 1)) return true;
        else return false;
    }
    else return false;
    break;
}
return false;
}
//постановка символов вокруг корабля
void putSymAroundShip(int *shipPos, int shipSize, int(&fleet)[10][10])
{
    int i1, i2, j1, j2;
    for (int k = 0; k < shipSize; k++)
    {
        int x = shipPos[k] % 10;
        int y = shipPos[k] / 10;
        (x == 0) ? i1 = 0 : i1 = -1;
        (x == 9) ? i2 = 0 : i2 = 1;
        (y == 0) ? j1 = 0 : j1 = -1;
        (y == 9) ? j2 = 0 : j2 = 1;

        for (int i = i1; i <= i2; i++)
        {
            for (int j = j1; j <= j2; j++)
            {
               if (fleet[x+i][y+j] == 0) fleet[x+i][y+j] = NEARship;
            }
        }
    }
}
// команда на выполнение ввода, проверка ввода и вовращает адрес для одномерного массива 
int inputSym()
{
    int x0 = getXcoord();
    int y0 = getYcoord();

    while (true)
    {
        string str;
        cin >> str;
        
            switch (str.size())
            {
            case 1:
                if (str[0] == '0')
                {
                    cout << "                                ";
                    return (error - 1); //Для повторного ввода
                }

            case 2:
                if (getChar(str[0]) != error && int(str[1]) >= 49 && int(str[1]) <= 57)
                {
                   // cout << "                                ";
                    return (10*getChar(str[0])+int(str[1])-49);
                }          

            case 3:
                if (getChar(str[0]) != error && int(str[1]) == 49 && int(str[2]) == 48)
                {
                    //cout << "                                ";
                    return (10*getChar(str[0]) + 9);
                }
            }      
        cout << "Вы ввели неверное значение!                                                  \a";
        putCursorPos(x0, y0);
        for (unsigned int i = 0; i <= str.size(); i++) { cout << " "; } //Затирание введенных символов
        putCursorPos(x0, y0);
        return error;
    }
}
//постановка корабля и проверка правильности координаты
void putShip(int shipSize) //размещение корабля, shipSize - кол-во палуб
{
    int side = 1; //для передачи в checkShipPos
    int *shipPos = new int[shipSize]; //pos[]- линейный массив с координатами корабля

    for (int i = 0; i < shipSize; i++)
    {
        cout << "Введите координату " << i + 1 << "-й ячейки " << shipSize << "-палубного корабля: ";
        int x = getXcoord();
        int y = getYcoord();
        while (true)
        {
            shipPos[i] = inputSym();
            if (shipPos[i] == error - 1) //удаление введеных ранее координат
            {
                for (int z = 1; z <= i; z++)
                {
                    int a = shipPos[z-1] % 10;
                    int b = shipPos[z-1] / 10;
                    allyFleet[a][b] = 0;
                    shipPos[0] = 0;
                }
                putCursorPos(x, y);
                drawMap();
                i = 0;
                cout << endl;
                cout << "Введите координату " << i + 1 << "-й ячейки " << shipSize << "-палубного корабля: ";
            }
            else if (!checkShipPos(shipPos, i, allyFleet))
            {
                cout << "В этой ячейке корабль разместить нельзя!\a";
                putCursorPos(x, y);
                cout << "    ";
                putCursorPos(x, y);
            }
            else
            {
                int a = shipPos[i] % 10;
                int b = shipPos[i] / 10;
                allyFleet[a][b] = 1;
                drawMap();
                break;
            }
        }
    }  
    putSymAroundShip(shipPos, shipSize, allyFleet);
    delete[] shipPos;
}
//Растановка кораблей на поле и запись данных в массив
void shipPlacement()
{ 
    int x0 = getXcoord();
    int y0 = getYcoord();

    for ( int j = 1 ; j <= 4; j++)
    {     
        for (int i =1 ; i <= j; i++)
        {
            cout << "\nПодсказка: Для размещения корабля введите координату формата 'А1'\n";
            cout << "           Для сброса введеных ранее координат введите '0'\n\n";
            putShip(5 - j);
            cout << "                                         \n";
            clearScreen();
            drawMap();
            putCursorPos(x0, y0);
        }
    }
}

// Автоматическая постановка корабля 
void autoShipPlacement(int *shipPos , int shipDeck, int(&fleet)[10][10])
{
    int direction;
    bool err;

    do 
    {
        err = false;
        do //определение местоположения
        {
            shipPos[0] = 0 + rand() % 100;
        }   while (!checkShipPos(shipPos, 0, fleet));
        if (shipDeck == 1) break;
        direction = 0 + rand() % 2;       //определение направления 0- горизонтально, 1-вертикально
        for (int i = 1; i < shipDeck; i++)
        {
            if (direction==1) shipPos[i] = shipPos[0] + i * 10;
            else shipPos[i] = shipPos[0] + i;

            if (shipPos[i] > 99)
            {
                err = true;
                break;
            }

            if (!checkShipPos(shipPos, i, fleet))
            {
                err = true;
                break;
            }
        }
    } while (err);
}
// автоматическая расстановка кораблей
void autoPlacement(int(&fleet)[10][10])
{
    int a, b;

    for (int i = 4; i > 0; i--)
    {
        int *shipPos = new int[i];
        for (int j = 1; j <= 5 - i; j++)
        {
            autoShipPlacement(shipPos, i, fleet);
            for (int k = 0; k < i; k++)
            {
                a = shipPos[k] % 10;
                b = shipPos[k] / 10;
                fleet[a][b] = 1;
            }
            putSymAroundShip(shipPos, i, fleet);
        }
        delete [] shipPos;
    }

}

//преобразование pos в координату формата А1
string convertPos(int pos)
{
    int a = pos % 10;
    int b = pos / 10;
    string str1;
    string str2;

    switch (b)
    {
    case 0:
       str1 = "А";
        break;
    case 1:
        str1 = "Б";
        break;
    case 2:
        str1 = "В";
        break;
    case 3:
        str1 = "Г";
        break;
    case 4:
        str1 = "Д";
        break;
    case 5:
        str1 = "Е";
        break;
    case 6:
        str1 = "Ж";
        break;
    case 7:
        str1 = "З";
        break;
    case 8:
        str1 = "И";
        break;
    case 9:
        str1 = "К";
        break;
    }
    switch (a)
    {
    case 0:
        str2 = "1";
        break;
    case 1:
        str2 = "2";
        break;
    case 2:
        str2 = "3";
        break;
    case 3:
        str2 = "4";
        break;
    case 4:
        str2 = "5";
        break;
    case 5:
        str2 = "6";
        break;
    case 6:
        str2 = "7";
        break;
    case 7:
        str2 = "8";
        break;
    case 8:
        str2 = "9";
        break;
    case 9:
        str2 = "10";
        break;
    }

    return (str1 + str2);
}
// запись ходов
void log(string str)
{
    int x0 = getXcoord();
    int y0 = getYcoord();
    putCursorPos(logX, logY);
    cout << str;
    logY++;
    putCursorPos(x0, y0);
}
//Проверка уничтожен ли корабль
bool checkShipDestroyed(int pos, int(&fleet)[10][10], int(&shots)[10][10])
{
    int a = pos % 10;
    int b = pos / 10;

    // проверка наличия не уничтоженных клеток 
    for (int i = 1; i < 4; i++)
    {
        if (a + i > 9) break;
        if (fleet[a + i][b] == SHIP) return false;
        else if (fleet[a + i][b] != HIT) break;
    }
    for (int i = 1; i < 4; i++)
    {
        if (a - i < 0) break;
        if (fleet[a - i][b] == SHIP) return false;
        else if (fleet[a - i][b] != HIT) break;
    }
    for (int i = 1; i < 4; i++)
    {
        if (b + i > 9) break;
        if (fleet[a][b+i] == SHIP) return false;
        else if (fleet[a][b+i] != HIT) break;
    }
    for (int i = 1; i < 4; i++)
    {
        if (b - i < 0) break;
        if (fleet[a][b - i] == SHIP) return false;
        else if (fleet[a][b - i] != HIT) break;
    }

    // присвоение клеткам с ранения - значения "уничтожения" если корабля уничтожен
    for (int i = 0; i < 4; i++)
    {
        if (fleet[a + i][b] == 3) 
        { 
            fleet[a + i][b] = 4; 
            shots[a + i][b] = 4;
        }
        else break;
    }
    for (int i = 1; i < 4; i++)
    {
        if (fleet[a - i][b] == 3)
        {
            fleet[a - i][b] = 4;
            shots[a - i][b] = 4;
        }
        else break;
    }
    for (int i = 1; i < 4; i++)
    {
        if (fleet[a][b + i] == 3)
        {
            fleet[a][b + i] = 4;
            shots[a][b + i] = 4;
        }
        else break;
    }
    for (int i = 1; i < 4; i++)
    {
        if (fleet[a][b - i] == 3)
        {
            fleet[a][b - i] = 4;
            shots[a][b - i] = 4;
        }
        else break;
    }
    return true;
}
//Ход игрока
bool shootPlayer()
{
    int pos;
    string str;

        cout << "Ваш ход: ";
        int x0 = getXcoord();
        int y0 = getYcoord();
        do
        {
          pos = inputSym();
        } while (pos == error);
        cout << "                                                    ";
        str = convertPos(pos);

    putCursorPos(x0 + 5, y0);
    if (enemyFleet[pos % 10][pos / 10] == 1)
    {
       enemyFleet[pos % 10][pos / 10] = 3;
       allyShots[pos % 10][pos / 10] = 3;
       if (checkShipDestroyed(pos, enemyFleet, allyShots))
       {
           cout << "Убил!!!";
           log("Ваш ход: " + str + " Убил!!!");
           return true;
       }
       else 
       { 
           cout << "Ранил";
           log("Ваш ход: " + str + " Ранил");
           return true;
       }
    
    }
    else
    {
        if (allyShots[pos % 10][pos / 10] == 0) allyShots[pos % 10][pos / 10] = 2;
        log("Ваш ход: " + str + " Мимо");
        cout << " Мимо";
        cout << endl;
        return false;
    }
}

//Если ИИ ранил корабль попытка его добить
int shootAIonWounded(int pos)
{
    int a = pos % 10;
    int b = pos / 10;
    int var = 0;                                     //определяет сколько существует вариантов для стрельбы
    int varPos[4] = { error, error, error, error };   //для записи позиций для стрельбы вариантов для стрельбы
    bool horizont = false;    //флаг что корабль расположен горизонтально
    bool vertical = false;    //флаг что корабль расположен вертикально

    for (int i = 1; i < 4; i++)
    {
        if (b - i < 0) break;
        if (enemyShots[a][b-i] == EMPTY)
        {
            varPos[0] = a + (b - i) * 10;
            break;
        }
        else if (enemyShots[a][b-i] == HIT) horizont = true;
        else break;
    }

    for (int i = 1; i < 4; i++)
    {
        if (b + i > 9) break;
        if (enemyShots[a][b+i] == EMPTY)
        {
            varPos[1] = a + (b + i) * 10;
            break;
        }
        else if (enemyShots[a][b+i] == HIT) horizont = true;
        else break;
    }

    if (!horizont)
    {
        for (int i = 1; i < 4; i++)
        {
            if (a - i < 0) break;
            if (enemyShots[a-i][b] == EMPTY)
            {
                varPos[2] = (a-i) + b*10;
                break;
            }
            else if (enemyShots[a-i][b] == HIT) vertical = true;
            else break;
        }

        for (int i = 1; i < 4; i++)
        {
            if (a + i > 9) break;
            if (enemyShots[a+i][b] == EMPTY)
            {
                varPos[3] = (a + i) + b*10;
                break;
            }
            else if (enemyShots[a+i][b] == HIT) vertical = true;
            else break;
        }
    }

    if (horizont)
    {
        varPos[2] = error;
        varPos[3] = error;
    }

    if (vertical)
    {
        varPos[0] = error;
        varPos[1] = error;
    }

    for (int i = 0; i < 4; i++)
    {
        if (varPos[i] != error)
        {
            varPos[var] = varPos[i];
            var++;
        }
    }
    return varPos[rand() % var];
}
//Простановка символов вокруг уничтоженного корабля для AI
void putSymArondDestroyedShipAI(int pos)
{
    int a = pos % 10;
    int b = pos / 10;
    int shipSize = 0;
    int shipPosBuffer[4]; //буферный массив

    // заполнение массива 

    shipPosBuffer[shipSize] = a + b * 10;
    shipSize++;
    for (int i = 1; i < 4; i++)
    {
        if ((enemyShots[a + i][b] == DESTROYED) && (a + i <= 9))
        {
            shipPosBuffer[shipSize] = (a + i) + b * 10;
            shipSize++;
        }
        else break;
    }
    for (int i = 1; i < 4; i++)
    {
        if ((enemyShots[a - i][b] == DESTROYED) && (a - i >= 0))
        {
            shipPosBuffer[shipSize] = (a - i) + b * 10;
            shipSize++;
        }
        else break;
    }
    for (int i = 1; i < 4; i++)
    {
        if ((enemyShots[a][b + i] == DESTROYED) && (b + i <= 9))
        {
            shipPosBuffer[shipSize] = a + (b + i) * 10;
            shipSize++;
        }
        else break;
    }
    for (int i = 1; i < 4; i++)
    {
        if ((enemyShots[a][b - i] == DESTROYED) && (b - i >= 0))
        {
            shipPosBuffer[shipSize] = a + (b - i) * 10;
            shipSize++;
        }
        else break;
    }

    int* shipPos = new int[shipSize];
    for (int i = 0; i < shipSize; i++) shipPos[i] = shipPosBuffer[i];
    putSymAroundShip(shipPos, shipSize, enemyShots);
    delete[] shipPos;
}
//Ход ИИ
bool shootAI()
{
    bool flag = false;
    int pos;
    string str;
    int num0=0; // количество 0 в массиве
    int arr[100]; //буферный массив
    for (int i = 0; i < 100; i++) arr[i] = 0xFF;

    int x1 = getXcoord();
    int y1 = getYcoord();
    cout << "                                                                           ";
    putCursorPos(x1, y1);

    cout << "Ход противника: ";
    int x0 = getXcoord();
    int y0 = getYcoord();
   
    for (int i = 0; i < 100; i++)
    {
        if (enemyShots[i % 10][i / 10] == EMPTY)
        {
            arr[num0] = i;
            num0++;
        }
        else if (enemyShots[i % 10][i / 10] == HIT)
        {
            pos = shootAIonWounded(i);
            flag = true;
            break;
        }
    }

    if (!flag) pos = arr[rand() % num0];

    str = convertPos(pos);
    cout << str;

    putCursorPos(x0 + 5, y0);
    if (allyFleet[pos % 10][pos / 10] == SHIP)
    {
        allyFleet[pos % 10][pos / 10] = HIT;
        enemyShots[pos % 10][pos / 10] = HIT;
        if (checkShipDestroyed(pos, allyFleet, enemyShots))
        {
            drawMap();
            cout << "Убил!!! (для продолжения введите '1'): ";
            yesNoInput();
            log("Ход противника: " + str + " Убил!!!");
            putSymArondDestroyedShipAI(pos);
            return true;
        }
        else
        {
            drawMap();
            cout << "Ранил (для продолжения введите '1'): ";
            yesNoInput();
            log("Ход противника: " + str + " Ранил");
            return true;
        }

    }
    else
    {
        if (enemyShots[pos % 10][pos / 10] == EMPTY) 
        {
            enemyShots[pos % 10][pos / 10] = MISSED;
            allyFleet[pos % 10][pos / 10] = MISSED;
        }
        drawMap();
        cout << "Мимо (для продолжения введите '1'): ";
        yesNoInput();
        log("Ход противника: " + str + " Мимо");
        return false;
    }
}


int main()
{
    //setlocale(LC_ALL, "rus");
    SetConsoleCP(1251); // Ввод с консоли в кодировке 1251
    SetConsoleOutputCP(1251); // Вывод на консоль в кодировке 1251.
    srand(time(0)); // Для работы функции рандом

    int allyShips = 20;
    int enemyShips = 20;
    const int X = 0;
    const int Y = 20;
    int turn;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // подключение цветов в консоль

        cout << "   Морской бой ver.1" << endl;
        cout << "======================";
        while (true)
        {
            while (true)
            {
                drawMap();
                putCursorPos(X, Y);
                cout << "Расстановка кораблей:\n";

                cout << "Расставить корабли автоматически? (1-ДА, 0-НЕТ): ";
                if (yesNoInput())
                {
                    autoPlacement(allyFleet);
                    drawMap();
                }
                else
                {
                    clearScreen();
                    drawMap();
                    putCursorPos(X, Y);
                    shipPlacement();
                }
                cout << "Расставить заново? (1-ДА, 0-НЕТ): ";
                if (!yesNoInput())
                {
                    clearScreen();
                    break;
                }
                else
                {
                    clearData();
                    clearScreen();
                }
            }

            autoPlacement(enemyFleet); //постановка кораблей ИИ
            drawMap();
            turn = rand() % 2;      //1 - ход игрока, 0 - ход ИИ  

            putCursorPos(X, Y);
            while (true)
            {
                if (turn)
                {
                    if (shootPlayer())
                    {
                        enemyShips--;
                        turn = 1;
                        cout << endl;
                        drawMap();
                        if (enemyShips == 0)
                        {
                            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 1));
                            cout << "\nФлот противника уничтожен! Вы победили!!!";
                            break;
                        }
                    }
                    else
                    {
                        drawMap();
                        turn = 0;
                    }
                }
                else
                {
                    if (shootAI())
                    {
                        turn = 0;
                        allyShips--;
                        if (allyShips == 0)
                        {
                            for (int i = 0; i < 100; i++) //показать оставшиеся корабли противника;
                            {
                                if (enemyFleet[i % 10][i / 10] == SHIP) allyShots[i%10][i/10] = SHIP;
                            }
                            drawMap();
                            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 4));
                            cout << "\nВыш флот уничтожен! Вы проиграли!!!";
                            break;
                        }
                    }
                    else
                    {
                        clearScreen();
                        drawMap();
                        putCursorPos(X, Y);
                        turn = 1;
                    }
                }
            }

            cout << "\nВы хотите начать заново? (1-ДА, 0-НЕТ): ";
            if (yesNoInput())
            {
                clearData();
                clearScreen();
                clearLog();
                logX = 80;
                logY = 5;

                allyShips = 20;
                enemyShips = 20;
            }
            else break;
        }
    }