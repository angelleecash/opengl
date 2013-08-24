#include "util.h"

char _utilBuffer[1024];

void Dump(char* p, int n)
{
	int i;
	for(i=0; i < n; i++)
	{
		P("0x%02x ", *(p+i) & 0xff);
		if(i > 0 && (i&0x7) == 7)
		{
			P("\n");
		}
	}

	P("\n");
}
