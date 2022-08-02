# include <time.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <stdio.h>

struct element {
    char * name;
    char priority;
    struct tm time;
    unsigned int isDone : 1;
};
struct node_l {
    struct element pkg;
    struct node_l * next;
};


struct element * createElement(char * name, char priority, struct tm T) {
    struct element * res;
    res = (struct element *) malloc(sizeof(*res));
    res->name = strdup(name);
    res->priority = priority;
    res->time = T;
    res->isDone = 0;


    return res;
}

struct node_l * appendElement(struct node_l * l, struct element * elem) {
    if(l == NULL) {
        l = (struct node_l *) malloc(sizeof(*l));
        l->pkg = *elem;
        l->next = NULL;
    } else 
        l->next = appendElement(l->next, elem);
    return l;
}
struct tm * getCurrentTime(void) {
    time_t current;
    current = time(NULL);
    return gmtime(&current);
}
void readNode(struct node_l * dat) {
    if(dat != NULL) {
        printf("%s, %c, %u, %d\n", dat->pkg.name, dat->pkg.priority,timegm(&dat->pkg.time), dat->pkg.isDone);
        readNode(dat->next);
    }
}

struct element * accessElement(struct node_l * l, size_t f){
    struct node_l * temp;
    for(temp = l; f > 0, temp != NULL; f--, temp = temp->next)
        ;
    return (temp != NULL) ? &temp->pkg : NULL;
         
}