#include "head.h"
#include "utils.h"
#include "passes_utils.h"
#include "linked_list_utils.h"

/*Push '1' to the word array in the index*/
void push_bit_to_data_word(int index, line_in_data_image new_node){
    new_node->word[index] = TURN_ON;
}

/*The function converts the decimal number to binary and push the bits to the new node in the linked list*/
void push_decimal_to_data_word(int dec, line_in_data_image new_node){
    int i, j;
    for (i = 0; i <= 15; i++){
        j = dec >> i;
        if (j & 1){          /*Checks if the bit equal to '1'*/
            push_bit_to_data_word(i, new_node);
        }
    }
    push_bit_to_data_word(INDEX_OF_A, new_node);    /*Turn on the bit in index of A*/
}

void push_char_to_data_image(char c, line_in_data_image new_node){
    int ascii_value = (int)c;
    push_decimal_to_data_word(ascii_value, new_node);
}

void push_number_to_data_image(char *number, line_in_data_image new_node, int num_of_line, int *error){
    int number_value = string_to_int(number, num_of_line, error);
    push_decimal_to_data_word(number_value, new_node);
}
/*The function add new words to the data-image for each number in the number list*/
int handle_data(char *token, int num_of_line, int *error){
    line_in_data_image new_node;
    int length = 0;
    token = strtok(NULL, COMMA); /*Catch the first number*/ 
    if (token == NULL){
        return length;
    }
    while (token != NULL){
        remove_white_spaces(token);
        if (*token == END_OF_STRING){
            printf("Error, A comma appears in an invalid place in the line: %d\n", num_of_line);
            return FAILED;
        }
        /*Add new word to the data-image and push the value to the word*/ 
        new_node = add_data_node(heads_list.data_image_head);
        push_number_to_data_image(token, new_node, num_of_line, error);
        length++;
        token = strtok(NULL, COMMA); /*Catch the next number*/
    }
    return length;
}

/*The function handles the .string lines*/
int handle_string(char *token, int number_of_line){
    int length;
    char *s;
    int i;
    line_in_data_image new_node;
    token = strtok(NULL, "\"");
    if (token == NULL){
        printf("Error, missing operand in line: %d\n", number_of_line);
        return FAILED;
    }
    length = strlen(token) + 1;    /*The number of new lines in the data-image */
    s = token;
    for (i = 0; i < length - 1; i++, s++){
        new_node = add_data_node(heads_list.data_image_head);   /*Add new word to the list*/
        push_char_to_data_image(*s, new_node); /*Push the char's value to the word node*/
    }
    if (*s != END_OF_STRING){ /*Check that the last char is \0*/
        printf("Error,2 not a valid string in line: %d    %c\n", number_of_line, *s);
        return FAILED;
    }
    new_node = add_data_node(heads_list.data_image_head);
    push_char_to_data_image(END_OF_STRING, new_node);     /*Add new word for \0 and push the value to the word*/

    if (!(check_if_token_is_empty(token))){             
        printf("Error, to much operands in line: %d\n", number_of_line);
        return FAILED;
    }
    return length;
}

/*The function handales the .string and .data lines*/
int handle_data_type(char *token, int num_of_line, int *error, int number_of_quotation_mark){
    int number_of_data_words;
    if ((strcmp(token, DATA) == 0)){          /*If the line is .data*/
        number_of_data_words = handle_data(token, num_of_line, error);
    }
    else{
        if(number_of_quotation_mark != 2){
            printf("Error, invalid string in line:%d\n",num_of_line);
            *error = TRUE;
        }else{
            number_of_data_words = handle_string(token, num_of_line); /*If the line is .string*/
        }
    }
    if (number_of_data_words == FAILED){
            *error = TRUE;
    }
    return number_of_data_words;
}

/*If the linew is extern line -> push the symbol data to the symbole table*/
void handle_extern_line(char *token){
    token = strtok(NULL, delimit);
    push_symbol_data(token, 0, 0, 0, EXTERNAL_ATTRIBUTE);
}

/*The function returns the number of code words that needs to be created for this command, the command has 2 operands
the command not allow immediate addressing in the target operand */
int handle_commands_with_two_operands_not_allow_immediate_in_target_operand(char *token, int number_of_line, int number_of_commas){
    int number_of_words = 2;
    int method_of_first_operand; 
    int method_of_second_operand;
    int status;
    if(number_of_commas > 1){
        printf("Error, unexpected comma in line: %d\n", number_of_line);
        return FALSE;
    }
    status = get_addressing_modes_of_two_operands(token, number_of_line, &method_of_first_operand, &method_of_second_operand);
    if (status == FAILED){
        return FALSE;
    }
    if (method_of_first_operand == IMMEDIATE_ADDRESSING){
        number_of_words += 1;
    }
    else if (method_of_first_operand == DIRECT_ADDRESSING || method_of_first_operand == INDEX_ADDRESSING){
        number_of_words += 2;
    }
    if (method_of_second_operand == IMMEDIATE_ADDRESSING){  /*If the addressing mode of the second operand is immediate -> print and error*/
        printf("Error, invalid addressing mode in line: %d\n", number_of_line);
        return FALSE;
    }
    else if (method_of_second_operand == DIRECT_ADDRESSING || method_of_second_operand == INDEX_ADDRESSING){
        number_of_words += 2;
    }
    return number_of_words;
}
/*The function returns the number of code words that needs to be created for this command, the command has 1 operands
the command not allow immediate addressing in the target operand */
int handle_commands_with_one_operands_not_allow_immediate_in_target_operand(char *token, int number_of_line, int number_of_commas){
    int method_number;
    if(number_of_commas != 0){
        printf("Error, unexpected comma in line: %d\n", number_of_line);
        return FALSE;
    }
    method_number = get_addressing_mode_of_operand(token, number_of_line);
    if (method_number == FAILED){
        return FALSE;
    }
    if (method_number == DIRECT_ADDRESSING || method_number == INDEX_ADDRESSING){
        return 4;
    }
    if (method_number == REGISTER_DIRECT_ADDRESSING){
        return 2;
    }
    printf("Error, invalid addressing mode in line: %d\n", number_of_line);
    return FALSE;
}
/*The function returns the number of code words that needs to be created for this command, the command has 1 operands
the command not allow immediate addressing and register addressing in the target operand */
int handle_commands_with_one_operands_not_allow_immediate_and_register_in_target_operand(char *token, int number_of_line, int number_of_commas){
    int method_number;
    if(number_of_commas != 0){
        printf("Error, unexpected comma in line: %d\n", number_of_line);
        return FALSE;
    }
    method_number = get_addressing_mode_of_operand(token, number_of_line);
    if (method_number == FAILED){
        return FALSE;
    }
    if (method_number == DIRECT_ADDRESSING || method_number == INDEX_ADDRESSING){
        return 4;
    }
    printf("Error, invalid addressing mode in line: %d\n", number_of_line); /*if the addressing mode is immediate addressing or register addressing*/
    return FALSE;
}
int handle_commands_with_no_operands(char *token, int number_of_line, int number_of_commas){
    if(number_of_commas != 0){
        printf("Error, unexpected comma in line: %d\n", number_of_line);
    }
    if (!(check_if_token_is_empty(token))){
        printf("Error to much operands in line: %d\n", number_of_line);
        return FALSE;
    }
    return 1;
}

/*The function returns the number of words that need to be created for the "move" command with the opernds in the current line*/
int move_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_two_operands_not_allow_immediate_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words;
}
/*The function returns the number of words that need to be created for the "cmp" command with the opernds in the current line
the "cmp" command allows all the addressing modes*/
int cmp_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words = 2;
    int method_of_first_operand;
    int method_of_second_operand;
    int status;
    if(number_of_commas > 1){
        printf("Error, unexpected comma in line: %d\n", number_of_line);
        return FALSE;
    }
    status = get_addressing_modes_of_two_operands(token, number_of_line, &method_of_first_operand, &method_of_second_operand);
    if (status == FAILED){
        return FALSE;
    }
    if (method_of_first_operand == IMMEDIATE_ADDRESSING){
        number_of_words += 1;
    }
    else if (method_of_first_operand == DIRECT_ADDRESSING || method_of_first_operand == INDEX_ADDRESSING){
        number_of_words += 2;
    }
    if (method_of_second_operand == IMMEDIATE_ADDRESSING){
        number_of_words += 1;
    }
    else if (method_of_second_operand == DIRECT_ADDRESSING || method_of_second_operand == INDEX_ADDRESSING){
        number_of_words += 2;
    }
    return number_of_words;
}
/*The function returns the number of words that need to be created for the "add" command with the opernds in the current line*/
int add_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_two_operands_not_allow_immediate_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words;
}

/*The function returns the number of words that need to be created for the "sub" command with the opernds in the current line*/
int sub_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_two_operands_not_allow_immediate_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words;
}

/*The function returns the number of words that need to be created for the "lea" command with the opernds in the current line,
the "lea" command not allow immediate and register addressing in the source operand and immediate addressing in the target operand*/
int lea_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words = 2;
    int method_of_first_operand;
    int method_of_second_operand;
    int status;
    if(number_of_commas > 1){
        printf("Error, unexpected comma in line: %d\n", number_of_line);
        return FALSE;
    }
    status = get_addressing_modes_of_two_operands(token, number_of_line, &method_of_first_operand, &method_of_second_operand);
    if (status == FAILED){
        return FALSE;
    }
    if (method_of_first_operand == IMMEDIATE_ADDRESSING || method_of_first_operand == REGISTER_DIRECT_ADDRESSING || method_of_second_operand == IMMEDIATE_ADDRESSING){
        printf("Error, invalid addressing mode in line: %d\n", number_of_line);
        return FALSE;
    }
    else if (method_of_first_operand == DIRECT_ADDRESSING || method_of_first_operand == INDEX_ADDRESSING){
        number_of_words += 2;
    }
    if (method_of_second_operand == DIRECT_ADDRESSING || method_of_second_operand == INDEX_ADDRESSING){
        number_of_words += 2;
    }
    return number_of_words;
}
/*The function returns the number of words that need to be created for the "not" command with the opernd in the current line,
the command not allow immediate addressing in the target operand*/
int clr_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_one_operands_not_allow_immediate_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words;    
}
/*The function returns the number of words that need to be created for the "not" command with the opernd in the current line,
the command not allow immediate addressing in the target operand*/
int not_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_one_operands_not_allow_immediate_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words;  
}
/*The function returns the number of words that need to be created for the "inc" command with the opernd in the current line,
the command not allow immediate addressing in the target operand*/
int inc_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_one_operands_not_allow_immediate_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words;  
}
/*The function returns the number of words that need to be created for the "dec" command with the opernd in the current line,
the command not allow immediate addressing in the target operand*/
int dec_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_one_operands_not_allow_immediate_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words;  
}
/*The function returns the number of words that need to be created for the "jmp" command with the opernd in the current line,
the command not allow immediate  and register addressing in the target operand*/
int jmp_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_one_operands_not_allow_immediate_and_register_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words;  
}
/*The function returns the number of words that need to be created for the "bne" command with the opernd in the current line,
the command not allow immediate  and register addressing in the target operand*/
int bne_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_one_operands_not_allow_immediate_and_register_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words; 
}
/*The function returns the number of words that need to be created for the "jsr" command with the opernd in the current line,
the command not allow immediate  and register addressing in the target operand*/
int jsr_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_one_operands_not_allow_immediate_and_register_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words; 
}
/*The function returns the number of words that need to be created for the "red" command with the opernd in the current line,
the command not allow immediate addressing in the target operand*/
int red_command(char *token, int number_of_line, int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_one_operands_not_allow_immediate_in_target_operand(token, number_of_line, number_of_commas);
    return number_of_words;  
}
/*The function returns the number of words that need to be created for the "prn" command with the opernd in the current line,
the command allows all the addressing modes in the target operand*/
int prn_command(char *token, int number_of_line, int number_of_commas){
    int method_number;
    if(number_of_commas != 0){
        printf("Error, unexpected comma in line: %d\n", number_of_line);
        return FALSE;
    }
    method_number = get_addressing_mode_of_operand(token, number_of_line);
     if (method_number == FAILED){
        return FALSE;
    }
    if (method_number == IMMEDIATE_ADDRESSING){
        return 3;
    }
    if (method_number == DIRECT_ADDRESSING || method_number == INDEX_ADDRESSING){
        return 4;
    }
    if (method_number == REGISTER_DIRECT_ADDRESSING){
        return 2;
    }
    return FALSE;
}

/*The function returns the number of words that need to be created for the "rts" command in the current line,
the command not allows operands*/
int rts_command(char *token, int number_of_line,int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_no_operands(token, number_of_line, number_of_commas);
    return number_of_words;
}

/*The function returns the number of words that need to be created for the "stop" command in the current line,
the command not allows operands*/
int stop_command(char *token, int number_of_line,int number_of_commas){
    int number_of_words;
    number_of_words = handle_commands_with_no_operands(token, number_of_line, number_of_commas);
    return number_of_words;
}

int first_pass(FILE *fp2 ,int *ICF,int *DCF){
    char line[81] = {'\0'};
    /*step 1 */
    int IC = START_ADDRESS;
    int DC = 0;
    int length_of_data;
    int L;
    int number_of_line = 0;
    int type_of_line;
    int error = 0;
    char *token;
    int lable_exist;
    int value;
    int base;
    int offset;
    int index_of_function;
    int number_of_commas;
    int number_of_quotation_mark;
    typedef int (*printer_t)(char *, int, int);
    printer_t check_the_command_line[NUMBER_OF_FUNCTIONS] = {&move_command, &cmp_command, &add_command, &sub_command, &lea_command, &clr_command, &not_command, &inc_command,
                                                             &dec_command, &jmp_command, &bne_command, &jsr_command, &red_command, &prn_command, &rts_command, &stop_command};
    /* setp 2 */
    while (fgets(line, BUFFERSIZE, fp2) != NULL){
        number_of_line++;
        lable_exist = FALSE;
        type_of_line = get_type_of_line(line, &lable_exist, FIRST_PASS, number_of_line);
        number_of_commas = get_number_of_sign(line, ',');
        number_of_quotation_mark = get_number_of_sign(line, '"');
        if (type_of_line == EMPTY_LINE || type_of_line == COMMENT_LINE){
            continue;
        }
        if (type_of_line == UNKNOWN_GUIDANCE_TYPE || type_of_line == INVALID_LABEL || type_of_line == FAILED){
            error = TRUE;
            continue;
        }
        /* If the first word in the line is a lable */
        if (lable_exist){
            /* catch the lable name */
            token = strtok(line, COLON);
        }
        if (type_of_line == DATA_GUIDANCE_LINE || type_of_line == STRING_GUIDANCE_LINE){
            if (lable_exist){
                /* push the lable data to the lables table */
                value = IC;
                offset = value % 16;
                base = value - offset;
                push_symbol_data(token, value, base, offset, DATA_ATTRIBUTE);
                token = strtok(NULL, delimit);
            }
            else{
                token = strtok(line, delimit);
            }
            /*Step 7 */
            length_of_data = handle_data_type(token, number_of_line, &error, number_of_quotation_mark); /*check the data, insert to the DATA-IMAGE and increase the DC */
            DC += length_of_data;
            IC += length_of_data;
            if(IC > MAX_WORDS){
                printf("Memory overflow !!! \n");
            }
            continue;
        }
        if (type_of_line == ENTRY_GUIDANCE_LINE){
            continue;
        }
        if (type_of_line == EXTERN_GUIDANCE_LINE){
            if (lable_exist){
                token = strtok(NULL, delimit);
                handle_extern_line(token);
            }
            else{
                token = strtok(line, delimit);
                handle_extern_line(token);
            }
            continue;
        }
        /*The line is instruction line */
        if (lable_exist){
            /*push the lable to the lables table with the attribute .code (check if the lable is valid and if the lable already exist) */
            value = IC;
            offset = value % 16;
            base = value - offset;
            push_symbol_data(token, value, base, offset, CODE_ATTRIBUTE);
            /*get the command_name*/
            token = strtok(NULL, delimit);
            if (token == NULL){
                printf("Missing parameter in line: %d \n", number_of_line);
                error = TRUE;
                continue;
            }
        }
        else{
            /*get the command_name*/
            token = strtok(line, delimit);
        }
        /*search the instruction name in the instructions table if not exist print error */
        index_of_function = lookup_cmd(token);
        if (index_of_function == NOT_A_VALID_COMMAND){
            printf("Error, the command: %s is not valid command, in line: %d\n", token, number_of_line);
            error = TRUE;
            continue;
        }

        /*Get the number of words that need to be created for the instruction (call the correct function by the command name) */
        L = check_the_command_line[index_of_function](token, number_of_line, number_of_commas);
        if (L == FALSE){
            error = TRUE;
            continue;
        }
        /* Increase the IC */
        IC = IC + L;
        if(IC > MAX_WORDS){
                printf("Memory overflow !!! \n");
                return FAILED;
        }
        continue;
    }
   /*Was an error in the program -> exit */
    /* Save the final values in the ICD and DCF */
    *DCF = DC;
    *ICF = IC;
    if (error == TRUE){
       return FAILED;
    }
    else
    	return SUCCEEDED;
}
