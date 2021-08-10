/*
 OutputFile.c: Creates a file output or appends to an existing file.
 ECED 3403
 21 August 09
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#define LSIZ 256 
#define RSIZ 256 
/*
char name_of_file[100];
unsigned int length_of_name;

//this function creates a file with an input filename and a prescreibed length of the filename
void create_file(char filename[], unsigned int filename_length) {
	// set the filename locally here
	int i;
	length_of_name = filename_length;
	//store the name of the file in the global variable for use by the append_file function
	for (i = 0; i < filename_length; i++) {
		name_of_file[i] = filename[i];
	}

	//pointer to the file
	FILE* fp;
	//create the file with the set filename
	fp = fopen(filename, "w");
	//close the file
	fclose(fp);
}

void append_file(char output[]) {
	FILE* fp;
	//open the filename set in the global variable
	fp = fopen(name_of_file, "a");
	//append the char array output[] to the file  
	fprintf(fp, "%s\n", output);
	//close the file 
	fclose(fp);
}
*/

/*
void read_file(char filename[]) {
	FILE* file = fopen(filename, "r"); // should check the result 
	char line[256];

	while (fgets(line, sizeof(line), file)) {
		//note that fgets don't strip the terminating \n, checking its
		//   presence would allow to handle lines longer that sizeof(line) 
		printf("%s", line);
	}
	getchar();
	// may check feof here to make a difference between eof and io failure -- network
	//   timeout for instance 

	fclose(file);
}
*/

void read_file(char filename[]) {
	FILE* file = fopen(filename, "r"); /* should check the result */
	char line[RSIZ][LSIZ];

	int i = 0;
	int tot = 0;
	//
	while (fgets(line[i], sizeof(line[i]), file)) {
		/* note that fgets don't strip the terminating \n, checking its
		   presence would allow to handle lines longer that sizeof(line) */
		// printf("%s", line);
		line[i][strlen(line[i])] = '\0';
		i++;
	}

	tot = i;
	int publics = 0;
	int st_address_found = 0;
	char st_address[5];
	printf("\nFilename: %s  \n", filename);
	for (i = 0; i < tot; i++)
	{
		if (line[i][0] == 'L' && line[i][1] == '1') {
			printf("%s", line[i]);
			publics++;
		}
		else if (line[i][0] == 'S' && line[i][1] == '1') {
			if (st_address_found == 0) {
				for (int addr_count = 0; addr_count < 5; addr_count++) {
					st_address[addr_count] = line[i][4 + addr_count];
				}
				st_address[4] = 0;
				//st_address = { line[i][4], line[i][5],line[i][6],line[i][7],0 };
				st_address_found = 1;
			}
		}
		
		//printf("First character: %c\n", line[i][0]);
		//printf("%s", line[i]);
	}

	printf("Starting address %s\n", st_address);
	if (publics == 0) {
		printf("No publics. \n");
	}

	//getchar();

	/* may check feof here to make a difference between eof and io failure -- network
	   timeout for instance */

	fclose(file);
}

/*
int main(void)
{
	char line[RSIZ][LSIZ];
	char fname[20];
	FILE* fptr = NULL;
	int i = 0;
	int tot = 0;
	printf("\n\n Read the file and store the lines into an array :\n");
	printf("------------------------------------------------------\n");
	printf(" Input the filename to be opened : ");
	scanf("%s", fname);

	fptr = fopen(fname, "r");
	while (fgets(line[i], LSIZ, fptr))
	{
		line[i][strlen(line[i]) - 1] = '\0';
		i++;
	}
	tot = i;
	printf("\n The content of the file %s  are : \n", fname);
	for (i = 0; i < tot; ++i)
	{
		printf(" %s\n", line[i]);
	}
	printf("\n");
	return 0;
}
*/