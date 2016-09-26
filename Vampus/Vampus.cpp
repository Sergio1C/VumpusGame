#include "Vampus.h"

void VumpusGame::CreateMap()
{
	//—троим уровень, состо€щий из 20 комнат
	//—оедин€ем первые 5 комнат
	Room* prevRooms = nullptr;

	for (int num = 1; num <= 5; num++)
	{
		Rooms.push_back(new Room(num));
		if (prevRooms)
		{
			CreateRoute(prevRooms, Rooms.back());
		}
		prevRooms = Rooms.back();
	}
	CreateRoute(prevRooms, Rooms.front());

	prevRooms = nullptr;
	//—оедин€ем следующие 10 комнат
	for (int num = 6; num <= 15; num++)
	{
		Rooms.push_back(new Room(num));
		if (prevRooms)
		{
			CreateRoute(prevRooms, Rooms.back());
		}
		prevRooms = Rooms.back();
	}
	CreateRoute(prevRooms, Rooms.at(5));

	prevRooms = nullptr;
	//—оедин€ем последние 5 комнат
	for (int num = 16; num <= 20; num++)
	{
		Rooms.push_back(new Room(num));
		if (prevRooms)
		{
			CreateRoute(prevRooms, Rooms.back());
		}
		prevRooms = Rooms.back();
	}
	CreateRoute(prevRooms, Rooms.at(15));

	prevRooms = nullptr;
	//—оедин€ем комнаты 3 маршрутом друг с другом
	for (std::vector<Room*>::iterator it = Rooms.begin(); it < Rooms.end(); ++it)
	{
		if ((*it)->AllRoute()) continue;
		for (std::vector<Room*>::iterator it2 = it + 1; it2 < Rooms.end(); ++it2)
		{
			if ((*it2)->AllRoute()) continue;
			if (IsRoute((*it), (*it2))) continue;
			CreateRoute((*it), (*it2));
			break;
		}

	};
	//–асставл€ем случайно €мы в 2 случайные комнаты
	RandomRoom()->hole = true;
	RandomRoom()->hole = true;
}

//main game loop
void VumpusGame::Action()
{
	char action;
	int number;

	while (1)
	{
		if (!Check()) break;

		std::cout << "You rooms:" << Player.Position << std::endl;
		std::cout << "Nearest rooms:" << Player.Position->route[0] << " " << Player.Position->route[1] << " " << Player.Position->route[2] << std::endl;
		std::cout << "Shoot or move (s,m)?:" << std::endl;
		std::cin >> action;
		switch (action)
		{
		case 'm':
		{
			std::cout << "You move in:";
			std::cin >> number;
			if (IsRoute(Player.Position, Rooms.at(number - 1)))
			{
				Player.Move(Rooms.at(number - 1));
				Vumpus.Move();
				break;
			}
		}
		case 's':
		{
			Shoot();
			if (!GameOver)
				Vumpus.Move();
			break;
		}
		default:
		{
			std::cout << "Wrong input, try again:";
			continue;
		}
		}
	}

}

bool VumpusGame::Check()
{
	//мы попали в комнату с €мой
	if (Player.Position->hole)
	{
		std::cout << "Oops...you are in hole.Game over!" << std::endl;
		GameOver = 1;
	}
	//€ма р€дом
	if (Player.Position->route[0]->hole || Player.Position->route[1]->hole || Player.Position->route[2]->hole)
	{
		std::cout << "I feel a breeze..." << std::endl;
	}
	//нас съедает Vumpus
	if (Player.Position == Vumpus.Position)
	{
		std::cout << "Oops...You ate the Vumpus.Game Over!" << std::endl;
		GameOver = 1;
	}
	//Vumpus р€дом
	if (Player.Position->route[0] == Vumpus.Position || Player.Position->route[1] == Vumpus.Position
		|| Player.Position->route[2] == Vumpus.Position)
	{
		std::cout << "I feel a Vumpus!" << std::endl;
	}
	//стрелы закончились, Vumpus жив
	if (!Player.getAmmo())
	{
		std::cout << "You dont got a Vumpus!Game over!" << std::endl;
		GameOver = 1;
	}
	if (GameOver)
	{
		std::cout << "------STATISTICS---------\n";
		std::cout << "You room:" << Player.Position->number << std::endl;
		std::cout << "Vumpus room:" << Vumpus.Position->number << std::endl;
		std::stringstream ss;
		for (auto Room : Rooms)
		{
			if (Room->hole) ss << Room->number << " ";
		}
		std::cout << "Holes rooms:" << ss.rdbuf() << std::endl;
		return 0;
	}


	return 1;
}

void VumpusGame::PrintRooms() const
{
	std::ostringstream ss;
	std::vector<Room*>::const_iterator it;
	for (it = Rooms.begin(); it != Rooms.end(); ++it)
	{
		ss << "(" << (*it)->number << ")->";
		for (int i = 0; i < 3; i++)
		{
			Room* route = (*it)->route[i];
			if (route) ss << route->number << " ";
		}
	}

	ss << std::endl;
	//clear ss
	std::cout << ss.str();
	ss.str("");
	ss.flush();

}

//addition metods
bool CreateRoute(Room* One, Room* Two)
{
	for (int i = 0; i < 3; i++)
	{
		if (One->route[i]) continue;

		for (int j = 0; j < 3; j++)
		{
			if (Two->route[j]) continue;

			One->route[i] = Two;
			Two->route[j] = One;
			return 1;
		}
	}
	return 0;
}

bool IsRoute(const Room* One, const Room* Two)
{
	for (int i = 0; i < 3; i++)
	{
		if ((One->route[i] == Two) || (Two->route[i] == One)) return 1;
	}
	return 0;
}

/*¬ыстрел игрока -
1.запрос номеров комнат, которые должна пролететь стрела
2.стрела не может возвратитьс€ к игроку
3. если указана комната при (1), не св€занна€ туннелем с предыдущей, то стрела улетает в случайную комнату
*/
void VumpusGame::Shoot()
{
	Player.IncreaseAmmo();

	int inputLen;
	int number;
	Room* PlayerRoom = Player.Position;
	Room* VumpusRoom = Vumpus.Position;
	std::vector<const Room*> ArrowRoute;

	std::cout << "\nNumber of rooms (1-5)? ";
	do
		std::cin >> inputLen;
	while (inputLen < 1 || inputLen > MAX_ROOM_FOR_AMMO);

	for (int i = 0; i < inputLen; i++)
	{
		std::cout << "Input " << inputLen << " room number for arrow:";
		std::cin >> number;

		Room* ArrowRoom = Rooms.at(number - 1);
		//перва€ комната дл€ стрелы должна быть св€зана с комнатой игрока
		if (i == 0 && !IsRoute(PlayerRoom, ArrowRoom))
		{
			i--; continue;
		}
		ArrowRoute.emplace_back(ArrowRoom);
	}

	const Room* ArrowRoom = nullptr;
	for (auto i = 0; i < ArrowRoute.size(); i++)
	{
		//если комната не св€зана с предыдущей, стрела летит в случайную клетку.
		//остальной маршрут прекращаем.
		if (ArrowRoom && !IsRoute(ArrowRoom, ArrowRoute[i]))
		{
			ArrowRoom = RandomRoom();
			i = ArrowRoute.size();
		}
		ArrowRoom = ArrowRoute[i];

		if (ArrowRoom == VumpusRoom)
		{
			std::cout << "\nCongratulations! You win! Your arrow got a Vampus." << std::endl;
			GameOver = 1;
			return;
		}
		if (ArrowRoom == PlayerRoom)
		{
			std::cout << "\n  OUCH! Arrow got you!\n" << std::endl;
			GameOver = 1;
			return;
		}
	}
};
