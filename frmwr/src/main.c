#include <avr/io.h>
#include "init.h"
#include "loop.h"
#include "test.h"

int main(void)
{
	init();
	test();
	
	while(1)
    {
		loop();
	}		
}
//eof
