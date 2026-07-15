>[CPW link](https://www.chessprogramming.org/Bitboards)

Bitboards are implemented in the `positions.h` and `positions.cpp` . The `Positions` class implements the bitboard through an array of `uint64_t`. 

**There are 2 main bitboard arrays:**

**1. Colours**
  ```cpp
  Bitboard colourBB[LEN_COLOURS] = {};
  ```
- This represents all the black and white pieces on the chess board

**2. Piece type** 
```cpp
Bitboard pieceBB[LEN_PIECES] = {};
```
- This represents all the piece types such as rook, bishop, king etc. 


**Reasoning:**

*1. Fast piece query:*
`AND` ing a colour bitboard with a piece bitboard lets us quickly query a specific piece of a specific colour (e.g. `colourBB[WHITE] & pieceBB[ROOK]` = white rooks). 

`OR` is used to union multiple piece types into a single set to calculate moves for pieces with overlapping move sets. For example, rooks and queens both slide orthogonally, so their orthogonal moves can be generated together over `pieceBB[ROOK] | pieceBB[QUEEN]`.

The `pieceBB` indexes correspond to the enum `PieceType`, and the `colourBB` indexes correspond to the enum `ColourType`.

*2. Memory Efficiency*
The bitboards are fixed-size, contiguous arrays stored inline as members of `Positions`, so querying them needs no pointer indirection and no separate heap allocation.