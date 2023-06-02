#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

const int MAX_ROWS = 20;
const int MAX_COLS = 40;


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

vector<pair<int, int>> findPath(int startRow, int startCol, int endRow, int endCol)
{
    vector<pair<int, int>> path;
    if (endRow > startRow) {
        for (int i = startRow + 1; i < endRow; i++) {
            path.push_back({ i, startCol });
        }
    }
    if (endRow < startRow) {
        for (int i = startRow - 1; i > endRow; i--) {
            path.push_back({ i, startCol });
        }
    }

    if (endCol > startCol) {
        path.push_back({ endRow, startCol });
        for (int i = startCol + 1; i <= endCol; i++) {
            path.push_back({ endRow, i });
        }
    }
    if (endCol < startCol) {
        path.push_back({ endRow, startCol });
        for (int i = startCol - 1; i >= endCol; i--) {
            path.push_back({ endRow, i });
        }
    }

    return path;
}


void drawMap(int currentRow, int currentCol, int startRow, int startCol, int endRow, int endCol)
{
    clearScreen();

    for (int row = 0; row < MAX_ROWS; ++row)
    {
        for (int col = 0; col < MAX_COLS; ++col)
        {
            if (row == startRow && col == startCol)
                cout << "A ";
            else if (row == currentRow && col == currentCol)
                cout << "X ";
            else if (row == endRow && col == endCol)
                cout << "B ";
            else
                cout << ". ";
        }
        cout << '\n';
    }
}


int main()
{
    int startRow, startCol, endRow, endCol;
    inputCoordinates(startRow, startCol, endRow, endCol);
    if (!isValidInput(startRow, startCol, endRow, endCol))
    {
        return 1;
    }

    vector<pair<int, int>> path = findPath(startRow, startCol, endRow, endCol);
    for (const auto& cell : path)
    {
        drawMap(cell.first, cell.second, startRow, startCol, endRow, endCol);
        Sleep(100);
    }

    cout << "Dostignuta tocka B!";

    return 0;
}