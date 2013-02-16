#include <unistd.h>               /* for sleep() function                */
#include <stdlib.h>
#include <stdio.h>

int main() {
	Byte crap = 0x30;
	int boost = 3;
	crap = crap + (Byte)boost;
	printf("Crap: %d\n", crap);
	return 0;
}
