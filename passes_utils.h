int lookup_cmd(char* token);
int check_if_token_is_empty(char *token);
int get_type_of_line(char line[], int *label_exist, int num_of_pass, int num_of_line);
int check_name(char *exp, int label_len, int num_of_line);
int search_label(char *exp, int num_of_pass, int num_of_line);
int get_addressing_modes(char *operand, int num_of_pass, int num_of_line);
int get_addressing_mode_of_operand(char *token, int number_of_line);
int get_addressing_modes_of_two_operands(char *token, int number_of_line, int *method_of_first_operand, int *method_of_second_operand);
int get_number_of_sign(char line[], char sign);
void push_symbol_data(char* label_name, int value, int base_address, int offset, int attributes);
int check_if_name_exist(char label_name[]);
void push_symbol_name(char label_name[]);


