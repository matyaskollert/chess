#include "GameBoard.h"
#include <array>
#include <iostream>

GameBoard::GameBoard() {
	m_boardArray = { {
		{'R','N','B','Q','K','B','N','R'},
		{'P','P','P','P','P','P','P','P'},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{'p','p','p','p','p','p','p','p'},
		{'r','n','b','q','k','b','n','r'}

	} };
	resetValidMoves();
	getValidMoves(true);
}

char GameBoard::checkForInput(int col, int row)
{
	auto targetPosition = m_boardArray[row][col];
	if (m_selectedSquare.row == row && m_selectedSquare.col == col) {
		m_boardArray[row][col] = m_selectedFigure;
		m_whitePickedUp = false;
		m_blackPickedUp = false;
		m_selectedFigure = 0;
		m_selectedSquare = { 0,0 };
	}
	//is the move valid?
	if (m_validMoveArray[m_selectedSquare.row][m_selectedSquare.col][row][col]) {
		//has white picked up a figure?
		if (m_whitePickedUp) {
			m_whiteChecked = false;
			m_boardArray[row][col] = m_selectedFigure;
			if (checkCheck(true) == 1) {
				m_blackChecked = true;
			}
			//if the king moved, update special variable
			if (m_selectedFigure == 'K') {
				m_whiteKingSquare = { row, col };
			}
			//if the king or rook moved, update castling possibilities
			if (m_selectedFigure == 'R' || m_selectedFigure == 'K') {
				m_whiteLongCastle = false;
				m_whiteShortCastle = false;
			}
			//is white castling short? move rook as well
			if (m_selectedFigure == 'K' && col == m_selectedSquare.col + 2) {
				m_boardArray[row][col - 1] = 'R';
				m_boardArray[row][col + 1] = 0;
			}
			//is white castling short? move rook as well
			if (m_selectedFigure == 'K' && col == m_selectedSquare.col - 2) {
				m_boardArray[row][col + 1] = 'R';
				m_boardArray[row][col - 2] = 0;
			}
			if (m_selectedFigure == 'P' && m_enPassantAccepted) {
				m_boardArray[row - 1][col] = 0;
				m_enPassantAccepted = false;
			}
			if (m_selectedFigure == 'P' && row == m_selectedSquare.row + 2) {
				m_enPassantSquare = { row - 1, col };
				m_enPassant = true;
			}
			else {
				m_enPassant = false;
			}
			m_whiteMove = false;
			m_whitePickedUp = false;
			resetValidMoves();
			getValidMoves(false);
		}
		//has black picked up a figure?
		else if (m_blackPickedUp) {
			m_blackChecked = false;
			m_boardArray[row][col] = m_selectedFigure;
			if (checkCheck(false) == -1) {
				m_whiteChecked = true;
			}
			if (m_selectedFigure == 'k') {
				m_blackKingSquare = { row, col };
			}
			if (m_selectedFigure == 'r' || m_selectedFigure == 'k') {
				m_blackLongCastle = false;
				m_blackShortCastle = false;
			}
			//is black castling short? move rook as well
			if (m_selectedFigure == 'k' && col == m_selectedSquare.col + 2) {
				m_boardArray[row][col - 1] = 'r';
				m_boardArray[row][col + 1] = 0;
			}
			//is black castling short? move rook as well
			if (m_selectedFigure == 'k' && col == m_selectedSquare.col - 2) {
				m_boardArray[row][col + 1] = 'r';
				m_boardArray[row][col - 2] = 0;
			}
			if (m_selectedFigure == 'p' && m_enPassantAccepted) {
				m_boardArray[row + 1][col] = 0;
				m_enPassantAccepted = false;
			}
			if (m_selectedFigure == 'p' && row == m_selectedSquare.row - 2) {
				m_enPassantSquare = { row + 1, col };

				m_enPassant = true;
			}
			else {
				m_enPassant = false;
			}
			m_whiteMove = true;
			m_blackPickedUp = false;
			resetValidMoves();
			getValidMoves(true);
		}
	}
	else if (targetPosition != 0 && !m_whitePickedUp && !m_blackPickedUp) {
		if (m_whiteMove && std::isupper(targetPosition)) {
			m_boardArray[row][col] = 0;
			m_whitePickedUp = true;
			m_selectedFigure = targetPosition;
			m_selectedSquare = { row, col };
		}
		else if (!m_whiteMove && std::islower(targetPosition)) {
			m_boardArray[row][col] = 0;
			m_blackPickedUp = true;
			m_selectedFigure = targetPosition;
			m_selectedSquare = { row, col };
		}
	}
	return targetPosition;
}

void GameBoard::getValidMoves(bool white)
{
	bool makesCheck = false;
	bool checkMate = true;
	//store king positions so that we can test if the king is in check after it's move
	const Square lastWhiteKing = m_whiteKingSquare, lastBlackKing = m_blackKingSquare;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			m_pickedUp = m_boardArray[x][y];
			m_pickedSquare = { x,y };
			if ((white && std::isupper(m_pickedUp)) || (!white && std::islower(m_pickedUp))) {
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						if (m_pickedUp == 'k') {
							m_blackKingSquare.row = i;
							m_blackKingSquare.col = j;
						}
						else if (m_pickedUp == 'K') {
							m_whiteKingSquare.row = i;
							m_whiteKingSquare.col = j;
						}
						const char lastPiece = m_boardArray[i][j];
						const char lastPiece2 = m_boardArray[x][y];
						m_boardArray[i][j] = m_pickedUp;
						m_boardArray[x][y] = 0;
						makesCheck = checkCheck(!white);
						m_boardArray[i][j] = lastPiece;
						m_boardArray[x][y] = lastPiece2;
						m_whiteKingSquare = lastWhiteKing;
						m_blackKingSquare = lastBlackKing;
						m_validMoveArray[x][y][i][j] = !makesCheck && checkValidMove(i, j);
						if (checkMate && m_validMoveArray[x][y][i][j]) {
							checkMate = false;
						}
					}
				}
			}
		}
	}
	if (checkMate) {
		std::cout << "CHECKMATE\n";
	}
}

void GameBoard::resetValidMoves()
{
	std::array<std::array<bool, 8>, 8> arr = { {
		{false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false}
	} };
	m_validMoveArray = { {
		{arr, arr, arr, arr, arr, arr, arr, arr},
		{arr, arr, arr, arr, arr, arr, arr, arr},
		{arr, arr, arr, arr, arr, arr, arr, arr},
		{arr, arr, arr, arr, arr, arr, arr, arr},
		{arr, arr, arr, arr, arr, arr, arr, arr},
		{arr, arr, arr, arr, arr, arr, arr, arr},
		{arr, arr, arr, arr, arr, arr, arr, arr},
		{arr, arr, arr, arr, arr, arr, arr, arr}
	} };
}

bool GameBoard::checkValidMove(int targetRow, int targetCol)
{

	if (m_pickedSquare.col == targetCol && m_pickedSquare.row == targetRow) {
		return false;
	}
	if (std::islower(m_pickedUp) && std::islower(m_boardArray[targetRow][targetCol])) {
		return false;
	}
	if (std::isupper(m_pickedUp) && std::isupper(m_boardArray[targetRow][targetCol])) {
		return false;
	}

	switch (m_pickedUp)
	{
	case 'P':
		return pawnValidMove(targetRow, targetCol, true);
	case 'N':
		return knightValidMove(targetRow, targetCol, true);
	case 'B':
		return bishopValidMove(targetRow, targetCol, true);
	case 'R':
		return rookValidMove(targetRow, targetCol, true);
	case 'Q':
		return queenValidMove(targetRow, targetCol, true);
	case 'K':
		return kingValidMove(targetRow, targetCol, true);
	case 'p':
		return pawnValidMove(targetRow, targetCol, false);
	case 'n':
		return knightValidMove(targetRow, targetCol, false);
	case 'b':
		return bishopValidMove(targetRow, targetCol, false);
	case 'r':
		return rookValidMove(targetRow, targetCol, false);
	case 'q':
		return queenValidMove(targetRow, targetCol, false);
	case 'k':
		return kingValidMove(targetRow, targetCol, false);
	default:
		return false;
	}
}

bool GameBoard::pawnValidMove(int row, int col, bool white)
{
	if (!white) {
		if (m_pickedSquare.row == 6 && m_pickedSquare.col == col && row == (m_pickedSquare.row - 2) && m_boardArray[m_pickedSquare.row - 1][col] == 0 && m_boardArray[row][col] == 0) {
			return true;
		}
		else if (m_pickedSquare.col == col && row == (m_pickedSquare.row - 1) && m_boardArray[row][col] == 0) {
			return true;
		}
		else if ((m_pickedSquare.col == (col + 1) || m_pickedSquare.col == (col - 1)) && row == (m_pickedSquare.row - 1) && std::isupper(m_boardArray[row][col])) {
			return true;
		}
		else if (m_enPassant && m_enPassantSquare.row == row && m_enPassantSquare.col == col && m_pickedSquare.row == row + 1 && m_enPassantSquare.col == col && std::isupper(m_boardArray[row + 1][col])) {
			m_enPassantAccepted = true;
			return true;
		}
		return false;
	}
	else {
		if (m_pickedSquare.row == 1 && m_pickedSquare.col == col && row == (m_pickedSquare.row + 2) && m_boardArray[m_pickedSquare.row + 1][col] == 0 && m_boardArray[row][col] == 0) {
			return true;
		}
		else if (m_pickedSquare.col == col && row == (m_pickedSquare.row + 1) && m_boardArray[row][col] == 0) {
			return true;
		}
		else if ((m_pickedSquare.col == (col + 1) || m_pickedSquare.col == (col - 1)) && row == (m_pickedSquare.row + 1) && std::islower(m_boardArray[row][col])) {
			return true;
		}
		else if (m_enPassant && m_enPassantSquare.row == row && m_enPassantSquare.col == col && m_pickedSquare.row == row - 1 && m_enPassantSquare.col == col && std::islower(m_boardArray[row - 1][col])) {
			m_enPassantAccepted = true;
			return true;
		}
		return false;
	}
}

bool GameBoard::knightValidMove(int row, int col, bool white)
{

	if (row == m_pickedSquare.row + 2 || row == m_pickedSquare.row - 2) {
		if (!(col == m_pickedSquare.col + 1 || col == m_pickedSquare.col - 1)) {
			return false;
		}
	}
	else if (col == m_pickedSquare.col + 2 || col == m_pickedSquare.col - 2) {
		if (!(row == m_pickedSquare.row + 1 || row == m_pickedSquare.row - 1)) {
			return false;
		}
	}
	else {
		return false;
	}
	return true;
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
	return true;
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
	return true;
}

bool GameBoard::queenValidMove(int row, int col, bool white)
{
	return bishopValidMove(row, col, white) || rookValidMove(row, col, white);
}

bool GameBoard::kingValidMove(int row, int col, bool white)
{
	if (m_pickedSquare.row == row)
	{
		const bool condShort = white ? m_whiteShortCastle : m_blackShortCastle;
		const bool condLong = white ? m_whiteLongCastle : m_blackLongCastle;
		if (condShort && m_pickedSquare.col + 2 == col && m_boardArray[row][col - 1] == 0 && m_boardArray[row][col] == 0) {
			const Square lastKing = m_whiteKingSquare;
			m_whiteKingSquare = { row, col - 1 };
			const bool check1 = checkCheck(!white);
			m_whiteKingSquare = { row, col };
			const bool check2 = checkCheck(!white);
			m_whiteKingSquare = lastKing;
			return !(check1 || check2);
		}
		else if (condLong && m_pickedSquare.col - 2 == col && m_boardArray[row][col + 1] == 0 && m_boardArray[row][col - 1] == 0 && m_boardArray[row][col] == 0) {
			const Square lastKing = m_whiteKingSquare;
			m_whiteKingSquare = { row, col + 1 };
			const bool check1 = checkCheck(!white);
			m_whiteKingSquare = { row, col };
			const bool check2 = checkCheck(!white);
			m_whiteKingSquare = lastKing;
			return !(check1 || check2);
		}
	}
	if (std::abs(row - m_pickedSquare.row) <= 1 && std::abs(col - m_pickedSquare.col) <= 1) {
		return true;
	}
	return false;
}

int GameBoard::checkCheck(bool white)
{
	int ret = 0;
	const auto lastPiece = m_pickedUp;
	const auto lastSquare = m_pickedSquare;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (m_boardArray[i][j] != 0) {
				m_pickedUp = m_boardArray[i][j];
				m_pickedSquare = { i, j };
				if (std::isupper(m_pickedUp) && white) {
					if (checkValidMove(m_blackKingSquare.row, m_blackKingSquare.col)) {
						ret = 1;
					}
				}
				else if (std::islower(m_pickedUp) && !white) {
					if (checkValidMove(m_whiteKingSquare.row, m_whiteKingSquare.col)) {
						ret = -1;
					}
				}
			}
		}
	}
	m_pickedUp = lastPiece;
	m_pickedSquare = lastSquare;
	return ret;
}
