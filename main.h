
#pragma once
#include <stdint.h>
#define MAX_LEN 256
/*数字栈*/
typedef  struct {
	uint16_t* nums;
	int  top;
	uint16_t capacity;
}NumsStack;
/*字符串栈*/
typedef struct {
	 char** strs;
	int  top;
	uint16_t capacity;
}StrStack;
