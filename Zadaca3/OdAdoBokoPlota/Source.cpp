#include <iostream>
#include <queue>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <windows.h>

using namespace std;

const int MAX_ROWS = 20;
const int MAX_COLS = 40;
const int MAX_OBSTACLES = MAX_ROWS * MAX_COLS * 0.02; // 2% od ukupnih polja
const int INF = 99999999;

pair<int, int> getNeighbors[4] = {
    {-1, 0},  // Susjed gore
    {1, 0},   // Susjed dolje
    {0, -1},  // Susjed lijevo
    {0, 1}    // Susjed desno
};

void inputCoordinates(int& startRow, int& startCol, int& endRow, int& endCol) {
    cout << "Unesite redak pocetne tocke A (od 1 do " << MAX_ROWS << "): ";
    cin >> startRow;
    startRow--;

    cout << "Unesite stupac pocetne tocke A (od 1 do " << MAX_COLS << "): ";
    cin >> startCol;
    startCol--;

    cout << "Unesite redak krajnje tocke B (od 1 do " << MAX_ROWS << "): ";
    cin >> endRow;
    endRow--;

    cout << "Unesite stupac krajnje tocke B (od 1 do " << MAX_COLS << "): ";
    cin >> endCol;
    endCol--;
}

bool isValidInput(int startRow, int startCol, int endRow, int endCol) {
    if (startRow < 0 || startRow >= MAX_ROWS || startCol < 0 || startCol >= MAX_COLS ||
        endRow < 0 || endRow >= MAX_ROWS || endCol < 0 || endCol >= MAX_COLS)
    {
        cout << "Neispravan unos. Broj redaka treba biti od 1 do " << MAX_ROWS
            << ", a broj stupaca od 1 do " << MAX_COLS << ".\n";
        return false;
    }

    return true;
}


bool isValidCell(int row, int col)
{
    // Provjerava je li polje unutar granica mape
    return (row >= 0 && row < MAX_ROWS&& col >= 0 && col < MAX_COLS);
}

void clearScreen() // ovako veli internet
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD home = { 0, 0 };
    DWORD numWritten;
    FillConsoleOutputCharacter(hOut, ' ', MAX_ROWS * MAX_COLS, home, &numWritten);
    SetConsoleCursorPosition(hOut, home);
}


// gospodin Dijkstra
vector<pair<int, int>> findPath(int startRow, int startCol, int endRow, int endCol, const char map[MAX_ROWS][MAX_COLS])
{
    // Inicijalizacija mape najkraæih udaljenosti
    int distance[MAX_ROWS][MAX_COLS];
    for (int row = 0; row < MAX_ROWS; ++row)
    {
        for (int col = 0; col < MAX_COLS; ++col)
        {
            distance[row][col] = INF;
        }
    }
    distance[startRow][startCol] = 0;

    // inicijalizacija parenta nodova (za rekonstrukciju puta)
    pair<int, int> parent[MAX_ROWS][MAX_COLS];

    // inicijalizacija prioriteta
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>> pq;
    pq.push(make_pair(0, make_pair(startRow, startCol)));

    while (!pq.empty())
    {
        pair<int, pair<int, int>> current = pq.top();
        pq.pop();

        int row = current.second.first;
        int col = current.second.second;
        int currDistance = -current.first;

        if (row == endRow && col == endCol)
            break;

        // provjera susjednih polja
        for (const auto& neighbor : getNeighbors)
        {
            int neighborRow = row + neighbor.first;
            int neighborCol = col + neighbor.second;

            if (isValidCell(neighborRow, neighborCol) && map[neighborRow][neighborCol] != '#')
            {
                int newDistance = currDistance + 1;
                if (newDistance < distance[neighborRow][neighborCol])
                {
                    distance[neighborRow][neighborCol] = newDistance;
                    parent[neighborRow][neighborCol] = make_pair(row, col);
                    pq.push(make_pair(-newDistance, make_pair(neighborRow, neighborCol)));
                }
            }
        }
    }

    vector<pair<int, int>> path;
    int row = endRow;
    int col = endCol;

    while (row != startRow || col != startCol)
    {
        path.push_back(make_pair(row, col));
        pair<int, int> prev = parent[row][col];
        row = prev.first;
        col = prev.second;
    }

    path.push_back(make_pair(startRow, startCol));
    reverse(path.begin(), path.end());

    return path;
}


void drawMap(const char map[MAX_ROWS][MAX_COLS])
{
    clearScreen();

    for (int row = 0; row < MAX_ROWS; ++row)
    {
        for (int col = 0; col < MAX_COLS; ++col)
        {
            cout << map[row][col] << ' ';
        }
        cout << endl;
    }
}

void initializeMap(char(&map)[MAX_ROWS][MAX_COLS], int startRow, int startCol, int endRow, int endCol) {
    srand(time(0));

    for (int row = 0; row < MAX_ROWS; ++row)
    {
        for (int col = 0; col < MAX_COLS; ++col)
        {
            if (rand() % 100 < 2 && !(row == startRow && col == startCol) && !(row == endRow && col == endCol))
            {
                map[row][col] = '#';
            }
            else
            {
                map[row][col] = '.';
            }
        }
    }
    map[startRow][startCol] = 'A';
    map[endRow][endCol] = 'B';
}

void editChar(char(&map)[MAX_ROWS][MAX_COLS], pair<int, int>& prevCell, char& prevChar, pair<int, int> currentCell) {
    // vrati staro, ako ima
    if (prevCell.first >= 0 && prevCell.second >= 0 && prevChar != '\0') {
        map[prevCell.first][prevCell.second] = prevChar;
    }
    // napuni trenutno za sljedeæi krug
    prevCell = make_pair(currentCell.first, currentCell.second);
    prevChar = map[currentCell.first][currentCell.second];
    // iscrtaj novo
    map[currentCell.first][currentCell.second] = 'X';
}


int main()
{
    int startRow, startCol, endRow, endCol;
    inputCoordinates(startRow, startCol, endRow, endCol);
    if (!isValidInput(startRow, startCol, endRow, endCol))
    {
        return 1;
    }

    char map[MAX_ROWS][MAX_COLS] = {};
    initializeMap(map, startRow, startCol, endRow, endCol);
    vector<pair<int, int>> path = findPath(startRow, startCol, endRow, endCol, map);

    pair<int, int> prevCell = make_pair(-1, -1);
    char prevChar = '\0';
    for (const auto& cell : path)
    {
        editChar(map, prevCell, prevChar, cell);
        drawMap(map);
        Sleep(100);
    }
    drawMap(map);
    cout << "Dostignuta tocka B!" << endl;

    return 0;
}
