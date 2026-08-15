#include "Game.h"

#include "Board.h"
#include "Pieces/Piece.h"

#include <iostream>

void Game::handleClick(Board& board, const Position clickPosition)
{
    if (gameState != GameState::Playing)
        return;

    Piece* piece = board.getPiece(clickPosition);

    if (piece && piece->getColor() == player)
    {
        selectedPosition = clickPosition;
        selectedMoves = board.getLegalMoves(*piece);
    }
    else
    {
        handleMove(board, clickPosition);
    }
}

void Game::handleMove(Board& board, const Position clickPosition)
{
    Color opponent = (player == Color::White) ? Color::Black : Color::White;

    for (const Move& move : selectedMoves)
    {
        if (clickPosition == move.to)
        {
            board.movePiece(move);
            if (board.isCheckmate(opponent))
            {
                gameState = GameState::Checkmate;
            }
            else if (board.isStalemate(opponent))
            {
                gameState = GameState::Stalemate;
            }
            else
            {
                player = opponent;
            }

            selectedPosition = std::nullopt;
            selectedMoves.clear();
            return;
        }
    }

    selectedPosition = std::nullopt;
    selectedMoves.clear();
}

std::optional<Position> Game::getSelectedPosition() const
{
    return selectedPosition;
}

std::vector<Move> Game::getSelectedMoves() const
{
    return selectedMoves;
}