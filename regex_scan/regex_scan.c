#include "regex_scan.h"
#include "stdio.h"
#include "stdlib.h"

//assumes ch is a length of 3 with 1 more for the null terminator
void alphabet_encode(char* ch, char enc)
{
	if(enc >= '!' && enc <= '_')
	{
		ch[0] = enc;
		ch[1] = 0;
	}else if(enc >= 'a' && enc <= 'w')
	{
		ch[0] = enc;
		ch[1] = 0;
	}else if(enc >= 'y' && enc <= '~')
	{
		ch[0] = enc;
		ch[1] = 0;
	}else
	{
		snprintf(ch, 4, "x%2X", enc);
	}
}


/**
 * saves a pointer to an array of Token into out on sucess
 * returns the size of the array on sucess
 * returns -1 on scanning error and frees the memory it usees in this case
 * on sucess the caller must free the saved pointer
 * the pointer passed into out will be overwritten
**/
int regex_scan(char* str, int size, struct Token** out)
{
	int i = 0;
	struct Token* oret = malloc(sizeof(struct Token));
	int osize = 0;
	int arrptr = 0;
	
	while(i < size)
	{
		struct Token* temp = realloc(oret, ++osize * sizeof(struct Token));
		if(temp == NULL)
		{
			free(oret);
			return -1;
		}else{oret = temp;}
		
		switch(str[i])
		{
			case '.': //DOT
				oret[arrptr].id = DOT;
				alphabet_encode(oret[arrptr].ch, '.');
				break;
			
			case '-': //DASH
				oret[arrptr].id = DASH;
				alphabet_encode(oret[arrptr].ch, '-');
				break;
				
			case '*': //KLEENE
				oret[arrptr].id = KLEENE;
				alphabet_encode(oret[arrptr].ch, '*');
				break;
				
			case '+': //PLUS
				oret[arrptr].id = PLUS;
				alphabet_encode(oret[arrptr].ch, '+');
				break;
				
			case '|': //PIPE
				oret[arrptr].id = PIPE;
				alphabet_encode(oret[arrptr].ch, '|');
				break;
			
			case '(': //OPEN
				oret[arrptr].id = OPEN;
				alphabet_encode(oret[arrptr].ch, '(');
				break;
				
			case ')': //CLOSE
				oret[arrptr].id = CLOSE;
				alphabet_encode(oret[arrptr].ch, ')');
				break;
			
			case '\\': //ESCAPE
				i += 1;
				if(i >= size)
				{
					free(oret);
					return -1;
				}
				switch(str[i])
				{
					case '.':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, '.');
						break;

					case '-':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, '-');
						break;

					case '*':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, '*');
						break;

					case '+':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, '+');
						break;

					case '|':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, '|');
						break;

					case '(':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, '(');
						break;
					
					case ')':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, ')');
						break;
					
					case '\\':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, '\\');
						break;
						
					case 's':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, ' ');
						break;

					case 'n':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, '\n');
						break;

					case 't':
						oret[arrptr].id = CHAR;
						alphabet_encode(oret[arrptr].ch, '\t');
						break;
						
					default:
						free(oret);
						return -1;
				}
				break;
			
			default: //CHAR
				oret[arrptr].id = CHAR;
				alphabet_encode(oret[arrptr].ch, str[i]);
		}
		i += 1;
		arrptr += 1;
	}
	
	*out = oret;
	return osize;
}
