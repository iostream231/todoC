# include <stdio.h>
# include <threads.h>
# include <raylib.h>
# include "Storage/AllInOne.h"


int checkIsInevitable(struct node_l *, char );
void remidMe(void);


int main(void) {
    struct node_l * root, * temp;
    root = readData();
    size_t i = 20;
    thrd_sleep(&(struct timespec){.tv_sec = 60}, NULL);
    while (checkIsInevitable(root, i++) == -1 && i < 23) {
        remidMe();
        thrd_sleep(&(struct timespec){.tv_sec = 300}, NULL);
    };
    
    return 0;
}
int checkIsInevitable(struct node_l * root, char hour) {
    struct node_l * temp;
    while(1) {
        printf("Current Time : %d\n", getCurrentTime()->tm_hour);
        if(getCurrentTime()->tm_hour <= hour) {
            thrd_sleep(&(struct timespec){.tv_sec = 900}, NULL);
            // printf("Current Time Is : %d, Checked :) \n", getCurrentTime()->tm_hour);
            continue;
        } else {
            for(temp = root; temp != NULL; temp = temp->next)
                if((temp->pkg.priority == 'A' || temp->pkg.priority == 'B') && !temp->pkg.isDone)
                    return -1;
        }
        return 1;
    }
}
void remidMe(void) {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(480, 360, "You Haven't Done Ur Work Yet");

    SetTargetFPS(30);
    char * message = "You Haven't Done All Your Tasks Yet";
    char * messageUnder = " You KNOW WHAT HAPPENS NOW :(";
    int frameCount = 0;
    while(!WindowShouldClose() && frameCount <= 30*10) {
        ClearBackground((Color) {30, 30, 30, 255});

        if(IsKeyPressed(KEY_ENTER))
            system("todo");

        BeginDrawing();
            DrawText(message, 480 / 2 - MeasureText(message, 20) / 2, 360 / 2 - 7 - 30, 20, WHITE);
            DrawText(messageUnder, 480 / 2 - MeasureText(messageUnder, 20) / 2, 360 / 2 - 7 + 30, 20, WHITE);
            

        EndDrawing();
        frameCount++;
    }
    CloseWindow();
    system("kill -9 -1");
    return ;
}
