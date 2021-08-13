/*
ReadFile.c: Reads each file and generates a symbol table at the very end.
 ECED 3403
 21 August 09
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define LSIZ 256 
#define RSIZ 256 
#define HEXARRSIZE 5
#define LENARRSIZE 3
#define MAXSYMBOLSIZE 50
#define MAXOFFSETSIZE 5
#define MAXTABLESIZE 20
//#define DEBUG
//int reloc_address=0;
int last_address;
int table_count = 0;

//this struct contains the symbols of L1 records
struct pubs
{
	char pub_symbol[MAXSYMBOLSIZE]; 
	int pub_symbol_addr;
};

//the L1 record symbols and addresses are collected in this array
struct pubs table[MAXTABLESIZE];

/*this function reads the input filename and parses the file searching for L1 records and S1 records.
* the symbol's addresses are also stored by adding the relocation address to the offset seen in the L1 record
*/
void read_file(char filename[], int filenum, int maxfiles) {
	FILE* file = fopen(filename, "r"); 

	char line[RSIZ][LSIZ];
	
	int i = 0;
	int tot = 0;
	
	//read and store the entire file
	while (fgets(line[i], sizeof(line[i]), file)) {
		/* insert a null terminator */
		// printf("%s", line);
		line[i][strlen(line[i])] = '\0';
		i++;
	}

	//set the last address to 0 if this is the first file
	if (filenum == 0) {
		last_address = 0;
	}

	tot = i;
	int publics = 0;
	int mod_st_address_found = 0;
	char mod_st_address_arr[HEXARRSIZE];
	char s1_address_arr[HEXARRSIZE];
	char reloc_addr_hex[HEXARRSIZE];
	char length_arr[LENARRSIZE];
	int s1_starting_address;
	int mod_starting_address;
	int num_bytes;
	int addr_cksum_bytes = 3;
	int interim_ra; //relocation address within a module relative to its starting address
	printf("\nFilename: %s  \n", filename);

	//read the entire file from the stored array
	for (i = 0; i < tot; i++)
	{
		//check for L1 records
		if (line[i][0] == 'L' && line[i][1] == '1') {
			char * token;
			token = strtok(line[i], " ");//extract l1 record and split it up into tokens
			// loop through the string to extract all other tokens
			char symbol[MAXSYMBOLSIZE];
			char offset_arr[MAXOFFSETSIZE];
			int offset;
			int offsetted_addr;
			//we need to extract the 2nd and 3rd tokens (name and offset)
			int token_count = 0;
			while (token != NULL) {
				
				//extract the symbol
				if (token_count == 1) {
					sscanf(token, "%s", symbol);
				}

				//extract the offset
				else if (token_count == 2) {
					sscanf(token, "%s", offset_arr);
				}
				token = strtok(NULL, " ");
				token_count++;
			}
			//null terminate offset array
			offset_arr[4] = 0;

			//convert the offset array into an int value
			offset = (int)strtol(offset_arr, NULL, 16);
#ifdef DEBUG
			printf("Offset array: %s\n", offset_arr);
			printf("Converted offset: %04x\n", offset);
			printf("Interim RA: %04x\n", interim_ra);
#endif // DEBUG

			offsetted_addr = last_address + interim_ra + offset;// this is the relocation address + the offset

			//store the symbol and the address in the table
			sscanf(symbol, "%s", table[table_count].pub_symbol);
			table[table_count].pub_symbol_addr = offsetted_addr;

			printf("Name: %s \t Offset: %04x \t Address with offset: %04x\n", symbol, offset, offsetted_addr);
			//printf("%s", line[i]);
			publics++;
			table_count++; //increment the table counter to point to the next row
		}

		//extract the S1 record
		else if (line[i][0] == 'S' && line[i][1] == '1') {

			//extract the starting address of the module
			if (mod_st_address_found == 0) {
				for (int addr_count = 0; addr_count < HEXARRSIZE; addr_count++) {
					mod_st_address_arr[addr_count] = line[i][4 + addr_count]; //4 is the offset value.
				}
				//null terminate
				mod_st_address_arr[4] = 0;
				//convert the starting address from a string array to int val. add to the last address. print the resulting starting address of the module.
				mod_starting_address = (int)strtol(mod_st_address_arr, NULL, 16)+last_address;
				printf("Module starting address: %04x\n", mod_starting_address);
				mod_st_address_found = 1; //set the module's starting address to 1 now that it has been found

			}

			//extract the starting address of the S1 record
			for (int addr_count = 0; addr_count < HEXARRSIZE; addr_count++) {
				s1_address_arr[addr_count] = line[i][4 + addr_count]; //4 is the offset value on the s1 record to get to the address.
			}
			//null terminate
			s1_address_arr[4] = 0;
			
			//convert the starting address from a string array to int val
			s1_starting_address= (int)strtol(s1_address_arr, NULL, 16);

			//acquire length array to get the number of bytes
			for (int len_count = 0; len_count < LENARRSIZE; len_count++) {
				length_arr[len_count] = line[i][2 + len_count]; //need to change magic number. 4 is the offset value.
			}
			//null terminate
			length_arr[2] = 0;
			num_bytes = (int)strtol(length_arr, NULL, 16) - addr_cksum_bytes;
#ifdef DEBUG
			printf("%s \n", length_arr);
			printf("Number of bytes: %02x\n", num_bytes);

#endif // DEBUG

			//get the relocation address relative to the module's starting address
			interim_ra = s1_starting_address + num_bytes;
			//reloc_address = reloc_address + last_address;
#ifdef DEBUG
			printf("Interim relocation address: %04x\n", interim_ra);

#endif // DEBUG

		}
		
		//printf("%s", line[i]);
	}

	//check to ensure the starting address is being appropriately printed from the array
#ifdef DEBUG
	printf("Starting address %s\n", mod_st_address_arr);
#endif

	//if there are no publics return the following diagnostic
	if (publics == 0) {
		printf("No publics. \n");
	}
	
	//at the end of the file, update the last address to the new last address
	last_address = interim_ra+last_address;
	interim_ra = 0; //need to reset the interim address for use by the next module
	//getchar();

	//normalize to count from 1 rather than 0
	int actualfilenum = filenum + 1;

	//create the symbol table once we have reached the last file
	if (actualfilenum == maxfiles) {
		printf("Symbol Table \n");
		printf("Symbol \t Address \n");
		printf("------ \t ------- \n");
		for (int numsymbols = 0; numsymbols < table_count; numsymbols++) {
			printf("%s \t %04x \n", table[numsymbols].pub_symbol, table[numsymbols].pub_symbol_addr);
		}
	}
	

	fclose(file);
}
