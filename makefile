runfile: firstpass.c utils.c assem.c preassembler.c  secondpass.c linked_list_utils.c passes_utils.c createfiles.c createfiles.h passes_utils.h head.h linked_list_utils.h utils.h commands.h
	gcc -g -Wall -ansi -pedantic preassembler.c firstpass.c utils.c assem.c secondpass.c linked_list_utils.c passes_utils.c createfiles.c -o assem -lm
clean: rm *~