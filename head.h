#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

enum addressing_modes {
	
	IMMEDIATE_ADDRESSING,
	DIRECT_ADDRESSING,
	INDEX_ADDRESSING,
	REGISTER_DIRECT_ADDRESSING
};


enum line_type {

	EMPTY_LINE,
	COMMENT_LINE,
	INSTRUCTION_LINE,
	DATA_GUIDANCE_LINE,
	STRING_GUIDANCE_LINE,
	ENTRY_GUIDANCE_LINE,
	EXTERN_GUIDANCE_LINE,
	UNKNOWN_GUIDANCE_TYPE,
	INVALID_LABEL
};

enum file_type {
	AM,
	OB,
	ENT,
	EXT
};

#define DOT "."
#define AS_FILE_EXTENSION ".as"
#define START_ADDRESS 100
#define BUFFERSIZE 81
#define MAX_NAME_LEN 31
#define stop "stop"
#define SPACE " "
#define COMMA ","
#define COLON ":"
#define QUOTATION_MARK '"'
#define COMMAND_SUCCEED 1
#define COMMAND_NOT_SUCCEED 0
#define NUMBER_OF_FUNCTIONS 16
#define NEW_LINE '\n'
#define END_OF_STRING '\0'
#define FALSE 0
#define TRUE 1
#define TAB '\t'
#define FLEG_ON 1
#define DATA_ATTRIBUTE 1
#define CODE_ATTRIBUTE 2
#define ENTRY_ATTRIBUTE 3
#define EXTERNAL_ATTRIBUTE 4
#define FIRST_INDEX 0
#define SECOND_INDEX 1
#define DATA ".data"
#define STRING ".string"
#define EXTERN ".extern"
#define ENTRY ".entry"
#define SPACE_CHAR ' '
#define NOT_A_VALID_COMMAND -1
#define INVALID_LABEL_NAME -1
#define FAILED -1
#define SUCCEEDED 1
#define FIRST_PASS 1
#define SECOND_PASS 2
#define INDEX_OF_A 18
#define INDEX_OF_R 17
#define INDEX_OF_E 16
#define LENGTH_OF_WORD 20
#define TURN_ON '1'
#define FIRST_OPERND 1
#define SECOND_OPERND 2
#define delimit " \t\r\n\v\f"
#define MAX_WORDS 8291




