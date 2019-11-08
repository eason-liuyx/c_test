#include <stdio.h>
#include <string.h>

char * longestPalindrome(char* s){
    int mirroridx1 = 0;
    int mirroridx2 = 0;
    int eq_len1 = 0;
    int eq_len2 = 0;
    int max_len1 = 0;
    int max_len2 = 0;
    int right = 0;
    int slen = strlen(s);
   
    printf("%d\n", slen);
    /* even case: similar as bb */
    for (int i = 0; i < slen - 1; i++) {
        eq_len1 = 0;
        
        if (s[i] == s[i + 1]){
            eq_len1 = 1;
	    int j = i - 1;
	    right = i + 1 + i - j;
            while (j >= 0 && right < slen){
                if (s[j] != s[right])
                    break;

                eq_len1++;
                j--;
		right = i + 1 + (i - j);
            }

            if (eq_len1 >= max_len1){
                max_len1 = eq_len1;
                mirroridx1 = i;
            }
        }        
    }

    printf("%d\n", max_len1);
    
    /* odd case: similar as bab */
    for (int i = 0; i < slen - 2; i++){
        eq_len2 = 0;

        if (s[i] == s[i + 2]){
            eq_len2 = 1;
            int j = i - 1;
	    right = i + 2 + i - j;
            while (j >= 0 && right < slen){
                if (s[j] != s[right])
                    break;

                eq_len2++;
                j--;
	        right = i + 2 + i - j;
            }
            
            if (eq_len2 >= max_len2){
                max_len2 = eq_len2;
                mirroridx2 = i;
            }
        }
    }
    
    printf("%d\n", max_len2);

    /* even */
    if (max_len1 > max_len2){
	int k;
        char outs[2 * max_len1 + 1];
        for (k = 0; k < (2 * max_len1); k++)
            outs[k] = s[mirroridx1 - (max_len1 - k) + 1];
	outs[k] = '\0'; 
	s = outs;
        
        return s;
    } else { /* odd */
    	int k;
        char outs[2 * max_len2 + 2];
        for (k = 0; k < (2 * max_len2 + 1); k++)
            outs[k] = s[mirroridx2 - (max_len2 - k) + 1];
	outs[k] = '\0';
	s = outs;

        return s;
    }
}
