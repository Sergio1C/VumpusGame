#pragma once
#include <vector>
#include <sstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <random>

const int MAX_ROUTE = 3; //количество туннелей между комнатами 
int MAX_AMMO = 5;		 //количество стрел у игрока в начале игры

/*
 ласс Room. Room - место, в которм находитс€ персонаж. 
 ажда€ комната соединена трем€ пут€ми с соседними.
Ћокаци€ состоит из 20 комнат, соединенных друг с другом.
*/
class Room
{
public:
	int number;
	Room* route[MAX_ROUTE];
	bool hole = false;

	Room(int _number, bool _hole = 0) : number(_number), hole(_hole) 
	{
		for (int i = 0; i < MAX_ROUTE; i++)
			route[i] = nullptr;
	};
	bool AllRoute() const
	{
		for (int i = 0; i < MAX_ROUTE; i++)
		if (!route[i]) return 0;
		return 1;
	}
	friend std::ostream& operator<<(std::ostream& os, const Room* ptrRoom)
	{
		os << ptrRoom->number;
		return os;
	}
	friend Room* RandomRoute(const Room* Room)
	{
		srand(time(0));
		int random_num = rand() % 2;
		return Room->route[random_num];
	}

};

class Person 
{
public:
	Person();
	//virtual ~Person();
	Room* Position = nullptr;
	virtual void Move(Room* To) { Position = To;}
};

Person::Person()
{}

class Vampus : public Person
{
public:
	Vampus() : Person() {};
	virtual void Move() { Position = RandomRoute(Position); }
};

class Player : public Person
{
public:
	Player() : Person() {};
	void Shoot();
protected:
	int ammo = MAX_AMMO;
};

/*¬ыстрел игрока - 
1.запрос номеров комнат, которое должна пролететь стрела
2.стрела не может возвратитьс€ к игроку
3. если указана комната при (1), не св€занна€ туннелем с предыдущей, то стрела улетает в случайную комнату
*/

void Player::Shoot()
{
	while (1)
	{
		std::cout <<
	}
};

bool CreateRoute(Room* One, Room* Two);
bool IsRoute(Room* One, Room* Two);

class VampusGame
{
public:
	VampusGame();
	~VampusGame();
	Vampus Vampus;
	Player Player;

	void Action();

	void PrintRooms() const;
private:
	void CreateMap();

protected:
	std::vector<Room*> Rooms;
	
	bool Check();
	Room* RandomRoom(int first = 1, int last = 20)
	{
		/*generate random room number
		http://stackoverflow.com/questions/15461140/stddefault-random-engine-generato-values-betwen-0-0-and-1-0
		(hardware method)
		*/	
		std::uniform_int_distribution<int> distribution(first, last - 1);
		std::random_device rd;
		std::default_random_engine generator(rd());
		int random_num = distribution(generator);

		return Rooms.at(random_num);
	}

};

VampusGame::VampusGame()
{
	CreateMap();
	Vampus.Position = RandomRoom();
	Player.Position = RandomRoom();
}

VampusGame::~VampusGame()
{
	std::vector<Room*>::iterator it;
	for (it = Rooms.begin(); it != Rooms.end(); ++it)
	{
		delete *it;
	}
}

void VampusGame::Action()
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
				Vampus.Move();
				break;
			}
		}
		case 's':
		{
			Player.Shoot();
			if (!Check()) break;
			Vampus.Move();

		}
		default: 
		{
			std::cout << "Wrong input, try again:";
			continue;
		}
		}
	}

}

void VampusGame::PrintRooms() const
{
	std::ostringstream ss;
	std::vector<Room*>::const_iterator it;
	for (it = Rooms.begin(); it != Rooms.end(); ++it)
	{
		ss << "("<<(*it)->number<<")->";
		for (int i = 0; i < 3; i++)
		{
			Room* route = (*it)->route[i];
			if (route) ss <<route->number<<" ";
		}
	}
	
	ss << std::endl;
	//ј вот так очищаем поток ss
	std::cout << ss.str();
	ss.str("");
	ss.flush();

}

void VampusGame::CreateMap()
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
		for (std::vector<Room*>::iterator it2 = it+1; it2 < Rooms.end(); ++it2)
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

bool VampusGame::Check()
{
	if (Player.Position->hole) //мы попали в комнату с €мой
	{
		std::cout << "Oops...you are in hole.Game over!" << std::endl;
		return 0;
	}
	if (Player.Position->route[0]->hole || Player.Position->route[1]->hole || Player.Position->route[2]->hole)
	{ 
		std::cout << "I feel a breeze..." << std::endl;
	}
	if (Player.Position == Vampus.Position) //нас съедает Vampus
	{
		std::cout << "Oops...You ate the Vampus.Game Over!" << std::endl;
		return 0;
	}
	if (Player.Position->route[0] == Vampus.Position || Player.Position->route[1] == Vampus.Position
		|| Player.Position->route[2] == Vampus.Position)
	{ 
		std::cout << "I feel a Vampus!" << std::endl;
	}

	return 1;
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

bool IsRoute(Room* One, Room* Two)
{
	for (int i = 0; i < 3; i++)
	{
		if ((One->route[i] == Two) || (Two->route[i] == One)) return 1;
	}
	return 0;
}