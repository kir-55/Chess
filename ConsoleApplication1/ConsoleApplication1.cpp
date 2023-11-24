// ConsoleApplication1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <stdlib.h> 
#include <vector>

using namespace std;

char board[8][8] = {
					'f'
					};
char pawn = '*';
char rock = '#';
char knight = '@';
char bishop = '&';
char king = '^';
char qween = '$';



void ResetBoard() {
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++) {
			if (y == 1 or y == 6)
				board[y][x] = pawn;
			else if (y == 0 or y == 7) {
				if (x == 0 or x == 7)
					board[y][x] = rock;
				else if (x == 1 or x == 6)
					board[y][x] = knight;
				else if (x == 2 or x == 5)
					board[y][x] = bishop;
				else if (x == 3)
					board[y][x] = king;
				else if (x == 4)
					board[y][x] = qween;
			}
			else
				board[y][x] = ' ';

		}
	}
}

void DrawBoard() {
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++) {
			if (board[y][x] != ' ')
				cout << board[y][x];
			else if ((x + y) % 2 == 0)
				cout << "X";
			else
				cout << "O";
		}
		cout << endl;
	}
}

bool isValidPos(int x, int y)
{
	if (x > -1 and x < 8 and y > -1 and y < 8)
		return true;
	return false;
}

bool moveTo(int sx, int sy, int fx, int fy) 
{
	if (isValidPos(sx, sy) and isValidPos(fx, fy) and board[sy][sx] == ' ' and board[fy][fx] == ' ')
		return false;

	char currPiece = board[sy][sx];
	board[sy][sx] = ' ';
	board[fy][fx] = currPiece;
		
	
}
 
int main()
{
	bool exit = false;
	int startPosX, startPosY, endPosX, endPosY;
	ResetBoard();
	while (true) {
		
		DrawBoard();
		cout << "Move from (x pos)" << endl;
		cin >> startPosX;
		cout << "Move from (y pos)" << endl;
		cin >> startPosY;
		cout << "Move to (x pos):" << endl;
		cin >> endPosX;
		cout << "Move to (y pos):" << endl;
		cin >> endPosY;
		system("cls");
		if (!moveTo(startPosX - 1, 8 - startPosY, endPosX - 1, 8 - endPosY))
			cout << "Invalid position!" << endl;
	}
}





//int height = 10;
//int width = 10;
//char sign;
//
//int main()
//{
//	cin >> height;
//	cin >> width;
//	cin >> sign;
//	for (int y = 0; y <= height; y++)
//	{
//		for (int x = 0; x <= width; x++) {
//			if (x == 0 or x == width or y == 0 or y == height)
//				cout << sign;
//			else
//				cout << " ";
//		}
//		cout << endl;
//	}
//}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
