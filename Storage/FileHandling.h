# include <stdio.h>
# include <stdlib.h>

# define MAXLINE 100


static FILE * storage;
static char * StoragePath = "./db";
static struct node_l * buff;

void storeData(struct node_l * dat) {


    if((storage = fopen(StoragePath, "w")) == NULL) {
        printf("E: Error Accessing File %s\n", StoragePath);
        return ;
    }
    struct node_l * temp;
    for(temp = dat; temp != NULL && temp->pkg.name != NULL; temp = temp->next){
        fputs(encodeData(composeString(temp->pkg)), storage);
        fputc('\n', storage);
    }
    // TODO: solve the munmap_chunk(): invalid pointer error
    fclose(storage);
}
struct node_l * readData(void) {
    if((storage = fopen(StoragePath, "r")) == NULL)
        return NULL;
    char line[100];
    struct node_l * temp = NULL;
    int i, l;
    for(i = 0; (l = getc(storage)) != EOF && i < MAXLINE;) {
        if(l != '\n')
            line[i++] = l;
        else {
            temp = appendElement(temp, decomposeString(decodeData(line)));
            while(i > 0)
                line[i--] = '\0';
            
        }
    }
    
    fclose(storage);
    return temp;
}

void eraseData(void) {
    if((storage = fopen(StoragePath, "w")) == NULL)
        return ;
    putc(EOF, storage);
    fclose(storage);
}