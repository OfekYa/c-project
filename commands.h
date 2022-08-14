struct command {
    char name[5];
	int opcode;
    int funct;
};

/* {"command_name", "opcode", "funct"} */
static struct command commands[16] = {
	{"mov", 0, 0},
	{"cmp", 1, 0},
	{"add", 2, 10},
	{"sub", 2, 11},
	{"lea", 4, 0},
	{"clr", 5, 10},
	{"not", 5, 11},
	{"inc", 5, 12},
	{"dec", 5, 13},
	{"jmp", 9, 10},
	{"bne", 9, 11},
	{"jsr", 9, 12},
	{"red", 12, 0},
	{"prn", 13, 0},
	{"rts", 14, 0},
	{"stop", 15, 0}
};

