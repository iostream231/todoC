# include <stdlib.h>
# include <threads.h>
# include <time.h>

// Reminds You Every Hour To Do Your Freaking Chores
// Damn IT, AM I THAT UNCONTROLABLE
int main(void){
    time_t current;
    while(current = time(NULL) && gmtime(&current)->tm_hour <= 21) {
        system("todo");
        thrd_sleep(&(struct timespec) {.tv_sec=3600}, NULL);
    }
    system("enf");
    return 0;
}