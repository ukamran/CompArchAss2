#include <stdio.h>
#include "FileHdr.h"
#define MAXROWS 100
#define MAXSTRLEN 10


void main()
{
	
	int num_modules;
	int count_modules;
	printf("Please select the number of modules to input. This must be an integer value greater than 2: \n");
	scanf("%d", &num_modules);
	getchar();
	char filename_arr[MAXROWS][MAXSTRLEN];
	char single_filename[MAXSTRLEN];
	printf("Great! You have selected %d modules. The first file you input will be the main file. \n", num_modules);

	count_modules = 0;
	do {
		printf("Please input the name of the xmo file %d \n", count_modules);
		scanf("%s", &filename_arr[count_modules]);
		printf("This is the resulting value %s and count number %d \n", filename_arr[count_modules], count_modules);
		getchar();
		count_modules++;
	} while (count_modules < num_modules);
	
	int i;
	
	for (i = 0; i < count_modules; i++) {
		//printf("%s \n", filename_arr[i]);
		read_file(filename_arr[i]);
	}
	//read the main file

	//read_file(filename_arr[0]);
	getchar();

} 