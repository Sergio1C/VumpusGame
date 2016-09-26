#include "Vampus.h"
#include <conio.h>

int main(int argc, char* argv[])
{
	char choise;

	do {
		system("cls");
		VumpusGame* pGame = new VumpusGame;
		pGame->Action();
		pGame->~VumpusGame();

		std::cout << "\nGame is over.Retry? (y,n)";
		std::cin >> choise;

	} while (choise == 'y');

	//Game.PrintRooms();
	getchar();
}