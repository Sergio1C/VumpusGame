#pragma once
#include <vector>
#include <sstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <random>

const int MAX_ROUTE = 3; //количество туннелей между комнатами 
int MAX_AMMO = 5;		 //количество стрел у игрока в начале игры
const int MAX_ROOM_FOR_AMMO = 5; // количество комнат, которое пролетает стрела при выстреле

/*
Room - место, в котором находится персонаж. 
Каждая комната соединена тремя путями с соседними.
Локация состоит из 20 комнат, соединенных друг с другом.
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

/*
Person - базовый класс для персонажей игры.
Vumpus,Player - производные. Смотри соотв. классы.
*/
class Person 
{
public:
	Person();
	Room* Position = nullptr;
	virtual void Move(Room* To) { Position = To;}
};

Person::Person(){}

class Vumpus : public Person
{
public:
	Vumpus() : Person() {};
	virtual void Move() { Position = RandomRoute(Position); }
};

class Player : public Person
{
public:
	Player() : Person() {};
	int getAmmo() const { return ammo; }
	void IncreaseAmmo() { ammo--; };
private:
	int ammo = MAX_AMMO;
};

bool CreateRoute(Room* One, Room* Two);
bool IsRoute(const Room* One, const Room* Two);

class VumpusGame
{
public:
	VumpusGame();
	~VumpusGame();
	Vumpus Vumpus;
	Player Player;
	void Action();
	void PrintRooms() const;
	void Shoot();
private:
	void CreateMap();
	bool GameOver = 0;

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

VumpusGame::VumpusGame()
{
	CreateMap();
	Vumpus.Position = RandomRoom();
	Player.Position = RandomRoom();
}

VumpusGame::~VumpusGame()
{
	std::vector<Room*>::iterator it;
	for (it = Rooms.begin(); it != Rooms.end(); ++it)
	{
		delete *it;
	}
}









