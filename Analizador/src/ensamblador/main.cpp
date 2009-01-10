#include <stdio.h>

extern "C" int __stdcall Addup (int, int, int); // Linea extraña...

int main(int argc, char* argv[])
{
	int suma;
	suma = Addup (155, 1, 1);
	printf("Hello World! La suma dio: %d.\n",suma);
	return 0;
}

