#pragma once
#include <array>

struct Square {
	int row;
	int col;
};

class GameBoard
{
public:
	GameBoard();
	char checkForInput(int col, int row);
	std::array<std::array<char, 8>, 8> m_boardArray;

private:
	char m_pickedUp = 0;
	Square m_pickedSquare;
	bool m_whiteMove = true;
	bool m_whitePickedUp = false;
	bool m_blackPickedUp = false;

	bool checkValidMove(int row, int col);
	bool pawnValidMove(int row, int col, bool white);
	bool knightValidMove(int row, int col, bool white);
	bool bishopValidMove(int row, int col, bool white);
	bool rookValidMove(int row, int col, bool white);
	bool queenValidMove(int row, int col, bool white);
	bool kingValidMove(int row, int col, bool white);
};

