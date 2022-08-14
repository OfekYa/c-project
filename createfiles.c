#include "head.h"
#include "createfiles.h"
#include "linked_list_utils.h"

int binary_to_hex(long int binary_value) { /*  Conversion from binary representation to hexadecimal representation.
 					       For example: 1101b --> 0xd  */
	long int hexadecimal_value = 0, i = 1, remainder;
	while (binary_value != 0) {
		remainder = binary_value % 10;
		hexadecimal_value = hexadecimal_value + remainder * i;
		i = i * 2;
		binary_value = binary_value / 10;
	}
	return hexadecimal_value;
}
int read_four_bits_as_int(line_in_data_image tmp, int index) { /* The function reads the four requested bits from the encoded word */
	char num[4];
	int start = 19 - index;
	int end = 15 - index;
	int i, j;
	int binary_value;
	for(j = 0, i = start; i > end; i--, j++) {
		num[j] = tmp->word[i];
	}
	binary_value = atoi(num);
	return binary_value;
}

/* The function creates the object file according to the given instructions */
void make_object_file(line_in_data_image head_of_data_image, FILE *object_fp, int *ICF, int *DCF){

	line_in_code_image tmp1 = heads_list.code_image_head;
	line_in_data_image tmp2 = heads_list.data_image_head;
	int binary_value;
	int hexadecimal_value = 0;
	int address = 100;
	int i;
	int size_of_data = *DCF;
	int size_of_instructions = *ICF - size_of_data - 100;
	fprintf(object_fp, "%d %d", size_of_instructions, size_of_data);
	while (tmp1 != NULL) {
		fprintf(object_fp, "\n%04d	", address);
		for(i = 0; i < 5; i++) {
			binary_value = read_four_bits_as_int(tmp1, i*4);

			hexadecimal_value = binary_to_hex(binary_value);

			fprintf(object_fp, "%c%x", i + 65, hexadecimal_value);
			if(i < 4) {
				fprintf(object_fp, "%c", '-');
			}
		}
		address += 1;	
		tmp1 = tmp1->next;	
	}
	while (tmp2 != NULL) {
		fprintf(object_fp, "\n%04d	", address);
		for(i = 0; i < 5; i++) {
			binary_value = read_four_bits_as_int(tmp2, i*4);

			hexadecimal_value = binary_to_hex(binary_value);

			fprintf(object_fp, "%c%x", i + 65, hexadecimal_value);
			if(i < 4) {
				fprintf(object_fp, "%c", '-');
			}
		}
		address += 1;	
		tmp2 = tmp2->next;	
	}
}

/* The function creates the entries and externals files according to the given instructions */
void make_entries_and_externals_files(symbol_tab *symbols_head,FILE *entry_fp, FILE *extern_fp) {

	symbol_tab *tmp = symbols_head;
	while (tmp != NULL) {	
	
		if (tmp->attributes[SECOND_INDEX] == ENTRY_ATTRIBUTE) {
			fprintf(entry_fp, "%s,%04d,%04d \n",tmp->symbol_name, tmp->base_address, tmp->offset);
				
		}
		
		if (tmp->attributes[SECOND_INDEX] == EXTERNAL_ATTRIBUTE) {
				
			fprintf(extern_fp, "%s %s %04d \n",tmp->symbol_name, "BASE", tmp->base_address);	
			fprintf(extern_fp, "%s %s %04d \n\n",tmp->symbol_name, "OFFSET", tmp->offset);	
		}
		tmp = tmp->next;
	}

}

int check_if_file_is_empty(FILE *fp) {
	fseek (fp, 0, SEEK_END);
    	if (!ftell(fp)) {
		return TRUE;
	}
	fseek (fp, 0, SEEK_SET);
	return FALSE;
}


void change_type_of_file(char *file_name, int mode) {/* The function changes the extension of the file name as needed */
	file_name = strtok(file_name, ".");
	if (mode == AM) {
		strcat(file_name,".am");	
	}
	if (mode == OB) {
		strcat(file_name,".ob");
	}
	if (mode == ENT) {
		strcat(file_name,".ent");
	}
	if (mode == EXT) {
		strcat(file_name,".ext");
	}
}
void create_files(char *file_name, int *ICF, int *DCF) { /* The function creates the required output files */

	FILE *object_fp; /* pointer to object file */
	FILE *entry_fp; /* pointer to entries file */
	FILE *extern_fp; /* pointer to externals file */
	

	
	change_type_of_file(file_name, OB);
	object_fp = fopen(file_name,"w+");
	make_object_file(heads_list.data_image_head, object_fp, ICF, DCF);
		
	change_type_of_file(file_name, ENT);
	entry_fp = fopen(file_name,"w+");
		
	change_type_of_file(file_name, EXT);			
	extern_fp = fopen(file_name,"w+");
		
	make_entries_and_externals_files(heads_list.symbols_head, entry_fp ,extern_fp);
	
	if(check_if_file_is_empty(entry_fp)) {
		change_type_of_file(file_name, ENT);
		remove(file_name);
	}
	if(check_if_file_is_empty(extern_fp)) {
		change_type_of_file(file_name, EXT);			
		remove(file_name);
	}
	fclose(object_fp);
	fclose(entry_fp);
	fclose(extern_fp);
		
}

