
#include "head.h"
#include "passes_utils.h"
#include "utils.h"
#include "linked_list_utils.h"
#include "commands.h"

char *get_lable_name(char *operand){ /* For index addressing pop the label name:	label[r4] ---> return label */
    int length = 32;
    char *label_name = malloc(length);
    int j = 0;
    memset(label_name, 0, length);
    while (operand[j] != '[')
    {
        label_name[j] = operand[j];
        j++;
    }
    return label_name;
}
char *get_register_name(char *operand){ /* For index addressing pop the register name:     label[r4] ---> return r4*/
    int length = 3;
    char *register_value = malloc(length);
    int j = 0, i = 0;
    memset(register_value, 0, length);
    while (operand[j] != '['){
        j++;
    }
    j++;
    while (operand[j] != ']'){
        register_value[i] = operand[j];
        j++;
        i++;
    }
    return register_value;
}

void push_bit_to_code_word(int index, line_in_code_image new_node){
    new_node->word[index] = TURN_ON;
}

void push_by_index(int dec, line_in_code_image new_node, int start, int end){ /* Writing the values ​​for the requested bits in the word */
    int index, j, x;
    for (j = 0, index = start; index <= end; j++, index++){
        x = dec >> j;
        if (x & 1){
            push_bit_to_code_word(index, new_node);
        }
    }
}

void push_decimal_to_code_word_in_two_complement(int dec, line_in_code_image new_node){
    push_by_index(dec, new_node, 0, 15);
}

/* 0-1 bits contain the number of the addressing mode of the target operand */
void push_bits_to_target_addressing(int addressing_mode, line_in_code_image new_node){
    push_by_index(addressing_mode, new_node, 0, 1);
}
/* Bits 2-5 contain the number of the target register if the target operand is a register */
void push_bits_to_target_register(char *token, line_in_code_image new_node){
    int reg;
    reg = atoi(token + 1);
    push_by_index(reg, new_node, 2, 5);
}
/* Bits 6-7 contain the number of the addressing mode of the source operand */
void push_bits_to_source_addressing(int addressing_mode, line_in_code_image new_node)
{
    push_by_index(addressing_mode, new_node, 6, 7);
}
/* Bits 8-11 contain the number of the source register if the source operand is a register */
void push_bits_to_source_register(char *token, line_in_code_image new_node){
    int reg;
    reg = atoi(token + 1);
    push_by_index(reg, new_node, 8, 11);
}
/* Bits 12-15 contain the funct field */
void push_funct_to_word(int funct_of_command, line_in_code_image new_node){
    push_by_index(funct_of_command, new_node, 12, 15);
}

/* Find the base address value of the symbol from the symbol table */
int get_base_by_lable_name(char *label_name){
    symbol_tab *sym = heads_list.symbols_head;
    int base;
    int label_len = strlen(label_name);

    while (sym != NULL){
        if (!strncmp(sym->symbol_name, label_name, label_len)){
            base = sym->base_address;
            return base;
        }
        sym = sym->next;
    }
    return FAILED;
}
/* Find the offset value of the symbol from the symbol table */
int get_offset_by_lable_name(char *label_name){
    symbol_tab *sym = heads_list.symbols_head;
    int offset;
    int label_len = strlen(label_name);

    while (sym != NULL){
        if (!strncmp(sym->symbol_name, label_name, label_len)){
            offset = sym->offset;
            return offset;
        }
        sym = sym->next;
    }
    return FAILED;
}
/* Checks if the symbol attribute contains external attribute */
int is_external_lable(char *label_name){
    symbol_tab *sym = heads_list.symbols_head;
    int label_len = strlen(label_name);

    while (sym != NULL){
        if (!strncmp(sym->symbol_name, label_name, label_len)){
            if (sym->attributes[SECOND_INDEX] == EXTERNAL_ATTRIBUTE){
                return TRUE;
            }
            return FALSE;
        }
        sym = sym->next;
    }
    return FAILED;
}

/*The function creates and sets the first work of the command in the code-image*/
void handle_first_word(int number_of_line, int index_of_command)
{
    line_in_code_image new_node;
    int opcode_of_command = commands[index_of_command].opcode;
    new_node = add_code_node(heads_list.code_image_head);
    push_bit_to_code_word(INDEX_OF_A, new_node);
    push_bit_to_code_word(opcode_of_command, new_node);
}
/*The function crease and sets the second word of the command in the code-image*/
line_in_code_image handle_second_word(char *token, int number_of_line, int index_of_command)
{
    line_in_code_image new_node;
    int funct_of_command = commands[index_of_command].funct; /*Get the funct of the command*/
    new_node = add_code_node(heads_list.code_image_head);    /*Add new code-node*/
    push_bit_to_code_word(INDEX_OF_A, new_node);
    push_funct_to_word(funct_of_command, new_node);
    return new_node;
}
/*the function creates and sets 2 new node in the code_node for the lable operand*/
void handle_lable(char *token)
{
    line_in_code_image first_node;
    line_in_code_image second_node;
    int base;
    int offset;
    first_node = add_code_node(heads_list.code_image_head);
    second_node = add_code_node(heads_list.code_image_head);
    if (is_external_lable(token)){
        push_bit_to_code_word(INDEX_OF_E, first_node);
        push_bit_to_code_word(INDEX_OF_E, second_node);
        base = first_node->IC + START_ADDRESS;                        /*calculate the acual base value*/
        offset = second_node->IC + START_ADDRESS;                     /*calculate the acual base value*/
        push_symbol_data(token, 0, base, offset, EXTERNAL_ATTRIBUTE); /*Update the base and offset for the lable with external attribute */
    }
    else
    {
        base = get_base_by_lable_name(token);
        push_decimal_to_code_word_in_two_complement(base, first_node);
        push_bit_to_code_word(INDEX_OF_R, first_node);
        offset = get_offset_by_lable_name(token);
        push_decimal_to_code_word_in_two_complement(offset, second_node);
        push_bit_to_code_word(INDEX_OF_R, second_node);
    }
}

/*The function sets the source/target addresing to immediate-addressing(00), creates new code-word 
and push the binary value of the number to the word*/
void handle_immediate_addressing(char *token, int number_of_line, line_in_code_image second_word, int opernd_location, int *error)
{
    line_in_code_image new_node;
    int number;
    if (opernd_location == FIRST_OPERND)
    {
        push_bits_to_source_addressing(IMMEDIATE_ADDRESSING, second_word);
    }
    else
    {
        push_bits_to_target_addressing(IMMEDIATE_ADDRESSING, second_word);
    }
    new_node = add_code_node(heads_list.code_image_head); /*Add new word to the code-image*/
    token++;
    number = string_to_int(token, number_of_line, error);          /*Gets the value of the number*/
    push_decimal_to_code_word_in_two_complement(number, new_node); /*Push the value to the new word*/
    push_bit_to_code_word(INDEX_OF_A, new_node);
}

void handle_direct_addressing(char *token, line_in_code_image second_word, int opernd_location){
    if (opernd_location == FIRST_OPERND){
        push_bits_to_source_addressing(DIRECT_ADDRESSING, second_word);
    }
    else{
        push_bits_to_target_addressing(DIRECT_ADDRESSING, second_word);
    }
    handle_lable(token);
}

/**/
void handle_index_addressing(char *token, line_in_code_image second_word, int opernd_location)
{
    char *lable_name;
    char *register_name;
    lable_name = get_lable_name(token);
    register_name = get_register_name(token);
    if (opernd_location == FIRST_OPERND){
        push_bits_to_source_register(register_name, second_word);
        push_bits_to_source_addressing(INDEX_ADDRESSING, second_word);
    }
    else{  /*Second operand*/
        push_bits_to_target_register(register_name, second_word);
        push_bits_to_target_addressing(INDEX_ADDRESSING, second_word);
    }
    handle_lable(lable_name);
}

void handle_register_mode(char *token, line_in_code_image second_word, int opernd_location){
    if (opernd_location == FIRST_OPERND){
        push_bits_to_source_register(token, second_word);
        push_bits_to_source_addressing(REGISTER_DIRECT_ADDRESSING, second_word);
    }
    else{
        push_bits_to_target_register(token, second_word);
        push_bits_to_target_addressing(REGISTER_DIRECT_ADDRESSING, second_word);
    }
}

/**/
int handle_commands_with_one_operand(char *token, int number_of_line, int index_of_command){
    int error;
    int addressing_mode;
    line_in_code_image second_word;
    handle_first_word(number_of_line, index_of_command);
    /*catch opernd */
    token = strtok(NULL, delimit);
    remove_white_spaces(token);
    addressing_mode = get_addressing_modes(token, SECOND_PASS, number_of_line);
    if (addressing_mode == FAILED){
        return FAILED;
    }
    second_word = handle_second_word(token, number_of_line, index_of_command);

    if (addressing_mode == REGISTER_DIRECT_ADDRESSING){
        handle_register_mode(token, second_word, SECOND_OPERND);
    }

    if (addressing_mode == IMMEDIATE_ADDRESSING){
        handle_immediate_addressing(token, number_of_line, second_word, SECOND_OPERND, &error);
        if (error == TRUE){
            printf("Error, not a valid number in line: %d\n", number_of_line);
            return FAILED;
        }
    }
    else if (addressing_mode == DIRECT_ADDRESSING){
        handle_direct_addressing(token, second_word, SECOND_OPERND);
    }
    else if (addressing_mode == INDEX_ADDRESSING){
        handle_index_addressing(token, second_word, SECOND_OPERND);
    }

    return TRUE;
}

/**/
int handle_commands_with_two_operands(char *token, int number_of_line, int index_of_command){
    int error;
    int addressing_mode_of_first_opperand;
    int addressing_mode_of_second_opperand;
    line_in_code_image second_word;
    handle_first_word(number_of_line, index_of_command);
    /*catch first opernd */
    token = strtok(NULL, COMMA);
    remove_white_spaces(token);
    addressing_mode_of_first_opperand = get_addressing_modes(token, SECOND_PASS, number_of_line);
    if (addressing_mode_of_first_opperand == FAILED){
        return FAILED;
    }
    second_word = handle_second_word(token, number_of_line, index_of_command);

    if (addressing_mode_of_first_opperand == REGISTER_DIRECT_ADDRESSING){
        handle_register_mode(token, second_word, FIRST_OPERND);
    }
    if (addressing_mode_of_first_opperand == IMMEDIATE_ADDRESSING){
        handle_immediate_addressing(token, number_of_line, second_word, FIRST_OPERND, &error);
        if (error == TRUE){
            printf("Error, not a valid number in line: %d\n", number_of_line);
            return FAILED;
        }
    }
    else if (addressing_mode_of_first_opperand == DIRECT_ADDRESSING){
        handle_direct_addressing(token, second_word, FIRST_OPERND);
    }
    else if (addressing_mode_of_first_opperand == INDEX_ADDRESSING){
        handle_index_addressing(token, second_word, FIRST_OPERND);
    }
    /*catch second opernd */
    token = strtok(NULL, COMMA);
    remove_white_spaces(token);
    addressing_mode_of_second_opperand = get_addressing_modes(token, SECOND_PASS, number_of_line);
    if (addressing_mode_of_second_opperand == FAILED){
        return FAILED;
    }
    if (addressing_mode_of_second_opperand == REGISTER_DIRECT_ADDRESSING){
        handle_register_mode(token, second_word, SECOND_OPERND);
    }
    if (addressing_mode_of_second_opperand == IMMEDIATE_ADDRESSING){
        handle_immediate_addressing(token, number_of_line, second_word, SECOND_OPERND, &error);
        if (error == TRUE){
            printf("Error, not a valid number in line: %d\n", number_of_line);
            return FAILED;
        }
    }
    else if (addressing_mode_of_second_opperand == DIRECT_ADDRESSING){
        handle_direct_addressing(token, second_word, SECOND_OPERND);
    }
    else if (addressing_mode_of_second_opperand == INDEX_ADDRESSING){
        handle_index_addressing(token, second_word, SECOND_OPERND);
    }
    return TRUE;
}

int handle_move_command(char *token, int number_of_line, int index_of_command){
    int status;
    status = handle_commands_with_two_operands(token, number_of_line, index_of_command);
    return status;
}

int handle_cmp_command(char *token, int number_of_line, int index_of_command){
    int status;
    status = handle_commands_with_two_operands(token, number_of_line, index_of_command);
    return status;
}

int handle_add_command(char *token, int number_of_line, int index_of_command){
    int status;
    status = handle_commands_with_two_operands(token, number_of_line, index_of_command);
    return status;
}
int handle_sub_command(char *token, int number_of_line, int index_of_command){
    int status;
    status = handle_commands_with_two_operands(token, number_of_line, index_of_command);
    return status;
}

int handle_lea_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_two_operands(token, number_of_line, index_of_command);
    return status;
}

int handle_clr_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_one_operand(token, number_of_line, index_of_command);
    return status;
}
int handle_not_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_one_operand(token, number_of_line, index_of_command);
    return status;
}

int handle_inc_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_one_operand(token, number_of_line, index_of_command);
    return status;
}

int handle_dec_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_one_operand(token, number_of_line, index_of_command);
    return status;
}

int handle_jmp_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_one_operand(token, number_of_line, index_of_command);
    return status;
}
int handle_bne_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_one_operand(token, number_of_line, index_of_command);
    return status;
}
int handle_jsr_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_one_operand(token, number_of_line, index_of_command);
    return status;
}
int handle_red_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_one_operand(token, number_of_line, index_of_command);
    return status;
}
int handle_prn_command(char *token, int number_of_line, int index_of_command)
{
    int status;
    status = handle_commands_with_one_operand(token, number_of_line, index_of_command);
    return status;
}

int handle_rts_command(char *token, int number_of_line, int index_of_command)
{
    handle_first_word(number_of_line, index_of_command);
    return TRUE;
}

int handle_stop_command(char *token, int number_of_line, int index_of_command)
{
    handle_first_word(number_of_line, index_of_command);
    return TRUE;
}

int second_pass(FILE *fp2){
    char line[81] = {'\0'};
    int number_of_line = 0;
    int type_of_line;
    int error = 0;
    char *token;
    int lable_exist;
    int status;
    int index_of_function;
    typedef int (*printer_t)(char *, int, int);
    printer_t check_the_command_line[NUMBER_OF_FUNCTIONS] = {&handle_move_command, &handle_cmp_command, &handle_add_command, &handle_sub_command, &handle_lea_command, &handle_clr_command, &handle_not_command, &handle_inc_command, &handle_dec_command, &handle_jmp_command, &handle_bne_command, &handle_jsr_command, &handle_red_command, &handle_prn_command, &handle_rts_command, &handle_stop_command};

    while (fgets(line, 81, fp2) != NULL){
        number_of_line++;
        lable_exist = 0;
        type_of_line = get_type_of_line(line, &lable_exist, SECOND_PASS, number_of_line);
        if (type_of_line == EMPTY_LINE || type_of_line == COMMENT_LINE || type_of_line == STRING_GUIDANCE_LINE ||
            type_of_line == DATA_GUIDANCE_LINE || type_of_line == EXTERN_GUIDANCE_LINE || type_of_line == ENTRY_GUIDANCE_LINE){
            continue;
        }
        if (type_of_line == INVALID_LABEL){
            error = 1;
            continue;
        }
        /*The first word in the line is a lable */
        if (lable_exist){
            /* catch the lable name */
            token = strtok(line, delimit);
        }
        /* the line is instruction line */
        if (lable_exist){
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

        /*Call the right function by the command name*/
        status = check_the_command_line[index_of_function](token, number_of_line, index_of_function);
        if (status == FAILED){
            error = TRUE;
            continue;
        }
    }
    if (error == TRUE){
        return FAILED;
    }
    else
        return SUCCEEDED;
}
