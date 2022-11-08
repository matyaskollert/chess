#include "GameEngine.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

int DEPTH = 2;

GameEngine::GameEngine()
{
	m_evaluation = 0;
}

float GameEngine::getEvaluation(GameBoard& board)
{
	auto start = std::chrono::high_resolution_clock::now();
	auto piece = negaMaxRoot(DEPTH, board, board.m_whiteMove);
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
	std::cout << piece.x.x << "," << piece.x.y << " - " << piece.y.x << "," << piece.y.y << std::endl;
	return m_evaluation / 100.f;
}

void GameEngine::getBestMovePromise(GameBoard& board) {
	if (board.countPieces() < 10) {
		DEPTH = 3;
	}
	std::thread thread([](GameBoard board, GameEngine* e) {
		std::cout << "new thread \n";
		auto start = std::chrono::high_resolution_clock::now();
		auto piece = e->alphaBetaRoot(DEPTH, board, board.m_whiteMove);
		auto stop = std::chrono::high_resolution_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
		std::cout << piece.x.x << "," << piece.x.y << " - " << piece.y.x << "," << piece.y.y << std::endl;
		e->m_bestMove = piece;
		e->ready = true;
		}, board, this);

	thread.detach();
}

PairPair GameEngine::getBestMove() {
	ready = false;
	return m_bestMove;
}

int GameEngine::calculateEvaluation(const GameBoard& board) {
	m_evaluation = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			const char& piece = board.m_boardArray[i][j];
			m_evaluation += convertPieceToValue(piece);
			switch (piece) {
			case 'P':
				m_evaluation += pawnWeights[i][j];
				break;
			case 'N':
				m_evaluation += knightWeights[i][j];
				break;
			case 'B':
				m_evaluation += bishopWeights[i][j];
				break;
			case 'R':
				m_evaluation += rookWeights[i][j];
				break;
			case 'Q':
				m_evaluation += queenWeights[i][j];
				break;
			case 'K':
				m_evaluation += kingWeights[i][j];
				break;
			case 'p':
				m_evaluation -= pawnWeights[7 - i][j];
				break;
			case 'n':
				m_evaluation -= knightWeights[7 - i][j];
				break;
			case 'b':
				m_evaluation -= bishopWeights[7 - i][j];
				break;
			case 'r':
				m_evaluation -= rookWeights[7 - i][j];
				break;
			case 'q':
				m_evaluation -= queenWeights[7 - i][j];
				break;
			case 'k':
				m_evaluation -= kingWeights[7 - i][j];
				break;
			default:
				m_evaluation += 0;
			}
		}
	}
	return m_evaluation;
}

int  GameEngine::convertPieceToValue(const char& piece) {
	switch (piece) {
	case 'P':
		return 100;
	case 'N':
		return 320;
	case 'B':
		return 330;
	case 'R':
		return 500;
	case 'Q':
		return 900;
	case 'K':
		return 20000;
	case 'p':
		return -100;
	case 'n':
		return -320;
	case 'b':
		return -330;
	case 'r':
		return -500;
	case 'q':
		return -900;
	case 'k':
		return -20000;
	default:
		return 0;
	}
}

PairPair GameEngine::negaMaxRoot(int depth, GameBoard& board, bool white)
{
	int max = -999999999;
	PairPair pieceMax{};
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (board.m_boardArray[row][col] != 0) {
				for (int moveRow = 0; moveRow < 8; moveRow++) {
					for (int moveCol = 0; moveCol < 8; moveCol++) {
						if (board.m_validMoveArray[row][col][moveRow][moveCol]) {
							GameBoard b = { board };
							b.checkForInput(row, col);
							b.checkForInput(moveRow, moveCol);
							if (b.m_blackPromotion || b.m_whitePromotion) {
								b.checkForInput(0, 0);
							}
							int score = -negaMax(depth - 1, b, b.m_whiteMove);
							if (score > max) {
								max = score;
								pieceMax.x.x = row;
								pieceMax.x.y = col;
								pieceMax.y.x = moveRow;
								pieceMax.y.y = moveCol;
							}
						}
					}
				}
			}
		}
	}
	return pieceMax;
}

int GameEngine::negaMax(int depth, GameBoard& board, bool white) {
	int i = white ? 1 : -1;
	if (depth == 0) {
		return i * calculateEvaluation(board);
	}
	int max = -999999999;
	PairPair pieceMax{};
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (board.m_boardArray[row][col] != 0) {
				for (int moveRow = 0; moveRow < 8; moveRow++) {
					for (int moveCol = 0; moveCol < 8; moveCol++) {
						if (board.m_validMoveArray[row][col][moveRow][moveCol]) {
							GameBoard b = { board };
							b.checkForInput(row, col);
							b.checkForInput(moveRow, moveCol);
							if (b.m_blackPromotion || b.m_whitePromotion) {
								b.checkForInput(0, 0);
							}
							int score = -negaMax(depth - 1, b, b.m_whiteMove);
							if (score > max) {
								max = score;
							}
						}
					}
				}
			}
		}
	}
	return max;
}

PairPair GameEngine::alphaBetaRoot(int depth, GameBoard& board, bool white) {
	int alpha = -999999999, beta = 999999999, max = -999999999;
	PairPair ret{};
	int row, col, moveRow, moveCol;

	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			if (board.m_boardArray[row][col] != 0) {
				for (moveRow = 0; moveRow < 8; moveRow++) {
					for (moveCol = 0; moveCol < 8; moveCol++) {
						if (board.m_validMoveArray[row][col][moveRow][moveCol]) {
							GameBoard b = { board };
							b.checkForInput(row, col);
							b.checkForInput(moveRow, moveCol);
							if (b.m_blackPromotion || b.m_whitePromotion) {
								b.checkForInput(0, 0);
							}
							int score = -alphaBeta(depth - 1, b, b.m_whiteMove, -beta, -alpha);
							if (score >= beta) {
								return { {row, col}, {moveRow, moveCol} };
							}
							if (score > alpha) {
								alpha = score;
								ret.x.x = row;
								ret.x.y = col;
								ret.y.x = moveRow;
								ret.y.y = moveCol;
							}
						}
					}
				}
			}
		}
	}
	return ret;
}

int GameEngine::alphaBeta(int depth, GameBoard& board, bool white, int alpha, int beta) {
	if (depth == 0) {
		return quiesce(board, white, alpha, beta);
	}
	int max = -999999999, row, col, moveRow, moveCol;
	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			if (board.m_boardArray[row][col] != 0) {
				for (moveRow = 0; moveRow < 8; moveRow++) {
					for (moveCol = 0; moveCol < 8; moveCol++) {
						if (board.m_validMoveArray[row][col][moveRow][moveCol]) {
							GameBoard b = { board };
							b.checkForInput(row, col);
							b.checkForInput(moveRow, moveCol);
							if (b.m_blackPromotion || b.m_whitePromotion) {
								b.checkForInput(0, 0);
							}
							int score = -alphaBeta(depth - 1, b, b.m_whiteMove, -beta, -alpha);
							if (score >= beta) {
								return beta;
							}
							if (score > alpha) {
								alpha = score;
							}
						}
					}
				}
			}
		}
	}
	return alpha;
}

int GameEngine::quiesce(GameBoard& board, bool white, int alpha, int beta) {
	const int i = white ? 1 : -1;
	const int stand_pat = i * calculateEvaluation(board);

	if (stand_pat >= beta) {
		return beta;
	}
	if (alpha < stand_pat) {
		alpha = stand_pat;
	}

	int row, col, moveRow, moveCol;

	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			if ((white && std::isupper(board.m_boardArray[row][col])) || (!white && std::islower(board.m_boardArray[row][col]))) {
				for (moveRow = 0; moveRow < 8; moveRow++) {
					for (moveCol = 0; moveCol < 8; moveCol++) {
						if (board.m_validMoveArray[row][col][moveRow][moveCol] && ((white && std::islower(board.m_boardArray[moveRow][moveCol])) || (!white && std::isupper(board.m_boardArray[moveRow][moveCol])))) {
							GameBoard b = { board };
							b.checkForInput(row, col);
							b.checkForInput(moveRow, moveCol);
							if (b.m_blackPromotion || b.m_whitePromotion) {
								b.checkForInput(0, 0);
							}
							int score = -quiesce(b, b.m_whiteMove, -beta, -alpha);
							if (score >= beta) {
								return beta;
							}
							if (score > alpha) {
								alpha = score;
							}
						}
					}
				}
			}
		}
	}
	return alpha;
}