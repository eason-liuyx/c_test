#include <ctype.h>
#include <string.h>

#define STACKLEN 46000
static int top;

static char stack[STACKLEN];
static char substr[STACKLEN];

static void Push(char val)
{
	stack[++top] = val;
}

static char Pop()
{
	if (top >= 0)
		return stack[top--];

	return '\0';
}

void proc_dig(char ch)
{
	int time;
	char tmp;
	int i = 0;

	time = ch - '0';

	if (time == 1)
		time = 10;
	else if (time < 2 || time > 10)
		return;

	tmp = Pop();
	while (tmp != '(') {
		substr[i] = tmp;
		i++;
		tmp = Pop();
	}

	for (int j = 0; j < time; j++) {
		for (int k = 0; k < i; k++) {
			Push(substr[k]);
		}
	}

	for (int k = 0; k < time * i; k++) {
		substr[k] = Pop();
	}

	for (int k = 0; k < time * i; k++) {
		Push(substr[k]);
	}
}

char* unzip(char *records)
{
	int len = strlen(records);
	char ch;

	top = -1;
	for (int i = 0; i < len; i++) {
		ch = records[i];

		if (isdigit(ch)) {
			proc_dig(ch);
		}

		if (isalpha(ch))
			Push(ch);

		if (ch == '(') {
			Push(ch);
		}
	}
	stack[++top] = '\0';

	return stack;
}

