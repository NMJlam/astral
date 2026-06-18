#include "positions.h"

int main() {
  constexpr auto StartFEN =
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Positions p;
  p.set(StartFEN);
}
