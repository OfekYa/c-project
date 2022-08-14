#include "head.h"
#include "linked_list_utils.h"
#include "utils.h"
#include "commands.h"

/*The function return the number of commas in the line array*/
int get_number_of_sign(char line[], char sign) {
	int i = 0;
	int num_of_sign = 0;
	while(line[i]){
		if (line[i] == sign)
			num_of_sign++;
		i++;	
	}
	return num_of_sign;
}

/*The function check if there is more argument or not*/
int check_if_token_is_empty(char *token){  
  token = strtok(NULL, delimit);
    if (token != NULL){
        return FALSE;
    }
    return TRUE;
}

int check_name(char *exp, int label_len, int num_of_line) {

/* The function gets a label name and checks that it meets all the required conditions
   * A valid label begins with an alphabet letter, followed by a series of letters and/or numbers without spaces.
   * The maximum length of a label is 31.  */

   	int i;
   	if(label_len > MAX_NAME_LEN) {
   		printf("Error in line %d: The maximum length of a label is 31: %s \n",num_of_line,exp);
   		return FALSE;
   	}
   	if (!isalpha(exp[FIRST_INDEX])){	
   		printf("Error in line %d: A valid label begins with an alphabet letter: %s\n",num_of_line,exp);
		return FALSE;
	}			
	for (i = 0; i < label_len; i++) {
		if ((exp[i] == SPACE_CHAR) || (exp[i] == TAB)) { 
			printf("Error in line %d: Spaces in the middle of the label name is invalid: %s \n",num_of_line,exp);
			return FALSE;
		}
		if (!isalnum(exp[i]))  {
			printf("Error in line %d: An invalid character is found in the label name: %s \n",num_of_line,exp);
			return FALSE;
		}
	}
	return TRUE;	
}

int check_if_name_exist(char label_name[]) { /* The function gets a label name and checks if it exists in the symbol table */

	symbol_tab *sym;
	sym = find_symbol(heads_list.symbols_head, label_name);
	
	if (sym != NULL) {
		return TRUE;
	}

	return FALSE;
}

/* *The function gets a pointer to a string and checks if there is a label.
*it is also checked whether the label name is valid: 
The character ':' must be attached to the label name.
There must be a space between the label name and the continuation of the sentence.
And other rules that are tested in calling the function: check_name */
int search_label(char *exp, int num_of_pass, int num_of_line) { 
	int i;
	for (i = 0; exp[i]; i++) {
		if (exp[i] == ':') {
			if (num_of_pass == SECOND_PASS) {
				return i+1;
			}
			if ((exp[i-1] == SPACE_CHAR) || (exp[i-1] == TAB)){
				printf("Error in line %d: The character ':' must be attached to the label name: %s",num_of_line,exp);
			}
			else if ((exp[i+1] != SPACE_CHAR) && (exp[i+1] != TAB)){
				printf("Error in line %d: There must be a space between the label name and the continuation of the sentence: %s",num_of_line,exp);
			}
			else if (check_name(exp, i, num_of_line)) { /* check if the label name meets the required conditions */
				if(!check_if_name_exist(exp)) { /* check if the label name is already in use */
					return i+1;
				}
				else {
					printf("Error in line %d: The label name is already in use: %s \n",num_of_line,exp);
				}	
			}
			return INVALID_LABEL_NAME;			
			}				    
	}
	return FALSE;		
}

	
int is_this_an_immediate_addressing(char *operand, int num_of_line){
    char *operand_name = operand;
    int i, first = 0;
    if (operand_name[FIRST_INDEX] == '#'){
        first = 1;
    }
    else{
        return FALSE;
    }
    if (operand_name[SECOND_INDEX] == '-'){
        first = 2;
    }
    if (!isdigit(operand_name[first])) {
    	printf("Error in line: %d !!! The operand value is invalid: %s\n", num_of_line, operand);
    	return FAILED;
    }
    for (i = first; operand_name[i]; i++){
        if (operand_name[i] == '.'){
            printf("Error in line: %d !!! The value does not belong to the Z group: %s \n", num_of_line, operand);
            return FAILED;
        }
        if (!isdigit(operand_name[i]) && ((operand[i] != SPACE_CHAR) && (operand[i] != TAB))){
            printf("Error in line %d !!! An invalid character: %c, is found in the operand value: %s \n", num_of_line, operand_name[i], operand);
            return FAILED;
        }
    }
    return TRUE;
}

int is_this_an_register_direct_addressing(char *operand, int num_of_line){
    char *operand_name = operand;
    int x, i;
    int vaild_num = TRUE;
    if (operand_name[FIRST_INDEX] == 'r'){
        for (i = SECOND_INDEX; operand_name[i]; i++){
            if (!isdigit(operand_name[i]))
            {
                vaild_num = FALSE;
            }
        }
        if (vaild_num){
            x = atoi(operand_name + 1);
            if ((x >= 0) && (x <= 15)){
                return TRUE;
            }
            else{
                printf("Error in line %d !!! The value of the register should be between 0 and 15: %s\n", num_of_line, operand);
                return FAILED;
            }
        }
    }
    return FALSE;
}

int is_this_an_index_addressing(char *operand, int num_of_pass, int num_of_line){
	char *operand_name = operand;
	char label_name[MAX_NAME_LEN] = {END_OF_STRING};
	int x, i,z, j = 0;
	int closing_bracket = FALSE, opening_bracket = FALSE;
	for (i = 0; operand_name[i]; i++){
		if (operand[i] && closing_bracket) {
			printf("Error in line: %d !!! invalid operand: %s\n", num_of_line, operand);
      			return FAILED;
		}
		if ((operand_name[i] == '[') && (!opening_bracket)){
			opening_bracket = TRUE;
			if (operand_name[i+1] != 'r'){
				 printf("Error in line: %d !!! invalid operand: %s\n", num_of_line, operand);
      				 return FAILED;
			}
			i = i + 2;
        		for (z = i; operand_name[z]; z++){
        			if (operand_name[z] == ']'){
        				closing_bracket = TRUE;
        				i=z;
        				break;
        			}     	
        			if(!isdigit(operand_name[z])) {
        				printf("Error in line: %d !!! invalid operand: %s\n", num_of_line, operand);
      				 	return FAILED;
        			}
        		}	       		
		}
		
		if ((operand_name[i] == '[') && (opening_bracket)){  
       			printf("Error in line: %d !!! invalid operand: %s\n", num_of_line, operand);
 			return FAILED;
 		}
	}
	if (!closing_bracket)
		return FALSE;

	else if ((closing_bracket) && (operand_name[i - 1] != ']')){
		printf("Error in line: %d !!! invalid operand: %s\n", num_of_line, operand);
		return FAILED;
    	}
	else {
		while (operand_name[j] != '['){
			label_name[j] = operand_name[j];
			j++;
        	}
		if (check_name(label_name, j - 2, num_of_line)){
			if (num_of_pass == SECOND_PASS){
                		if (!check_if_name_exist(label_name)){
                   			 printf("Error in line: %d !!! The label name: %s is not defined \n", num_of_line, label_name);
                   			 return FAILED;
                		}
            		}
		}
        	else {
		   	 return FAILED;
        	}
        j = j + 2;
        x = atoi(operand + j);
        if ((x < 10) || (x > 15)){
            printf("Error in line %d !!! The value of the register should be between 10 and 15: %s\n", num_of_line, operand);
            return FAILED;
        }
    }
    return TRUE;
}

/*      The function receives an operand and returns the correct addressing mode to it. 
	In case errors are found, a message will be printed for them and FAILED will be returned . 
*/
int get_addressing_modes(char *operand, int num_of_pass, int num_of_line){	
    int status;
    if ((status = is_this_an_immediate_addressing(operand, num_of_line))){
        if (status == FAILED){
            return FAILED;
        }
        return IMMEDIATE_ADDRESSING;
    }
    if ((status = is_this_an_register_direct_addressing(operand,num_of_line))){
        if (status == FAILED){
            return FAILED;
        }
        return REGISTER_DIRECT_ADDRESSING;
    }
    if ((status = is_this_an_index_addressing(operand, num_of_pass, num_of_line))){
        if (status == FAILED){
            return FAILED;
        }
        return INDEX_ADDRESSING;
    }
    if (num_of_pass == FIRST_PASS){
        if (check_name(operand, strlen(operand), num_of_line))
            return DIRECT_ADDRESSING;
    }
	if (num_of_pass == SECOND_PASS){
		if (check_if_name_exist(operand)){
			return DIRECT_ADDRESSING;
	   	}
		else{
			printf("Error in line: %d, label name doesnt exist: %s\n",num_of_line,operand);
       			 return FAILED; 
		}
	}
	return FAILED;
}

/*The function returns the index of the function in the commands array*/
int get_addressing_mode_of_operand(char *token, int number_of_line){
    int method_number;
    token = strtok(NULL, delimit);
    if (token == NULL){
        printf("Error missing operand in line: %d\n", number_of_line);
        return FAILED;
    }
    remove_white_spaces(token);
    method_number = get_addressing_modes(token, FIRST_PASS, number_of_line);
    /*If operand not a valid operand */
    if (method_number == FAILED){
        return FAILED;
    }
    if (!(check_if_token_is_empty(token))){
        printf("Error to much operands in line: %d\n", number_of_line);
        return FAILED;
    }
    return method_number;
}

int get_addressing_modes_of_two_operands(char *token, int number_of_line, int *method_of_first_operand, int *method_of_second_operand){    /*Get the first opernd */
    token = strtok(NULL, COMMA);
    if (token == NULL){
        printf("Error missing operand in line: %d\n", number_of_line);
        return FAILED;
    }
    del_edge_spaces(token);
    *method_of_first_operand = get_addressing_modes(token, FIRST_PASS, number_of_line);
    /*If operand not a valid operand */
    if (*method_of_first_operand == FAILED)
    {
        return FAILED;
    }
    /*Get the second opernd */
    token = strtok(NULL, delimit);
    if (token == NULL){
        printf("Error missing operand in line: %d\n", number_of_line);
        return FAILED;
    }
    remove_white_spaces(token);
    *method_of_second_operand = get_addressing_modes(token, FIRST_PASS, number_of_line);
    /*If operand not a valid operand */
    if (*method_of_second_operand == FAILED){
        return FAILED;
    }
    if (!(check_if_token_is_empty(token))){
        printf("Error to much operands in line: %d\n", number_of_line);
        return FAILED;
    }
    return SUCCEEDED;
}

int push_symbol_data(char* label_name, int value, int base_address, int offset, int attributes) {

	symbol_tab *sym;
	
	sym = fill_data(heads_list.symbols_head, label_name, value, base_address, offset, attributes);
	if(sym == NULL){
		
		return FAILED;
	}
    	return SUCCEEDED;
}
 
void push_symbol_name(char label_name[]){
	
	symbol_tab *sym;
	sym = new_symbol(label_name);
	if(sym != NULL)
		heads_list.symbols_head = add_symbol(heads_list.symbols_head, sym); 
	
}


int type_of_guidance(char line[]) { /* is this data/string/entry/extern guidance?
				       in case of errors in the guidence name, an error will be printed */
	if ((!strncmp(DATA, line, 5)) && isspace(line[5])) {		
		return DATA_GUIDANCE_LINE;
	}		
	if ((!strncmp(STRING, line, 7)) && isspace(line[7])) {
		return STRING_GUIDANCE_LINE;
	}		 
	if ((!strncmp(ENTRY, line, 6)) &&  isspace(line[6])) {
		return ENTRY_GUIDANCE_LINE;
	 }
	if ((!strncmp(EXTERN, line, 7))  &&  isspace(line[7])) {	
		return EXTERN_GUIDANCE_LINE;
	}
	return FAILED;
}

int push_symbol_after_extern(char line[], int num_of_line) {/* Add the symbol to the symbols table in the following case: .extern label */
	char *tmp_line;
	int len;
	tmp_line = line + 8;
	del_all_spaces(tmp_line);
	len = strlen(tmp_line) - 2;
	if (check_name(tmp_line, len, num_of_line)) {
		if(!check_if_name_exist(tmp_line)) {
			push_symbol_name(tmp_line);
			return SUCCEEDED;
		}
		else {
			printf("Error in line %d: The label name: %s is already exist \n",num_of_line, line);
		}
	}
	return FAILED;

}
int handle_guidance(char line[],int num_of_pass, int num_of_line) {
	int guidance_type; 
	char *label_name;
	guidance_type = type_of_guidance(line);
	if (guidance_type == EXTERN_GUIDANCE_LINE) {
		if(num_of_pass == SECOND_PASS) { /* For extern we already perform what is required in the first pass */
			return EXTERN_GUIDANCE_LINE;
		}
		if (push_symbol_after_extern(line, num_of_line)) { /* We will add the label name after ".extern" to the symbol table */
			return EXTERN_GUIDANCE_LINE;
		}
		return INVALID_LABEL; /* FAILED !!! error should be ON */
	}
	if (guidance_type == ENTRY_GUIDANCE_LINE) {
		if (num_of_pass == SECOND_PASS) { /* For entry we will perform operations only in the second pass */
			label_name = line + 7; /*  .entry label ---> label */
			del_all_spaces(label_name);
			if (check_if_name_exist(label_name)) {
				add_attributes(heads_list.symbols_head, label_name, ENTRY_ATTRIBUTE);/* update the attribute[1] of the symbol to ENTRY */
			}
			else {
				printf("Error in line: %d, the lable name: %s doesn't exist\n",num_of_line, label_name);
				return INVALID_LABEL;
			}
		}
		return ENTRY_GUIDANCE_LINE;

	}
	if (guidance_type == STRING_GUIDANCE_LINE) {
		return STRING_GUIDANCE_LINE;
	}
	if (guidance_type == DATA_GUIDANCE_LINE) {
		return DATA_GUIDANCE_LINE;
	}
	printf("Error in line: %d, an unkonwn guidnace name: %s \n", num_of_line, line);
	return UNKNOWN_GUIDANCE_TYPE; /* FAILED !!! error should be TRUE */
}
int lookup_cmd(char *token){
    int i;
    for (i = 0; i < NUMBER_OF_FUNCTIONS; i++){
        char command[50];
        int len;
        strcpy(command, commands[i].name);
        len = strlen(commands[i].name);
        command[len] = NEW_LINE;                                                     /*Add \n to the command, in case that the command entered alone*/
        command[len + 1] = END_OF_STRING;                                            /*Close the string*/
        if ((strcmp(token, commands[i].name) == 0) || (strcmp(token, command) == 0)) /*Checks if the command equals to one of the known commands*/
            return i;
    }
    return -1;
}
int get_type_of_line(char line[], int *label_exist, int num_of_pass, int num_of_line) {

/* * check is this empty/comment/instruction/guidance line ?
   * also uses functions that check the validity of the label name and adds their name to the symbol table if applicable.
*/
	int possible_label;
	char *exp;
	int guidance_type;
	char tmp[BUFFERSIZE];
	strcpy(tmp, line);
	if (tmp[FIRST_INDEX] == NEW_LINE) { 
		return EMPTY_LINE;
	}	
	if (tmp[FIRST_INDEX] == ';') { 
		return COMMENT_LINE;
	}
	if (tmp[FIRST_INDEX] == '.') {
		guidance_type = handle_guidance(tmp, num_of_pass, num_of_line); /* .extern label1  //  .entry label2  //  .data 6,-9  //  .string "abcd" */
		return guidance_type;
	}			
	if((possible_label = search_label(tmp, num_of_pass, num_of_line))) {
		*label_exist = TRUE;
		
		if (possible_label == INVALID_LABEL_NAME) {/*  lab$el  //  3label  //  lab el  */
			return INVALID_LABEL;
		}
		exp = tmp + possible_label; /* label1: add r3,r2 ----> add r3,r2   //   label2: .entry K ----> .entry K */
		exp = del_prim_spaces(exp);
		if (exp[FIRST_INDEX] == '.') {
			guidance_type = handle_guidance(exp, num_of_pass, num_of_line);
			if ((guidance_type == DATA_GUIDANCE_LINE) || (guidance_type == STRING_GUIDANCE_LINE)) { /* label1: .data 6,-9 */
				if (num_of_pass == FIRST_PASS) {
					take_first_word(tmp); /* label: .data 6,-9 ----> label */
					if (check_if_name_exist(tmp)){
						printf("Error in line: %d, the lable name: %s already exist\n",num_of_line, tmp);
						return FAILED;
					}
					push_symbol_name(tmp);
				}
			}	
			return guidance_type;
		}
		take_first_word(exp); /* add r3,r2 -----> add  //   prn #48 ----> prn */
		if (lookup_cmd(exp) != -1) { /* exp belongs to the list of instructions */
			if (num_of_pass == SECOND_PASS) {
				return INSTRUCTION_LINE;	
			}
			take_first_word(tmp); /* label: add r5,r7 ----> label */
			if (check_if_name_exist(tmp)){
				printf("Error in line: %d, the lable name: %s allready exist\n",num_of_line, tmp);
				return FAILED;
			}
			push_symbol_name(tmp); 
			return INSTRUCTION_LINE;
		}
		else {
			printf("Error in line: %d, unkown input: %s\n",num_of_line, exp);
			return FAILED; /* label: HELLO  //  label:  234 */
		}
	}
	take_first_word(tmp); /* sub r5,r2 -----> sub */	

	if(lookup_cmd(tmp) != -1) { /* line belongs to the list of instructions */
		return INSTRUCTION_LINE;	
	}
	printf("Error in line: %d, unkown input: %s",num_of_line, line);
	return FAILED;
}




