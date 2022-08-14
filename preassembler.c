#include "head.h"
#include "utils.h"
#include "linked_list_utils.h"


void push_macro_instructions(char macro_name[], char instructions[]) { /* The function gets a macro name and updates its instructions */
	
	macro_tab *mac;
	mac = fill_instructions(heads_list.macros_head, macro_name, instructions);
	if(mac == NULL)
		printf("PROBLEM FILL INSTRUCTIONS: %s\n",macro_name);
}
 
int check_if_macro_exist(char macro_name[]) { /* The function gets a string and checks if it is a macro name that has already been defined */

	macro_tab *mac;
	mac = find_macro(heads_list.macros_head, macro_name);
	
	if (mac != NULL)
		return TRUE;
	return FALSE;
}

void push_macro_name(char macro_name[]){ /* Add a new macro name to the macro table */
	
	macro_tab *mac;
	mac = new_macro(macro_name);
	if(mac != NULL)
		heads_list.macros_head = add_macro(heads_list.macros_head, mac); 
	
}

void pre_assembler(FILE *fp1, FILE *fp2) { /*   The function gets the source file fp1 and an empty file fp2.
						Goes through line after line and executes macros as required.
						The function uses a linked list to implement the "macro table" and performs what is required by a single 							scroll through the source file*/
	char *exp;
	macro_tab *mac;
	char line[BUFFERSIZE] = {END_OF_STRING};
	int macroFlag;
	char name[MAX_NAME_LEN] = {END_OF_STRING};
	while (fgets(line,BUFFERSIZE,fp1) != NULL) {
		
		macroFlag = FALSE;
		exp = del_prim_spaces(line);
		if (check_if_macro_exist(exp)) {
			mac = find_macro(heads_list.macros_head, exp);
			fputs((mac->instructions),fp2);
			macroFlag = TRUE;
		} 
		if ((!strncmp("macro", exp, 5))  && (isspace(exp[5])) && (!macroFlag)) { /* checks if the first field is "macro" */
			macroFlag = TRUE;
			exp = exp + 5;	/* now exp point to the rest of the line after "macro" */	
			exp = del_prim_spaces(exp);
			push_macro_name(exp);
			strcpy(name,exp);

			while (macroFlag){
				fgets(line,BUFFERSIZE,fp1);
				exp = del_prim_spaces(line);

				if(strncmp("endm", exp, 4))
					push_macro_instructions(name, exp);/*  add the macro instructions to the structure that belongs to it */	
				else 
					macroFlag = FALSE;
			}	
		}
		else if (!macroFlag) 
			fputs(exp,fp2);
	 }	
}

