/* Convert an unsigned int to a C string. */#if defined (__cplusplus)	extern "C"#endifchar* uitoa (unsigned int ui, char* s);/*================================================================================*/char* uitoa (unsigned int ui, char* s) {/*----------------------------------------------------------------------------------	Abstract:			Parameters and modes:	Returns:	Change Log:----------------------------------------------------------------------------------*/	unsigned int divisor = 10;	unsigned int i;	char* end_s;			/* First find the biggest "place" in the number -- ones, tens, hundreds, etc.*/	while (ui / divisor)	{		divisor *= 10;	}	divisor /= 10;		/* Compute the digit in each place in the number and put it into s */	for (end_s = s; divisor >= 1; divisor /= 10)	{		i = ui / divisor;		i += '0';		ui %= divisor;		*end_s = (char) i;		++end_s;		*end_s = '\0';	}		return s;}