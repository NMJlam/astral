#include "positions.h"
#include "types.h"
#include <cctype>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

struct PieceInfo {
  PieceType pt;
  ColourType ct;
};

constexpr std::array<PieceInfo, 128> makePieceLookup() {
  std::array<PieceInfo, 128> t{};
  t['P'] = {PAWN, WHITE};
  t['N'] = {KNIGHT, WHITE};
  t['B'] = {BISHOP, WHITE};
  t['R'] = {ROOK, WHITE};
  t['Q'] = {QUEEN, WHITE};
  t['K'] = {KING, WHITE};
  t['p'] = {PAWN, BLACK};
  t['n'] = {KNIGHT, BLACK};
  t['b'] = {BISHOP, BLACK};
  t['r'] = {ROOK, BLACK};
  t['q'] = {QUEEN, BLACK};
  t['k'] = {KING, BLACK};
  return t;
}

constexpr auto PIECE_LOOKUP = makePieceLookup();

// private
void Positions::parse_positions(std::stringstream &ss) {

  ss >> std::noskipws;

  int file = FILE_A;
  int rank = RANK_8;

  for (;;) {
    unsigned char token;

    if (!(ss >> token)) {
      throw PositionSetError("Invalid FEN. Unexpected end of stream.");
    }

    if (isspace(token)) {
      break;
    }

    if (isdigit(token)) {
      const int diff = (token - '0');

      if (diff < 1 || diff > 8) {
        throw PositionSetError(
            "Invalid FEN. Invalid number of squares to skip.");
      }

      file += diff;
      if (file > FILE_NB) {
        throw PositionSetError("Invalid FEN. File out of bounds.");
      }

    } else if (token == '/') {
      if (file < FILE_NB) {
        throw PositionSetError(
            "Invalid FEN. Trying to end rank when not at end.");
      }
      --rank;
      file = FILE_A;
    } else {
      constexpr std::string_view valid_pieces = "PNBRQKpnbrqk";
      if (valid_pieces.find(token) == std::string_view::npos) {
        throw PositionSetError("Invalid FEN. Unknown piece character.");
      }
      const auto [pt, ct] = PIECE_LOOKUP[token];
      Bitboard pos = 1ULL << (rank * 8 + file);
      pieceBB[ALL_PIECES] |= pos;
      pieceBB[pt] |= pos;
      colourBB[ct] |= pos;
      ++file;
    }
  }
};

// public
Bitboard Positions::pieces() { return Positions::pieceBB[ALL_PIECES]; }

Bitboard Positions::pieces(PieceType pt) { return Positions::pieceBB[pt]; }

Bitboard Positions::pieces(ColourType ct) { return Positions::colourBB[ct]; }

Bitboard Positions::pieces(ColourType ct, PieceType pt) {
  return Positions::colourBB[ct] & Positions::pieceBB[pt];
}

void Positions::set(const std::string &fen_exp) {
  std::stringstream ss(fen_exp);
  parse_positions(ss);
  // NOTE: parsing of the additional flags might be needed later.
}
