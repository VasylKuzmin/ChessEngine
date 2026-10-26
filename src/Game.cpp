#include "Game.h"

#include "Board.h"
#include "Pieces/Piece.h"

void Game::handleClick(Board& board, const std::pair<float, float> coords, float squareSize)
{
    if (0 < coords.first < squareSize && squareSize * 8 < coords.second < squareSize * 9)
    {
        board.undoLastMove();
        board.undoLastMove();
    }
}

void Game::handlePositionClick(Board& board, const std::optional<Position> clickPosition)
{
    if (gameState != GameState::Playing)
        return;

    if (!clickPosition)
        return;

    if (promotionPositions[0])
    {
        size_t i = 0;
        for (auto position : promotionPositions)
        {
            if (clickPosition == position)
            {
                PieceType promotionType = (i == 0)   ? PieceType::Queen
                                          : (i == 1) ? PieceType::Rook
                                          : (i == 2) ? PieceType::Bishop
                                                     : PieceType::Knight;
                board.promoteLastMovedPiece(promotionType, player);
                for (auto& pos : promotionPositions)
                {
                    pos = std::nullopt;
                }
                player = (player == Color::White) ? Color::Black : Color::White;
                return;
            }
            i++;
        }
        return;
    }

    Piece* piece = board.getPiece(*clickPosition);

    if (piece && piece->getColor() == player)
    {
        selectedPosition = clickPosition;
        selectedMoves = board.getLegalMoves(*piece);
    }
    else
    {
        handleMove(board, *clickPosition);
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
            else if (board.getPiece(move.to)->getType() == PieceType::Pawn &&
                     (move.to.getVertical() == 1 || move.to.getVertical() == 8))
            {
                if (move.to.getVertical() == 1)
                {
                    promotionPositions = {Position::fromValid(move.to.getHorizontal(), 4),
                                          Position::fromValid(move.to.getHorizontal(), 3),
                                          Position::fromValid(move.to.getHorizontal(), 2),
                                          Position::fromValid(move.to.getHorizontal(), 1)};
                }
                else
                {
                    promotionPositions = {Position::fromValid(move.to.getHorizontal(), 8),
                                          Position::fromValid(move.to.getHorizontal(), 7),
                                          Position::fromValid(move.to.getHorizontal(), 6),
                                          Position::fromValid(move.to.getHorizontal(), 5)};
                }
            }
            else
            {
                player = opponent;
            }
            
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

std::array<std::optional<Position>, 4> Game::getPromotionPositions() const
{
    return promotionPositions;
}

Color Game::getCurrentPlayer() const
{
    return player;
}

GameState Game::getGameState() const
{
    return gameState;
}