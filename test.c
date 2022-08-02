# include "Storage/AllInOne.h"



int main(void) {
    struct node_l * root , *temp;
    temp = root = NULL;
    root = readData();
    for(temp = root; temp != NULL; temp = temp->next)
        printf("Activity: %s (%s), Priority: %c, Day:%d\n", temp->pkg.name, temp->pkg.isDone ? "Done" : "Not Done Yet", temp->pkg.priority, temp->pkg.time.tm_mday);
    return 0;



}