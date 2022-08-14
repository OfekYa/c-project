#include "head.h"


/*The function covert the string to an int*/
int string_to_int(char *c, int num_of_line, int *error){
    char p = *c;
    int ergebnis = 0;
    int sign = 1;
    if ('-' == *c || '+' == *c){
        if ('-' == *c){
            sign = -1;
        }
        c++;
    }
    while (*c){
        p = *c - '0';
        if (0 <= p && 9 >= p){
            ergebnis = ergebnis * 10 + p;
            c++;
        }
        else{
            printf("Error, not a valid number in .data, in line %d \n", num_of_line);
            *error = TRUE;
            return FAILED;
        }
    }
    return ergebnis * sign;
}


char *del_prim_spaces(char *line) { /* delete the white characters that come before the text */
	int i = 0;
	int numOfSpaces;

	while ((line[i] == SPACE_CHAR) || (line[i] == TAB)) {
		i++;
	}
	numOfSpaces = i;
	return line + numOfSpaces;
}

void del_all_spaces(char *line) { /* delete all the white characters from the line */
	int count = 0, i = 0;
	for (i = 0; line[i]; i++) {
        	if ((line[i] != SPACE_CHAR) && (line[i] != TAB) && (line[i] != NEW_LINE)) /* rebuild the expression while deleting spaces, etc. */
			line[count++] = line[i]; /* count is incremented */
	}
    
	line[count] = END_OF_STRING; /* end of the string */
}

int check_if_white_space(char character) {
	if ((character == SPACE_CHAR) || (character == TAB) || (character == NEW_LINE) )  {
		return TRUE;
	}
	return FALSE;
}

/*The function removes whitespaces from the array*/
void remove_white_spaces(char *str){
    int i = 0, j = 0;
    while (str[i]){
        if (!check_if_white_space(str[i])){
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = END_OF_STRING;
}

/*The function remove the white spaces in the beginning and end of the string*/
void del_edge_spaces(char *str){
	int i = strlen(str)-1;
    int j = 0;
    int z =0;
	while (check_if_white_space(str[i])) {
		str[i] = END_OF_STRING;
		i--;
	}
    while(check_if_white_space(str[j])){
        j++;
    }
    while(str[j]){
        str[z] = str[j];
        z++;
        j++;
    }
    str[z] = END_OF_STRING;
}

void take_first_word(char *line) { /* Modifies the line so that it contains only the first word.
				      Also used to pull out the label name. */
	int count = 0, i = 0, k;

	while ((line[i] != SPACE_CHAR) && (line[i] != TAB) && (line[i] != ':') && (line[i] != NEW_LINE)){  
		line[count++] = line[i];
		i++;
	}
	for(k = i; line[k]; k++) 
		line[k] = END_OF_STRING;

}


