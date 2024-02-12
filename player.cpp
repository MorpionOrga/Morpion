#include "player.h"

player::player() 
{
	playerName = "";
	value = ' ';
	players = INVALID_SOCKET;
}

player::~player() {

}

void player::addPlayer(std::string name) {
	for (int i = 0; i < 10; i++)
	{
		if (pList[i]->playerName != "")
		{
			if (i == 0)
			{
				pList[i]->playerName = name;
				pList[i]->value = 'X';
			}
			else if (i == 1)
			{
				pList[i]->playerName = name;
				pList[i]->value = 'O';
			}
			else {
				pList[i]->playerName = name;
				pList[i]->value = ' ';
			}
		}
	}
}

void player::handlePlayer(std::string name)
{
	if (pEmpty(name))
	{
		addPlayer(name);
	}	
}

BOOL player::pEmpty(std::string name) {
	for (int i = 0; i < 10; i++)
	{
		if (name == pList[i]->playerName)
		{
			return false;
		}
	}
	return true;
}

int player::getIndexFromName(std::string name)
{
	for (int i = 0; i < 10; i++)
	{
		if (name == pList[i]->playerName)
		{
			return i;
		}
	}
	return NULL;
}

char player::getValue()
{
	return value;
}