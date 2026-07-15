#pragma once

#include <cstdint>
#include <sstream>
#include <string>

using Bitboard = uint64_t;

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
  void parse_positions(std::stringstream &ss);
  void parse_stm(std::stringstream &ss);

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
