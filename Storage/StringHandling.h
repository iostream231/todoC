# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>



char * encodeData(char *);
char * decodeData(char *);
int getStr(char *, size_t, FILE *);
char * composeString(struct element );
struct element * decomposeString(char *);
char * itoa(int );
char * reverse(char *);

char * encodeData(char data[]) {
    char * w;
    for(w = data; *w != EOF && *w != '\0' ; ++w)
        if(*w != '\n' && *w != '\b' && *w != '\t')
            *w += 3;
    return data;
        
}
char * decodeData(char data[]) {
    char * w;
    for(w = data; *w != EOF && *w != '\0' ; ++w)
        if(*w != '\n' && *w != '\b' && *w != '\t')
            *w -= 3;
    return data;
}

int getStr(char string[], size_t max, FILE * fp) {
    if(fp == NULL)
        return -1;
    int c, i;
    for(i = 0; (c = fgetc(fp)) != EOF && i < max; ++i)
        string[i] = c;
    if(c == '\n')
        string[i++] = c;
    string[i] = '\0';
    return i;
 
}
char * composeString(struct element todo) {
    if(todo.name == NULL)
        return NULL;
    char * str = (char *) malloc(sizeof(todo.name)+100*sizeof(char)), * w, i; // 100 is a maximum I guess
    for(w = str; (*w = *todo.name) != '\0'; ++w, todo.name++)
        ;
    
    *w++ = '/';
    *w++ = todo.priority;
    *w++ = '/';
    *w++ = todo.isDone + '0';
    *w++ = '/';
    char * n;
    for(n = itoa(timegm(&todo.time)); (*w = *n++) != '\0'; ++w)
        ;

    return str;
}
char * itoa(int c) {
    char * f = (char *) malloc(log10(c)*sizeof(char)+10), * w = f; // 10 just in case
    while(c != 0) {
        *w++ = c % 10 + '0';
        c /= 10;
    }
    *w = '\0';
    return reverse(f);
}
char * reverse(char s[]) {
    int i, j, temp;
    for(i = 0, j = strlen(s)- 1; i < j; i++, --j) {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
    return s;
}
struct element * decomposeString(char * s) {
    char * w;
    struct element * res = (struct element *) malloc(sizeof(*res));
    unsigned i;
    for(w = s, i = 0, res->name = (char *) malloc((strlen(s) + 1)*sizeof(char));*w != '/'; ++i, w++)
        res->name[i] = *w;
    res->name[i++] = '\0';
    res->priority = *++w;
    w+=2;
    res->isDone = *w-'0';
    w+=2;
    time_t bruh = (time_t) atoi(w);
    res->time = *gmtime(&bruh);
    return res;
}
