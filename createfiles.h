void pre_assembler(FILE *fp1, FILE *fp2);
void handle_files(int argc,char *argv[]);
int first_pass(FILE *fp2 ,int *ICF,int *DCF);
int second_pass(FILE *fp2);
void create_files(char *file_name, int *ICF, int *DCF) ;
void change_type_of_file(char *file_name, int mode);
int check_if_file_is_empty(FILE *fp);
