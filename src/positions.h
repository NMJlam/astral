#pragma once

#include "types.h"
#include <cstdint>
#include <sstream>
#include <string>

struct PositionSetError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

enum PieceType {
  ALL_PIECES,
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  KING,
  QUEEN,
  LEN_PIECES
};

enum ColourType { BLACK, WHITE, LEN_COLOURS };

class Positions {

private:
  Bitboard colourBB[LEN_COLOURS] = {};
  Bitboard pieceBB[LEN_PIECES] = {};
  ColourType sideToMove;
  uint8_t castlingRights{};
  Square enPassant;
  int halfmoveClock{};
  int fullmoveNumber{};

  void parse_positions(std::stringstream &ss);
  void parse_stm(std::stringstream &ss);
  void parse_castling(std::stringstream &ss);
  void parse_ep(std::stringstream &ss);
  int parse_moves(std::stringstream &ss);

public:
  Bitboard pieces();
  Bitboard pieces(PieceType pt);
  Bitboard pieces(ColourType ct);
  Bitboard pieces(ColourType ct, PieceType pt);
  template <typename... PieceTypes> Bitboard pieces(PieceTypes... pts) {
    return (pieceBB[pts] | ...);
  }

  void set(const std::string &fen_exp);
};
