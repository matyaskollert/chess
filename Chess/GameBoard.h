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
	//handle user input
	char checkForInput(int row, int col);
	//stores the current game position
	std::array<std::array<char, 8>, 8> m_boardArray;
	//stores all the valid moves for the currently picked up figure
	std::array<std::array<std::array<std::array<bool, 8>, 8>, 8>, 8> m_validMoveArray;
	//currently selected up figure
	char m_selectedFigure = 0;
	//the square at which the currently selected figure is
	Square m_selectedSquare = { 0,0 };

	//is it white's move
	bool m_whiteMove = true;

	int countFigures();

private:
	//currently picked up figure in valid checking
	char m_pickedUp = 0;
	//the square at which the currently picked up figure is in valid checking
	Square m_pickedSquare;
	//is white in check
	bool m_whiteChecked = false;
	//is black in check
	bool m_blackChecked = false;
	//the position of the white king
	Square m_whiteKingSquare{ 0, 4 };
	//the position of the black king
	Square m_blackKingSquare{ 7, 4 };
	//can white short castle
	bool m_whiteShortCastle = true;
	//can white long castle
	bool m_whiteLongCastle = true;
	//can black short castle
	bool m_blackShortCastle = true;
	//can black long castle
	bool m_blackLongCastle = true;

	//has white picked up a figure
	bool m_whitePickedUp = false;
	//has black picked up a figure
	bool m_blackPickedUp = false;

	bool m_enPassant = false;
	Square m_enPassantSquare = { 0,0 };
	bool m_enPassantAccepted = false;

	void handleWhiteMove(int& row, int& col);
	void handleBlackMove(int& row, int& col);
	void getValidMoves(bool white);
	void resetValidMoves();
	bool checkValidMove(int row, int col);
	bool pawnValidMove(int row, int col, bool white);
	bool knightValidMove(int row, int col, bool white);
	bool bishopValidMove(int row, int col, bool white);
	bool rookValidMove(int row, int col, bool white);
	bool queenValidMove(int row, int col, bool white);
	bool kingValidMove(int row, int col, bool white);
	//check if the opposite king is in check
	int checkCheck(bool white);
};

