#include "head.h"
#include "createfiles.h"
#include "linked_list_utils.h"

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Zero argument were passed to the program in the command line.\n\n");
		return 0;
	}	 
	handle_files(argc, argv);	
	return 0;
}

void handle_files(int argc,char *argv[]) {
	FILE *fp1; /* pointer to primary source file */
	FILE *fp2; /* pointer to extended source file */
	int i;
	int ICF;
	int DCF;
	char file_name[250];


	for (i = 1; i < argc; i++) {
		ICF = 0;
		DCF = 0;
		
		strcpy(file_name, argv[i]);
		fp1 = fopen(strcat(file_name, AS_FILE_EXTENSION), "r"); /* We will use the source file for reading only */
		if (fp1 == NULL) {
		        printf("Cannot open file %s \n", argv[i]);
		        continue;
		}
		if (check_if_file_is_empty(fp1)) {
			printf("File %s is empty \n", argv[i]);
			continue;
		}
		change_type_of_file(file_name,AM);
		fp2 = fopen(file_name ,"w+");
		pre_assembler(fp1, fp2); /* now fp2 is the extended source file */
		
		fclose(fp1);		
		fclose(fp2);		
		fp2 = fopen(file_name,"r");
		if (first_pass(fp2, &ICF,&DCF) == FAILED) {
			fclose(fp2);		
			free_tables();
			continue;
		}
		fseek(fp2,0,SEEK_SET);
		if (second_pass(fp2) == FAILED) {
			fclose(fp2);				
			free_tables();
			continue;
		}
		fclose(fp2);		
		create_files(file_name, &ICF, &DCF);
		free_tables();
	}
}