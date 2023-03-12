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

	bool m_whitePromotion = false;
	bool m_blackPromotion = false;

	Square m_lastMove = { -1,-1 };

	int countPieces();

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
	char promotion(int row);
	void sameSquare(int row, int col);
	void resetValidMoves();
	bool checkValidMove(const int& row, const int& col);
	bool pawnValidMove(const int& row, const int& col, bool white);
	bool knightValidMove(const int& row, const int& col, bool white);
	bool bishopValidMove(const int& row, const int& col, bool white);
	bool rookValidMove(const int& row, const int& col, bool white);
	bool queenValidMove(const int& row, const int& col, bool white);
	bool kingValidMove(const int& row, const int& col, bool white);
	//check if the opposite king is in check
	int checkCheck(bool white);
};

