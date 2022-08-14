
typedef struct macro {
	char macro_name[MAX_NAME_LEN];
	char instructions[480];
	struct macro *next;    
}macro_tab;

macro_tab* add_macro(macro_tab *macros_head, macro_tab *new_mac);
macro_tab* new_macro(char* macro_name);
macro_tab* find_macro(macro_tab *macros_head, char *macro_name);
macro_tab* fill_instructions(macro_tab *macros_head, char *macro_name,	char *instructions);



typedef struct symbol {
	char symbol_name[MAX_NAME_LEN];
	int value;
	int base_address;
	int offset;
	int attributes[2];
	struct symbol *next;
}symbol_tab;

symbol_tab* add_symbol(symbol_tab *symbols_head, symbol_tab *new_sym);
symbol_tab* new_symbol(char* symbol_name);
symbol_tab* find_symbol(symbol_tab *symbols_head, char *symbol_name);
symbol_tab* fill_data(symbol_tab *symbols_head, char *symbol_name, int value, int base_address, int offset, int attributes);
symbol_tab* add_attributes(symbol_tab *symbols_head, char *symbol_name, int attributes);



struct Line{
    char word[LENGTH_OF_WORD];
	int IC;
    struct Line *next;
 };

typedef struct Line *line_in_data_image;
typedef struct Line *line_in_code_image;

line_in_data_image create_data_node();
line_in_data_image add_data_node();
line_in_code_image create_code_node();
line_in_code_image add_code_node();
line_in_code_image get_new_code_node();



void free_tables();

struct heads{
	line_in_code_image code_image_head;
	line_in_data_image data_image_head;
	symbol_tab *symbols_head;
	macro_tab *macros_head;
}heads_list;
