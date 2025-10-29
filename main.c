#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
/*************************************数字栈************************************************************/
/*创建数字栈*/
NumsStack* Creat_NumsStack(uint16_t capacity) {
	NumsStack* stack = (NumsStack*)malloc(sizeof(NumsStack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->nums = (uint16_t*)malloc(sizeof(uint16_t) * capacity);
	return stack;
}
/*数字栈出栈*/
uint16_t NumsStack_Pop(NumsStack* stack) {
	if (!stack || stack->top == -1) {
		return 0; //栈空
	}
	return stack->nums[stack->top--];
}
/*数字栈入栈*/
void NumsStack_Push(NumsStack* stack, uint16_t num) {
	if (!stack || stack->top == stack->capacity - 1) {
		return; //栈满
	}
	stack->nums[++stack->top] = num;
}
/*数字栈销毁*/
void freeNumStack(NumsStack* stack) {
	if (!stack) return;
	free(stack->nums);
	free(stack);
}



/****************************************** 字符串栈 ******************************************************/
/*创建字符串栈*/
StrStack* Creat_StrStack(uint16_t capacity) {
	StrStack* stack = (StrStack*)malloc(sizeof(StrStack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->strs = (char**)malloc(sizeof(char*) * capacity);
	return stack;
}

/*字符串栈出栈*/
char* StrStack_Pop(StrStack* stack) {
	if (!stack || stack->top == -1) {
		return 0; //栈空
	}
	return stack->strs[stack->top--];
}
/*字符串栈入栈*/
void StrStack_Push(StrStack* stack, char* value) {
	if (!stack || stack->top == stack->capacity - 1) {
		return; //栈满
	}
	stack->strs[++stack->top] = _strdup(value);
}
/*字符串栈销毁*/
void freeStrStack(StrStack* stack) {
	if (!stack) return;
	for (uint16_t i = 0; i <= stack->top; i++) {
		free(stack->strs[i]);
	}
	free(stack->strs);
	free(stack);
}
/***************************************************字符串解码*********************************/

char* decode_string(const char* s) {
	
	size_t len = strlen(s);
	if (len == 0) {
		return (char*)calloc(1, sizeof(char)); // 处理空输入
	}

	NumsStack* ns = Creat_NumsStack((uint16_t)len);
	StrStack* ss = Creat_StrStack((uint16_t)len);

	// 检查栈是否创建成功
	if (!ns || !ss) {
		freeNumStack(ns);
		freeStrStack(ss);
		return NULL; // 创建失败，返回NULL
	}

	char* current_str = (char*)calloc(1, sizeof(char));
	if (!current_str) { // 检查 calloc 是否成功
		freeNumStack(ns);
		freeStrStack(ss);
		return NULL;
	}

	int current_num = 0;

	for (size_t i = 0; i < len; i++) {
		if (isdigit(s[i])) {
			current_num = current_num * 10 + (s[i] - '0');
		}
		else if (s[i] == '[') {
			NumsStack_Push(ns, current_num);
			StrStack_Push(ss, current_str);

			current_num = 0;
			free(current_str);
			current_str = (char*)calloc(1, sizeof(char));
			if (!current_str) { // 每次分配后都检查
				// 错误处理：需要释放所有已分配的资源
				freeNumStack(ns);
				freeStrStack(ss);
				return NULL;
			}
		}
		else if (s[i] == ']') {


			uint16_t num = NumsStack_Pop(ns);
			char* str = StrStack_Pop(ss);
			
			if (!str) { 
				free(current_str); freeNumStack(ns); freeStrStack(ss); return NULL; 
			}

			// 使用 size_t 进行长度计算
			size_t current_str_len = strlen(current_str);
			size_t prev_str_len = strlen(str);

			// 计算拼接后的新总长度
			size_t new_len = prev_str_len + current_str_len * num;
			char* new_current_str = (char*)malloc(new_len + 1);

			if (!new_current_str) { // 检查 malloc
				// 错误处理
				free(current_str);
				free(str);
				freeNumStack(ns);
				freeStrStack(ss);
				return NULL;
			}

			// 先复制上一层的字符串
			memcpy(new_current_str, str, prev_str_len);

			// 循环拼接重复的字符串部分
			for (int j = 0; j < num; ++j) {
				memcpy(new_current_str + prev_str_len + j * current_str_len, current_str, current_str_len);
			}
			new_current_str[new_len] = '\0'; // 手动添加字符串结束符

			// 释放不再需要的内存
			free(str);
			free(current_str);

			current_str = new_current_str;
		}
		else { // 字母
			size_t current_len = strlen(current_str);

			// 使用临时指针 realloc 防止后续空间不够返回NULL
			char* temp = (char*)realloc(current_str, current_len + 2);
			if (!temp) { // 检查 realloc
				// 错误处理
				free(current_str);
				freeNumStack(ns);
				freeStrStack(ss);
				return NULL;
			}
			current_str = temp; // 仅在成功时更新指针

			current_str[current_len] = s[i];
			current_str[current_len + 1] = '\0';
		}
	}

	freeNumStack(ns);
	freeStrStack(ss);

	return current_str;
}

int main(void) {

	char str[1024];
	int i=scanf("%s", str);
	printf("%s\n", decode_string(str));
	return 0;
}
