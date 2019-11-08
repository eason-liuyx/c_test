#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WD_NUM	64
#define MAX_WD_LEN	16
#define MAX_STR_LEN	512

int strlabel(){
	char words[MAX_WD_NUM][MAX_WD_LEN];
	char input[MAX_STR_LEN];
	char *p;
	int bitmap[MAX_STR_LEN + 2] = {0};
	int pos = 0;
	int wd_num;
	int len;

	/* input word count */
	scanf("%d", &wd_num);
	if (wd_num < 1 || wd_num > MAX_WD_NUM) {
		printf("Invalid word num: %d\n", wd_num);
		goto out;
	}

	/* input words */
	for (int i = 0; i < wd_num; i++) {
		scanf("%s", input);
		len = strlen(input);
		if (len < 1 || len > MAX_WD_LEN) {
			printf("Invalid word len:%d\n", len);
			goto out;
		}
		sprintf(words[i], "%s", input);
	}

	/*input string */
	scanf("%s", input);
	len = strlen(input);
	if (len < 1 || len > MAX_STR_LEN) {
		printf("Invalid string len:%d\n", len);
		goto out;
	}

	/* match word in string */
	for (int i = 0; i < wd_num; i++) {
		do {
			p = strstr(input + pos, words[i]);
			if (p) {
				pos = p - input + 1;
				len = strlen(words[i]);
				for (int j = pos; j < pos + len; j++)
					bitmap[j] = 1;

				p = NULL;
			} else {
				pos = 0;
				break;
			}
		} while(1);
	}

	len = strlen(input);
	for (int i = 0; i < len + 2; i++)
		printf("%d", bitmap[i]);

	printf("\n");

	/* output string with label */
	int i = 0;
	do {
		if (bitmap[i] == 0 && bitmap[i + 1] == 1)
			printf("<b>%c", input[i]);
		else if (bitmap[i] == 1 && bitmap[i + 1] == 0)
			printf("</b>%c", input[i]);
		else
			printf("%c", input[i]);
		i++;
	} while(input[i - 1]);

	printf("\n");
	return 0;
out:
	return -1;
}
