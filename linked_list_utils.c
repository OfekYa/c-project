#include "head.h"
#include "linked_list_utils.h"


macro_tab* add_macro(macro_tab *macros_head, macro_tab *new_mac) { /* Add a macro structure to the macro table */
	new_mac->next = macros_head;
	macros_head = new_mac;
	return macros_head;
}

macro_tab* new_macro(char* macro_name) { /* Create a new macro structure */
	macro_tab *new_mac = (macro_tab*)malloc(sizeof(macro_tab));
	if(new_mac != NULL) {
		strcpy(new_mac->macro_name, macro_name);		
	}
	return new_mac;
}

macro_tab* find_macro(macro_tab *macros_head, char *macro_name) { /* Search for a specific macro in the macro table */

	int mac_len = strlen(macro_name);
	while (macros_head != NULL) {	
		if (!strncmp(macros_head->macro_name, macro_name, mac_len))		
			return macros_head;
		macros_head = macros_head->next;
	}
	return NULL;
}

macro_tab* fill_instructions(macro_tab *macros_head, char *macro_name,	char *instructions) { /* Filling the macro instructions */
	macro_tab *mac;
	mac = find_macro(macros_head, macro_name);
	if (mac != NULL) {
		strcat(mac->instructions, instructions);
		return mac;
	}
	return NULL;	
}


/*##########################################################################################################################*/

symbol_tab* add_symbol(symbol_tab *symbols_head, symbol_tab *new_sym) { /* Add a symbol structure to the symbol table */
	
	new_sym->next = symbols_head;
	symbols_head = new_sym;
	return symbols_head;

}

symbol_tab* new_symbol(char* symbol_name) { /* Create a new symbol structure */
	
	symbol_tab *sym = (symbol_tab*)malloc(sizeof(symbol_tab));
	if(sym != NULL) {
		strcpy(sym->symbol_name, symbol_name);
		sym->value = 0;
		sym->base_address = 0;
		sym->offset = 0;
		sym->attributes[0] = 0;
		sym->attributes[1] = 0;
		sym->next = NULL;	
	}
	return sym;
}

/* Search for a specific symbol in the symbol table */
symbol_tab* find_symbol(symbol_tab *symbols_head, char *symbol_name) { 
	int head_len;
	int sym_len = strlen(symbol_name);
	while (symbols_head != NULL) {	
		head_len = strlen(symbols_head->symbol_name);
		if ((!strncmp(symbols_head->symbol_name, symbol_name, sym_len)) && (head_len == sym_len))		
			return symbols_head;
		symbols_head = symbols_head->next;
	}
	return NULL;
}

/* Filling the symbol data */
symbol_tab* fill_data(symbol_tab *symbols_head, char *symbol_name, int value, int base_address, int offset, int attributes) {
	int index;
	symbol_tab *sym;
	sym = find_symbol(symbols_head, symbol_name);
	if (attributes == DATA_ATTRIBUTE || attributes == CODE_ATTRIBUTE){
		index = 0;
	}
	if (attributes == EXTERNAL_ATTRIBUTE ||  attributes == ENTRY_ATTRIBUTE){
		index = 1;
	}
	if (sym != NULL) {
		sym->value = value;
		sym->base_address = base_address;
		sym->offset = offset;
		sym->attributes[index] = attributes;
		return sym;
	}
	return NULL;
	
}
/* update symbol attribute */
symbol_tab* add_attributes(symbol_tab *symbols_head, char *symbol_name, int attributes) { 
	symbol_tab *sym;
	sym = find_symbol(symbols_head, symbol_name);
	if (sym != NULL) {
		sym->attributes[SECOND_INDEX] = attributes;
		return sym;
	}
	return NULL;
}
/* ######################################################################################################################### */
/*The function creates a new data node and sets the "word" array to an ascii value of zero*/
line_in_data_image create_data_node(){
    line_in_data_image temp; 
    int i;
    temp = (line_in_data_image)malloc(sizeof(struct Line)); 
    temp->next = NULL;
    for(i = 0; i < LENGTH_OF_WORD; i ++){
        temp->word[i] = '0';
    }
    return temp;
}
/*The function adds the new node to the end of the data-image*/
line_in_data_image add_data_node(){
    line_in_data_image temp,p;
    temp = create_data_node();
    if(heads_list.data_image_head == NULL){
        heads_list.data_image_head = temp;     
    }
    else{
        p  = heads_list.data_image_head;
        while(p->next != NULL){
            p = p->next;
        }
        p->next = temp;
    }
    return temp;
}



/*The function creates a new code node and sets the "word" array to an ascii value of zero*/
line_in_code_image create_code_node(){
    line_in_code_image temp; 
    int i;
    temp = (line_in_code_image)malloc(sizeof(struct Line)); 
    temp->next = NULL;
    for(i = 0; i < LENGTH_OF_WORD; i ++){
        temp->word[i] = '0';
    }
    return temp;
}

/*The function adds the new code-node to the end of the code-image and set the "IC" */
line_in_code_image add_code_node(){
    line_in_code_image temp,p;
    temp = create_code_node();
    if(heads_list.code_image_head == NULL){
        heads_list.code_image_head = temp;     
    }
    else{
        p  = heads_list.code_image_head;
        while(p->next != NULL){
            p = p->next;
        }
        p->next = temp;
        temp->IC = p->IC +1;   /*Sets the IC property to the value of the previous node plus 1 */
    }
    return temp;
}
/*######################################################################################################################### */
/*the function free  in the end of the program all the nodes in the tables*/
void free_tables(){
	line_in_code_image tmp1 = heads_list.code_image_head ;
	line_in_data_image tmp2 = heads_list.data_image_head;
	symbol_tab *tmp3 = heads_list.symbols_head;
	macro_tab *tmp4 = heads_list.macros_head;

   	while (heads_list.code_image_head != NULL){
       tmp1= heads_list.code_image_head ;
       heads_list.code_image_head  = heads_list.code_image_head->next;
       free(tmp1);
    }
	while (heads_list.data_image_head != NULL){
       tmp2= heads_list.data_image_head;
       heads_list.data_image_head = heads_list.data_image_head->next;
       free(tmp2);
    }
	while (heads_list.symbols_head != NULL){
       tmp3 = heads_list.symbols_head;
       heads_list.symbols_head = heads_list.symbols_head->next;
       free(tmp3);
    }
	while (heads_list.macros_head != NULL){
       tmp4 = heads_list.macros_head;
       heads_list.macros_head = heads_list.macros_head->next;
       free(tmp4);
    }

}
