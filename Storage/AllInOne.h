
# ifndef STOR_H
    # include "DataTypes.h"
    # include "StringHandling.h"
    # include "FileHandling.h"


    # define STR_MAX 10000
    # define STOR_H 1


    char * encodeData(char *);
    char * decodeData(char *);
    int getStr(char *, size_t, FILE *);
    char * composeString(struct element );
    struct element *decomposeString(char *);
    char * itoa(int );
    char * reverse(char *);

    struct element * createElement(char *, char , struct tm);
    struct node_l * appendElement(struct node_l * l, struct element * elm);
    struct tm * getCurrentTime(void);
    void readNode(struct node_l *);
    void eraseData(void);
#endif