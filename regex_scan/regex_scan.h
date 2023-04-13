#ifndef REGEX_SCAN_H
#define  REGEX_SCAN_H

enum TokenID {
	CHAR, DOT, DASH, KLEENE, PLUS, PIPE, OPEN, CLOSE
};

struct Token {
	enum TokenID id;
	char ch[4];
};

/**
 * saves a pointer to an array of Token into out on sucess
 * returns the size of the array on sucess
 * returns -1 on scanning error and frees the memory it usees in this case
 * on sucess the caller must free the saved pointer
 * the pointer passed into out will be overwritten
**/
int regex_scan(char* str, int size, struct Token** out);

#endif //REGEX_SCAN_H
