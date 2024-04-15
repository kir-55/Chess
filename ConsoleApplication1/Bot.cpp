#include "Bot.h"
#include "ChessMethods.h"
#include <limits>
#include <iostream>


Bot::Bot(std::string name, int depth) {
	m_name = name;
    this->depth = depth;
}

float Bot::Evaluate(std::array<std::array<int, 8>, 8> board, std::string movesNotation, bool color) {
    std::array<std::array<int, 8>, 8> whiteAttackMap = getAttackMap(board, false, movesNotation);
    std::array<std::array<int, 8>, 8> blackAttackMap = getAttackMap(board, true, movesNotation);

    int whitePiecesValue = 0, blackPiecesValue = 0;
    int whiteAttackPower = 0, blackAttackPower = 0;
    float value = 0;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int currentPieceValue = board[y][x];
            if (currentPieceValue > 0)
                whitePiecesValue += currentPieceValue;
            else if (currentPieceValue < 0)
                blackPiecesValue += abs(currentPieceValue);

            whiteAttackPower += whiteAttackMap[y][x];
            blackAttackPower += blackAttackMap[y][x];
        }
    }
    std::cout << "piecevalues: " << (whitePiecesValue + blackPiecesValue) / 2 << std::endl;

    //position whiteKingPos = getKingPos(true, board);
    //position blackKingPos = getKingPos(false, board);

    //std::vector<possibleMove> whiteKingPossibleMoves = KING.GetPossibleMoves(board, blackAttackMap, movesNotation, whiteKingPos);
    //std::vector<possibleMove> whiteKingSafePossibleMoves = getSafeMoves(board, KING.GetPossibleMoves(board, blackAttackMap, movesNotation, whiteKingPos), whiteKingPos, true, movesNotation);

    //std::vector<possibleMove> blackKingPossibleMoves = KING.GetPossibleMoves(board, whiteAttackMap, movesNotation, blackKingPos);
    //std::vector<possibleMove> blackKingSafePossibleMoves = getSafeMoves(board, KING.GetPossibleMoves(board, whiteAttackMap, movesNotation, blackKingPos), blackKingPos, false, movesNotation);

    //if ((whitePiecesValue + blackPiecesValue) / 2 < 20)//lategame or latemiddlegame
    //{
    //    value += (whiteKingSafePossibleMoves.size() - whiteKingPossibleMoves.size()) * 0.25;
    //    value += whiteKingSafePossibleMoves.size() * 0.1;
    //   
    //    value -= (blackKingSafePossibleMoves.size() - blackKingPossibleMoves.size()) * 0.25;
    //    value -= blackKingSafePossibleMoves.size() * 0.1;
    //}
    //else {
    //    value += (whiteKingSafePossibleMoves.size() - whiteKingPossibleMoves.size()) * 0.25;
    //    value += (8 - whiteKingSafePossibleMoves.size()) * 0.1;

    //    value -= (blackKingSafePossibleMoves.size() - blackKingPossibleMoves.size()) * 0.25;
    //    value -= (8 - blackKingSafePossibleMoves.size()) * 0.1;
    //}

    value += whitePiecesValue - blackPiecesValue;
    value += (whiteAttackPower - blackAttackPower) * 0.1;
    //std::cout << "evalue is: " << value << std::endl;
    return value;
}

globalMove Bot::FindMove(std::array<std::array<int, 8>, 8> board, std::string movesNotation, bool color) {
    float bestScore = color? std::numeric_limits<float>::lowest() : std::numeric_limits<float>::max();
    globalMove bestMove;

    std::array<std::array<int, 8>, 8> attackMap = getAttackMap(board, color, movesNotation);
    std::vector<globalMove> moves = getAllMoves(board, attackMap, color, movesNotation);

    for (const globalMove& move : moves) {
        std::array<std::array<int, 8>, 8> virtualBoard = board;
        movePiece(move, virtualBoard);
        std::string newMovesNotation = movesNotation + " | " + moveToString(move);

        float eval = minimax(depth - 1, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), !color, virtualBoard, newMovesNotation);

        if ((color and eval > bestScore) or (!color and eval < bestScore)) {
            bestScore = eval;
            bestMove = move;
        }
    }

    return bestMove;
}

float Bot::minimax(int depth, float alpha, float beta, bool maximizingPlayer, std::array<std::array<int, 8>, 8> board,std::string movesNotation) {
    if (depth == 0) {
        return this->Evaluate(board, movesNotation, maximizingPlayer);
    }

    std::array<std::array<int, 8>, 8> attackMap = getAttackMap(board, maximizingPlayer, movesNotation);

    if (maximizingPlayer) {
        float maxEval = std::numeric_limits<float>::lowest();
        std::vector<globalMove> moves = getAllMoves(board, attackMap, maximizingPlayer, movesNotation);
        for (const globalMove& move : moves) {
            std::array<std::array<int, 8>, 8> virtualBoard = board;
            movePiece(move, virtualBoard);
            std::string newMovesNotation = movesNotation + " | " + moveToString(move);
            float eval = minimax(depth - 1, alpha, beta, false, virtualBoard, newMovesNotation);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    }
    else {
        float minEval = std::numeric_limits<float>::max();
        std::vector<globalMove> moves = getAllMoves(board, attackMap, maximizingPlayer, movesNotation);
        for (const globalMove& move : moves) {
            std::array<std::array<int, 8>, 8> virtualBoard = board;
            movePiece(move, virtualBoard);
            std::string newMovesNotation = movesNotation + " | " + moveToString(move);
            float eval = minimax(depth - 1, alpha, beta, false, virtualBoard, newMovesNotation);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}