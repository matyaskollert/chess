#include "GameBoard.h"
#include <array>
#include <iostream>

GameBoard::GameBoard() {
	m_boardArray = { {
		{'r','n','b','q','k','b','n','r'},
		{'p','p','p','p','p','p','p','p'},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{'P','P','P','P','P','P','P','P'},
		{'R','N','B','Q','K','B','N','R'}
	} };
}

char GameBoard::checkForInput(int col, int row)
{
	auto x = m_boardArray[row][col];
	if (m_pickedSquare.row == row && m_pickedSquare.col == col) {
		m_boardArray[row][col] = m_pickedUp;
		m_whitePickedUp = false;
		m_blackPickedUp = false;
		m_pickedUp = 0;
		m_pickedSquare = {};
	}
	if (m_whitePickedUp) {
		if ((std::islower(x) || x == 0) && checkValidMove(row, col)) {
			m_boardArray[row][col] = m_pickedUp;
			m_whiteMove = false;
			m_whitePickedUp = false;
		}
		else {
			std::cout << "Invalid move\n";
		}
	}
	else if (m_blackPickedUp) {
		if ((!std::islower(x) || x == 0) && checkValidMove(row, col)) {
			m_boardArray[row][col] = m_pickedUp;
			m_whiteMove = true;
			m_blackPickedUp = false;
		}
		else {
			std::cout << "Invalid move\n";
		}
	}
	else if (x != 0) {
		if (m_whiteMove && !std::islower(x)) {
			m_boardArray[row][col] = 0;
			m_whitePickedUp = true;
			m_pickedUp = x;
			m_pickedSquare.col = col;
			m_pickedSquare.row = row;
		}
		else if (!m_whiteMove && std::islower(x)) {
			m_boardArray[row][col] = 0;
			m_blackPickedUp = true;
			m_pickedUp = x;
			m_pickedSquare.col = col;
			m_pickedSquare.row = row;
		}
	}
	return x;
}

bool GameBoard::checkValidMove(int row, int col)
{
	switch (m_pickedUp)
	{
	case 'P':
		return pawnValidMove(row, col, true);
	case 'N':
		return knightValidMove(row, col, true);
	case 'B':
		return bishopValidMove(row, col, true);
	case 'R':
		return rookValidMove(row, col, true);
	case 'Q':
		return queenValidMove(row, col, true);
	case 'K':
		return kingValidMove(row, col, true);
	case 'p':
		return pawnValidMove(row, col, false);
	case 'n':
		return knightValidMove(row, col, false);
	case 'b':
		return bishopValidMove(row, col, false);
	case 'r':
		return rookValidMove(row, col, false);
	case 'q':
		return queenValidMove(row, col, false);
	case 'k':
		return kingValidMove(row, col, false);
	default:
		return false;
	}
}

bool GameBoard::pawnValidMove(int row, int col, bool white)
{
	if (!white) {
		if (m_pickedSquare.row == 1 && m_pickedSquare.col == col && row == (m_pickedSquare.row + 2) && m_boardArray[m_pickedSquare.row + 1][col] == 0) {
			return true;
		}
		else if (m_pickedSquare.col == col && row == (m_pickedSquare.row + 1) && m_boardArray[row][col] == 0) {
			return true;
		}
		else if (m_pickedSquare.col == (col + 1) && row == (m_pickedSquare.row + 1) && !std::islower(m_boardArray[row][col])) {
			return true;
		}
		else if (m_pickedSquare.col == (col - 1) && row == (m_pickedSquare.row + 1) && !std::islower(m_boardArray[row][col])) {
			return true;
		}
		return false;
	}
	else {
		if (m_pickedSquare.row == 6 && m_pickedSquare.col == col && row == (m_pickedSquare.row - 2) && m_boardArray[m_pickedSquare.row - 1][col] == 0) {
			return true;
		}
		else if (m_pickedSquare.col == col && row == (m_pickedSquare.row - 1) && m_boardArray[row][col] == 0) {
			return true;
		}
		else if (m_pickedSquare.col == (col + 1) && row == (m_pickedSquare.row - 1) && std::islower(m_boardArray[row][col])) {
			return true;
		}
		else if (m_pickedSquare.col == (col - 1) && row == (m_pickedSquare.row - 1) && std::islower(m_boardArray[row][col])) {
			return true;
		}
		return false;
	}
}

bool GameBoard::knightValidMove(int row, int col, bool white)
{
	return false;
}

bool GameBoard::bishopValidMove(int row, int col, bool white)
{
	int rowD = (m_pickedSquare.row - row) < 0 ? 1 : -1;
	int colD = (m_pickedSquare.col - col) < 0 ? 1 : -1;
	int inc = colD * (m_pickedSquare.col - col);
	if (inc == rowD * (m_pickedSquare.row - row)) {
		for (int i = 1; i < -inc; i++) {
			if (m_boardArray[m_pickedSquare.row + (rowD * i)][m_pickedSquare.col + (colD * i)] != 0) {
				return false;
			}
		}
	}
	else {
		return false;
	}

	if (!white) {
		if (!std::islower(m_boardArray[row][col]) || m_boardArray[row][col] == 0) {
			return true;
		}
	}
	else {
		if (std::islower(m_boardArray[row][col]) || m_boardArray[row][col] == 0) {
			return true;
		}
	}
}

bool GameBoard::rookValidMove(int row, int col, bool white)
{
	int D, inc;

	if (m_pickedSquare.row - row == 0) {
		D = (m_pickedSquare.col - col) < 0 ? 1 : -1;
		inc = D * (m_pickedSquare.col - col);
		for (int i = 1; i < -inc; i++) {
			if (m_boardArray[row][m_pickedSquare.col + (D * i)] != 0) {
				return false;
			}
		}
	}
	else if (m_pickedSquare.col - col == 0) {
		D = (m_pickedSquare.row - row) < 0 ? 1 : -1;
		inc = D * (m_pickedSquare.row - row);

		for (int i = 1; i < -inc; i++) {
			if (m_boardArray[m_pickedSquare.row + (D * i)][col] != 0) {
				return false;
			}
		}
	}
	else {
		return false;
	}

	if (!white) {
		if (!std::islower(m_boardArray[row][col]) || m_boardArray[row][col] == 0) {
			return true;
		}
	}
	else {
		if (std::islower(m_boardArray[row][col]) || m_boardArray[row][col] == 0) {
			return true;
		}
	}
	return false;
}

bool GameBoard::queenValidMove(int row, int col, bool white)
{
	return bishopValidMove(row, col, white) || rookValidMove(row, col, white);
}

bool GameBoard::kingValidMove(int row, int col, bool white)
{
	return false;
}

