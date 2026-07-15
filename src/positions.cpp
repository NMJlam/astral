#include "positions.h"
#include "types.h"
#include <array>
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

  // parse the board positions
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
      if (file >= FILE_NB || rank < RANK_1) {
        throw PositionSetError("Invalid FEN. Piece out of bounds.");
      }
      const auto [pt, ct] = PIECE_LOOKUP[token];
      const Square sq = make_square(File(file), Rank(rank));
      set_bit(pieceBB[ALL_PIECES], sq);
      set_bit(pieceBB[pt], sq);
      set_bit(colourBB[ct], sq);
      ++file;
    }
  }

  if (rank != RANK_1 || file != FILE_NB) {
    throw PositionSetError("Invalid FEN. File and rank is not an 8x8 board.");
  }
};

void Positions::parse_stm(std::stringstream &ss) {
  // parsing the sideToMove
  char stm;
  if (!(ss >> stm)) {
    throw PositionSetError("Invalid FEN. Unexpected end of stream.");
  }
  if (stm != 'w' && stm != 'b') {
    throw PositionSetError("Invalid FEN. Unknown side to move.");
  }
  sideToMove = (stm == 'w') ? WHITE : BLACK;

  // ignore the following space
  ss.ignore(1);
}
// public
Bitboard Positions::pieces() { return Positions::pieceBB[ALL_PIECES]; }

Bitboard Positions::pieces(PieceType pt) { return Positions::pieceBB[pt]; }

Bitboard Positions::pieces(ColourType ct) { return Positions::colourBB[ct]; }

Bitboard Positions::pieces(ColourType ct, PieceType pt) {
  return Positions::colourBB[ct] & Positions::pieceBB[pt];
}

void Positions::set(const std::string &fen_exp) {
  std::stringstream ss(fen_exp);
  ss >> std::noskipws;
  parse_positions(ss);
  // NOTE: parsing of the additional flags might be needed later.
  parse_stm(ss);
}
