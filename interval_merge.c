#include <stdio.h>
#include <stdlib.h>

#if 0
#define max(a, b)  (a) > (b) ? (a) : (b)

int** sort(int** intervals, int intervalsSize){
    int tmp0, tmp1;
    for (int i = 0; i < intervalsSize - 1; i++){
  //      printf("before inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
        if (intervals[i][0] > intervals[i + 1][0]){
            tmp0 = intervals[i][0];
            tmp1 = intervals[i][1];
            intervals[i][0] = intervals[i + 1][0];
            intervals[i][1] = intervals[i + 1][1];
            intervals[i + 1][0] = tmp0;
            intervals[i + 1][1] = tmp1;
            //printf("inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
        }
    }
    return intervals;
}

int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes){
    int mergecnt = 0;
    printf("intervalsSize is %d, intervalcolsize is %d\n", intervalsSize, *intervalsColSize);
    if (intervalsSize == 1) {
	printf("coming here\n");
	int** array = (int **)malloc(sizeof(int *));
	array[0] = (int *)malloc(sizeof(int) * 2);
	array[0] = intervals[0];
        return array;
    }
  
    intervals = sort(intervals, intervalsSize); //排序

    for (int i = 0; i < intervalsSize; i++){ 
        printf("original inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
    }
   
    //merge interval
    for (int i = 0; i < intervalsSize - 1; i++){
        if (intervals[i][1] >= intervals[i + 1][0]){
            intervals[i + 1][0] = intervals[i][0];
            intervals[i + 1][1] = max(intervals[i][1], intervals[i + 1][1]);
            intervals[i][0] = -1; // will be deleted
            mergecnt++;
        }
    }
    
    for (int i = 0; i < intervalsSize; i++){ 
        printf("after merge inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
    }
   
    printf("merge: %d\n", mergecnt);
    int retsz = intervalsSize - mergecnt;    // 行数
    
    //malloc array
    int** array = (int **)malloc(retsz * sizeof(int *));
    int* retcolsz = (int *)malloc(retsz * sizeof(int));
    for (int j = 0; j < retsz; j++){
        array[j] = malloc(2 * sizeof(int));
        retcolsz[j] = 2;
    }
    
    //fill array
    int k = 0;
    for (int i = 0; i < intervalsSize; i++){
        if (intervals[i][0] != (-1)) {
            array[k][0] = intervals[i][0];
            array[k][1] = intervals[i][1];      
            k++;
        }
    }

    *returnSize = retsz;
    *returnColumnSizes = retcolsz;
     
    for (k = 0; k < retsz; k++) {
        printf("array[%d], %d, %d\n", k, array[k][0], array[k][1]);
        printf("returncolSizes, %d\n", (*returnColumnSizes)[k]);
    }
 
    printf("returnSize %d\n", *returnSize);
        
    return array;
}
#endif

#define max(a, b)  (a) > (b) ? (a) : (b)

int** sort_interval(int** intervals, int intervalsSize){
    int tmpstart, tmpend;
    int curstart, nextstart;
    for (int i = 0; i < intervalsSize - 1; i++){
        curstart = intervals[i][0];
        nextstart = intervals[i + 1][0];
        if (curstart > nextstart){
            tmpstart = intervals[i][0];
            tmpend = intervals[i][1];
            intervals[i][0] = intervals[i + 1][0];
            intervals[i][1] = intervals[i + 1][1];
            intervals[i + 1][0] = tmpstart;
            intervals[i + 1][1] = tmpend;
        }
    }
    return intervals;
}

int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes){
    int mergecnt = 0;
    int k = 0;
    int i;
    printf("intervalsSize is %d, intervalcolsize is %d\n", intervalsSize, *intervalsColSize);
    if (intervalsSize == 1) {
        printf("coming here\n");
        int** array = (int **)malloc(sizeof(int *));
        array[0] = malloc(sizeof(int) * 2);
        array[0] = intervals[0];
        return array;
    }

    intervals = sort_interval(intervals, intervalsSize); //排序

    for (i = 0; i < intervalsSize; i++){
        printf("original inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
    }

    //merge interval
    int curend, nextstart;
    for (i = 0; i < intervalsSize - 1; i++){
        curend = intervals[i][1];
        nextstart = intervals[i + 1][0];
        if (curend >= nextstart){
            intervals[i + 1][0] = intervals[i][0];
            intervals[i + 1][1] = max(intervals[i][1], intervals[i + 1][1]);
            intervals[i][0] = -1; // will be deleted
            mergecnt++;
        }
    }

    for (i = 0; i < intervalsSize; i++){
        printf("after merge inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
    }

    int retsz = intervalsSize - mergecnt;    // 行数

    //malloc array
    int** array = (int **)malloc(sizeof(int *) * retsz);
    int* retcolsz = (int *)malloc(sizeof(int) * retsz);
    for (int j = 0; j < retsz; j++){
        array[j] = malloc(sizeof(int) * 2);
        retcolsz[j] = 2;
    }

    //fill array
    for (i = 0; i < intervalsSize && k < retsz; i++){
        if (intervals[i][0] != (-1)) {
            array[k][0] = intervals[i][0];
            array[k][1] = intervals[i][1];
            k++;
        }
    }

    *returnSize = retsz;
    *returnColumnSizes = retcolsz;

    for (k = 0; k < retsz; k++) {
        printf("array[%d], %d, %d\n", k, array[k][0], array[k][1]);
        printf("returncolSizes, %d\n", (*returnColumnSizes)[k]);
    }

    printf("returnSize %d\n", *returnSize);

    return array;
}

