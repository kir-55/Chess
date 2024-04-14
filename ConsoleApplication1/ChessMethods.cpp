#include "ChessMethods.h"

position getKingPos(bool color, std::array<std::array<int, 8>, 8> board) {
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (board[y][x] == (color ? KING_ID : -KING_ID))
                return position{ x, y };
    return position{ -1, -1 };
}

bool positionExist(position pos) {
    if (pos.x > -1 and pos.x < boardSizeX and pos.y > -1 and pos.y < boardSizeY)
        return true;
    return false;
}

bool checkForCheck(position kingPos, std::array<std::array<int, 8>, 8> attackMap) {
    if (positionExist(kingPos) and attackMap[kingPos.y][kingPos.x] > 0) {
        return true; 
    }
    return false;
}

int getPiece(position pos, std::array<std::array<int, 8>, 8> board) {
    if (positionExist(pos))
        return board[pos.y][pos.x];
    return 0;
}

Piece& getPiece(int piece) {
    switch (piece) {
    case 5:
        return ROOK;
    case 3:
        return KNIGHT;
    case 4:
        return BISHOP;
    case 9:
        return QUEEN;
    case 10:
        return KING;
    case 1:
        return PAWN;
    }
}

void movePiece(globalMove globalMove, std::array<std::array<int, 8>, 8>& board) {
    int selectedPieceId = board[globalMove.from.y][globalMove.from.x];
    if (selectedPieceId == 0) {
        std::cout << "there is no piece!!!!!!\n";
        return;
    }

    board[globalMove.from.y][globalMove.from.x] = 0;
    board[globalMove.to.y][globalMove.to.x] = selectedPieceId;

    getPiece(abs(selectedPieceId)).AfterMove(board, globalMove);
}

std::string moveToString(globalMove globalMove) {
    return   std::string(1, abc[globalMove.from.x]) + std::to_string(globalMove.from.y + 1)
    + " "  + std::string(1, abc[globalMove.to.x])   + std::to_string(globalMove.to.y + 1);
}

std::array<std::array<int, 8>, 8> getAttackMap(std::array<std::array<int, 8>, 8> board, bool color, std::string moves) {
    std::array<std::array<int, 8>, 8> attackMap = std::array<std::array<int, 8>, 8>();
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int currSquare = board[y][x];
            if (color ? currSquare < 0 : currSquare>0) {
                std::vector<possibleMove> attackedSquares = getPiece(abs(currSquare)).GetPossibleMoves(board, attackMap, moves, { {x,y} }, true);
                for (possibleMove possibleMove : attackedSquares) {
                    attackMap[possibleMove.to.y][possibleMove.to.x] += 1;
                }
            }
        }
    }
    return attackMap;
}

std::vector<possibleMove> getSafeMoves(std::array<std::array<int, 8>, 8> board, std::vector<possibleMove> moves, position from, bool color, std::string movesNotation) {
    std::vector<possibleMove> safeMoves;

    for (possibleMove move : moves) {
        std::array<std::array<int, 8>, 8> virtualBoard = board;
        globalMove g;
        g.from.x = from.x;
        g.from.y = from.y;
        g.to.x = move.to.x;
        g.to.y = move.to.y;
        g.moveType = move.moveType;

        movePiece(g, virtualBoard);

        std::array<std::array<int, 8>, 8> vrtAttackMap = getAttackMap(virtualBoard, color, movesNotation);

        if (!checkForCheck(getKingPos(color, virtualBoard), vrtAttackMap))
            safeMoves.push_back(move);
    }
    return safeMoves;
}

std::vector<globalMove> getAllMoves(std::array<std::array<int, 8>, 8> board, std::array<std::array<int, 8>, 8> attackMap,  bool color, std::string movesNotation) {
    std::vector<globalMove> gMoves;
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++) {
            int currSquare = board[y][x];
            if (color ? currSquare > 0 : currSquare < 0) {
                std::vector<possibleMove> pMoves = getSafeMoves(board, getPiece(abs(currSquare)).GetPossibleMoves(board, attackMap, movesNotation, std::array<int, 2>{ x, y }), position{ x, y }, color, movesNotation);
                for (const auto pMove : pMoves) {
                    globalMove g;
                    g.from.x = x;
                    g.from.y = y;
                    g.to.x = pMove.to.x;
                    g.to.y = pMove.to.y;
                    g.moveType = pMove.moveType;
                    gMoves.push_back(g);
                }
            }
        }
    return gMoves;
}
