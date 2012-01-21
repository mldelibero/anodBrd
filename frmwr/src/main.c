#include <avr/io.h>
#include "avr_compiler.h"
#include "init.h"
#include "loop.h"
#include "test.h"
#include "dac.h"

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
