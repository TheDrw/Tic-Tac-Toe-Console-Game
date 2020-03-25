// TicTacToeGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h" // i don't really know what to do with this
#include "Game.h"

int main()
{
	auto game = std::make_unique<Game>();
	game->play();
	return 0;
}