#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TIMELEN 20
#define MSGLEN	300
static int top;

typedef struct {
	int id;
	char time[TIMELEN];
} logsystem;

static logsystem *logsystemcreate() {
	logsystem *obj = (logsystem *)calloc(MSGLEN, sizeof(logsystem));

	return obj;
}

static void logsystemfree(logsystem *obj) {
	free(obj);
}

static void logsystemput(logsystem *obj, int id, char *timestamp) {
	obj[top].id = id;
	strcpy(obj[top].time, timestamp);
	top++;
}

static int get_cmplen(char *gra) {
	if (strcmp(gra, "Year") == 0)
		return 4;
	if (strcmp(gra, "Month") == 0)
		return 7;
	if (strcmp(gra, "Day") == 0)
		return 10;
	if (strcmp(gra, "Hour") == 0)
		return 13;
	if (strcmp(gra, "Minute") == 0)
		return 16;
	if (strcmp(gra, "Second") == 0)
		return 19;

	return 0;
}

static void searchlog(logsystem *obj, char *s, char *e, int cmp_len, int *retid, int *retsize) {
       	int count = 0;
	for (int i = 0; i < top; i++) {
	       if ((strncmp(obj[i].time, s, cmp_len) >= 0) && (strncmp(obj[i].time, e, cmp_len) <= 0)) {
		       retid[count] = obj[i].id;
		       count++;
	       }
	}
	*retsize = count;
}      			       

int* logsystemretreive(logsystem* obj, char *s, char *e, char *gra, int *retsize) {
	int cmp_len = get_cmplen(gra);
	int *retid = malloc(sizeof(int));

	searchlog(obj, s, e, cmp_len, retid, retsize);

	return retid;
}

void test_logsystem()
{
	logsystem *obj = logsystemcreate();
	int id1 = 1;
	char timestamp1[] = "2017:01:01:01:59:59";
	int id2 = 2;
	char timestamp2[] = "2016:01:01:01:59:59";
	char s[] = "2016:01:02:01:59:59";
	char e[] = "2017:01:01:01:59:59";
	char gra[] = "Day";
	int retsize;
	int *ret;
	logsystemput(obj, id1, timestamp1);
	logsystemput(obj, id2, timestamp2);
	ret = logsystemretreive(obj, s, e, gra, &retsize);

	for (int i = 0; i < retsize; i++) {
		printf("%d\n", ret[i]);
	}
}
		
