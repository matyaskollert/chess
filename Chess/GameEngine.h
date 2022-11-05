#pragma once
#include <array>
#include <vector>
#include "GameBoard.h"
#include "Common.h"

class GameEngine
{
public:
	GameEngine();
	float getEvaluation(GameBoard& board);
	void getBestMovePromise(GameBoard& board);
	PairPair getBestMove();
	bool ready = false;
private:
	int m_evaluation;
	PairPair m_bestMove = {};

	std::array<std::array<int, 8>, 8> pawnWeights = { {
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 5, 10, 10,-20,-20, 10, 10,  5,},
		{ 5, -5,-10,  0,  0,-10, -5,  5,},
		{ 0,  0,  0, 20, 20,  0,  0,  0,},
		{ 5,  5, 10, 25, 25, 10,  5,  5,},
		{10, 10, 20, 30, 30, 20, 10, 10,},
		{50, 50, 50, 50, 50, 50, 50, 50,},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
	} };

	std::array<std::array<int, 8>, 8> knightWeights = { {
		{-50,-40,-30,-30,-30,-30,-40,-50,},
		{-40,-20,  0,  5,  5,  0,-20,-40,},
		{-30,  5, 10, 15, 15, 10,  5,-30,},
		{-30,  0, 15, 20, 20, 15,  0,-30,},
		{-30,  5, 15, 20, 20, 15,  5,-30,},
		{-30,  0, 10, 15, 15, 10,  0,-30,},
		{-40,-20,  0,  0,  0,  0,-20,-40,},
		{-50,-40,-30,-30,-30,-30,-40,-50,},
	} };

	std::array<std::array<int, 8>, 8> bishopWeights = { {
		{-20,-10,-10,-10,-10,-10,-10,-20,},
		{-10, 15,  0,  0,  0,  0, 15,-10,},
		{-10, 10, 10, 10, 10, 10, 10,-10,},
		{-10,  0, 10, 10, 10, 10,  0,-10,},
		{-10,  5,  5, 10, 10,  5,  5,-10,},
		{-10,  0,  5, 10, 10,  5,  0,-10,},
		{-10,  0,  0,  0,  0,  0,  0,-10,},
		{-20,-10,-10,-10,-10,-10,-10,-20,},
	} };

	std::array<std::array<int, 8>, 8> rookWeights = { {
		{ 0,  0,  5,  5,  5,  5,  0,  0},
		{-5,  0,  0,  0,  0,  0,  0, -5,},
		{-5,  0,  0,  0,  0,  0,  0, -5,},
		{-5,  0,  0,  0,  0,  0,  0, -5,},
		{-5,  0,  0,  0,  0,  0,  0, -5,},
		{ 0,  0,  0,  0,  0,  0,  0,  0,},
		{ 5, 10, 10, 10, 10, 10, 10,  5,},
		{ 0,  0,  5,  5,  5,  5,  0,  0,},
	} };

	std::array<std::array<int, 8>, 8> queenWeights = { {
		{-20,-10,-10, -5, -5,-10,-10,-20},
		{-10,  0,  5,  0,  0,  0,  0,-10},
		{-10,  5,  5,  5,  5,  5,  0,-10},
		{  0,  0,  5,  5,  5,  5,  0, -5},
		{ -5,  0,  5,  5,  5,  5,  0, -5},
		{-10,  0,  5,  5,  5,  5,  0,-10},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-20,-10,-10, -5, -5,-10,-10,-20},
	} };

	std::array<std::array<int, 8>, 8> kingWeights = { {
		{ 20, 30, 10,  0,  0, 10, 30, 20},
		{ 20, 20,  0,  0,  0,  0, 20, 20},
		{-10,-20,-20,-20,-20,-20,-20,-10},
		{-20,-30,-30,-40,-40,-30,-30,-20},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
	} };

	std::array<std::array<int, 8>, 8> kingWeightsEnd = { {
		{-50,-30,-30,-30,-30,-30,-30,-50},
		{-30,-30,  0,  0,  0,  0,-30,-30},
		{-30,-10, 20, 30, 30, 20,-10,-30},
		{-30,-10, 30, 40, 40, 30,-10,-30},
		{-30,-10, 30, 40, 40, 30,-10,-30},
		{-30,-10, 20, 30, 30, 20,-10,-30},
		{-30,-20,-10,  0,  0,-10,-20,-30},
		{-50,-40,-30,-20,-20,-30,-40,-50},
	} };


	PairPair negaMaxRoot(int depth, GameBoard& board, bool white);
	int negaMax(int depth, GameBoard& board, bool white);

	PairPair alphaBetaRoot(int depth, GameBoard& board, bool white);
	int alphaBeta(int depth, GameBoard& board, bool white, int alpha, int beta);

	int quiesce(GameBoard& board, bool white, int alpha, int beta);

	void calculateEvaluation(std::array<std::array<char, 8>, 8> board);

	int convertPieceToValue(char piece);
};

