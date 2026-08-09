#include "DoubleDecker.h"

int main()
{
	Pawn pawn(Position('e', 2));
	pawn.displayPosition();
	pawn.move(Position('e', 4));
	pawn.displayPosition();
	return 0;
}
