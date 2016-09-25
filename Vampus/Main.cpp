#include "Vampus.h"
#include <conio.h>

int main(int argc, char* argv[])
{
	char choise;

	do {
		VampusGame* ptrGame = new VampusGame;
		ptrGame->Action();
		ptrGame->~VampusGame();

		std::cout << "VampusGame is over.Retry?";
		std::cin >> choise;

	} while (choise == 'y');

	//Game.PrintRooms();
	getchar();
}