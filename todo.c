# include <stdio.h>
# include "Storage/AllInOne.h"
# define RAYGUI_IMPLEMENTATION 1
# include <raylib.h>
# include <ctype.h>
// # include "raygui/src/raygui.h"

// # include "GUI/day.h"

enum {DAY=1, WEEK, MONTH, YEAR} CurrentState;

const static unsigned int WIDTH = 1080;
const static unsigned int HEIGHT = 720;

enum {DEF = 1, CONFIRM, CREATE} currentState = DEF;
enum {NAME = 4, PRIORITY} focus = NAME;

int main(void) {
    struct node_l * root,* todayR;

    SetTraceLogLevel(LOG_ERROR);

    for(root = readData(), todayR = NULL; root != NULL; root = root->next)
        if(root->pkg.time.tm_mday == getCurrentTime()->tm_mday)
            todayR = appendElement(todayR, &root->pkg);
    
    root = readData();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WIDTH, HEIGHT, "Day List");

    Font fonts[8] = {
        LoadFont("todo_Lib_fonts/Ubuntu-Bold.ttf"),
        LoadFont("todo_Lib_fonts/Ubuntu-BoldItalic.ttf"),
        LoadFont("todo_Lib_fonts/Ubuntu-Italic.ttf"),
        LoadFont("todo_Lib_fonts/Ubuntu-Light.ttf"),
        LoadFont("todo_Lib_fonts/Ubuntu-LightItalic.ttf"),
        LoadFont("todo_Lib_fonts/Ubuntu-Medium.ttf"),
        LoadFont("todo_Lib_fonts/Ubuntu-MediumItalic.ttf"),
        LoadFont("todo_Lib_fonts/Ubuntu-Regular.ttf"),
        
    };


    struct node_l * root_F = todayR;
    SetTargetFPS(30);
    char currentTime[100];
    char task_str[100];
    strftime(currentTime, 100, "%d %A %b", getCurrentTime());
    size_t pending, done;
    struct node_l * temp;
    Rectangle recs_tsks[50];
    size_t i;
    for(temp = root_F, pending = done = 0; temp != NULL; temp = temp->next) {
        recs_tsks[done + pending] = (Rectangle) {10, 55 + (done + pending) * 45, WIDTH -20, 40};
        if(temp->pkg.isDone)
            done++;
        else 
            pending++;
    }
    
    sprintf(task_str, "Tasks: %d/%d", done, pending+done);

    char priority_str[2];
    priority_str[1] = '\0';

    struct element * semp = NULL;
    Rectangle addButton = (Rectangle) {10, 55 + (done+pending)*45, WIDTH - 20, 35};

    unsigned char task_name[100], task_name_count = 0, c, prio = 'A';
    for(size_t f = 0; f < 100; f++)
        task_name[f] = '\0';


    unsigned int frameCount = 0;
    size_t confirmed, hasChanged = 0;
    while(!WindowShouldClose()) {
        ClearBackground((Color) {35, 35, 35, 255});


        if(currentState == DEF) {
            if(semp != NULL) {
                if(confirmed == 1) {
                    semp->isDone = 1;
                    done++, --pending;
                    sprintf(task_str, "Tasks: %d/%d", done, pending+done);
                    hasChanged = 1;
                    storeData(root);
                }
                confirmed = 0;
                semp = NULL;
            }
            BeginDrawing(); 
            /*                                          Top Section                             */
            DrawRectangle(0, 0, WIDTH, 50, (Color) {20, 20, 20, 255});
            if(CheckCollisionPointCircle(GetMousePosition(), (Vector2) {20, 20}, 20.0f)) {
                DrawCircle(20, 20, 20.0f, BLACK);
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                if(IsGestureDetected(GESTURE_TAP)) {
                    if(hasChanged)
                        storeData(root_F);
                    return 2;
                }
            } else 
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            DrawLine(10, 20, 30, 20, WHITE);
            DrawLine(10, 20, 20,  30, WHITE);
            DrawLine(10, 20, 20,10, WHITE);

            DrawTextEx(fonts[0], currentTime, (Vector2) {
                50 + WIDTH / 2 - MeasureText(currentTime, 20) / 2 - 50,
                15,
            }, 20, 2.0f, WHITE);

            DrawTextEx(fonts[3], task_str, (Vector2){
                50 + WIDTH - 150 - MeasureText(task_str, 20) / 2,
                15,
            }, 20, 2.0f, WHITE);

            DrawLine(WIDTH-200, 0, WIDTH-200, 50, (Color) {255, 255, 255, 255});


            /*                                          Main Section                            */
            for(temp = root_F, i = 0; temp != NULL; temp = temp->next, ++i) {
                DrawRectangleRounded(recs_tsks[i], 0.3f, 0, (Color) {27, 27, 27, 255});
                if(!temp->pkg.isDone) {
                    switch(temp->pkg.priority) {
                        case 'A' :
                            DrawRectangleRounded((Rectangle) {recs_tsks[i].x, recs_tsks[i].y, 30 ,recs_tsks[i].height}, 0.3f, 0, (Color) {170, 255, 170, 255});
                            break;
                        case 'B' :
                            DrawRectangleRounded((Rectangle) {recs_tsks[i].x, recs_tsks[i].y, 30 ,recs_tsks[i].height}, 0.3f, 0, (Color) {255, 170, 170, 255});
                            break;
                        case 'C' :
                            DrawRectangleRounded((Rectangle) {recs_tsks[i].x, recs_tsks[i].y, 30 ,recs_tsks[i].height}, 0.3f, 0, (Color) {170, 170, 255, 255});
                            break;
                        case 'D' :
                            DrawRectangleRounded((Rectangle) {recs_tsks[i].x, recs_tsks[i].y, 30 ,recs_tsks[i].height}, 0.3f, 0, (Color) {200, 200, 200, 255});
                            break;
                        default :
                            DrawRectangleRounded((Rectangle) {recs_tsks[i].x, recs_tsks[i].y, 30 ,recs_tsks[i].height}, 0.3f, 0, (Color) {27, 27, 27, 255});
                            break;
                    }
                    DrawTextEx(fonts[7], temp->pkg.name, 
                    (Vector2) {
                        40 + WIDTH / 2 - 10 - MeasureText(temp->pkg.name, 20) / 2,
                        10 + recs_tsks[i].y
                    }, 20, 2.0f, WHITE);

                    if(CheckCollisionPointRec(GetMousePosition(), recs_tsks[i])) {
                        if(recs_tsks[i].height < 45) {
                            recs_tsks[i].y -= 5;
                            recs_tsks[i].height += 5;
                        }
                        if(IsGestureDetected(GESTURE_TAP)) {
                            currentState = CONFIRM;
                            semp = &temp->pkg; 
                        }
                    } else {
                        recs_tsks[i].y = 55 + 45 * i;
                        recs_tsks[i].height = 40;
                    }
                } else {
                    DrawRectangleRounded((Rectangle) {recs_tsks[i].x, recs_tsks[i].y, 30 ,recs_tsks[i].height}, 0.3f, 0, (Color) {27, 27, 27, 255});
                    DrawTextEx(fonts[7], temp->pkg.name, 
                    (Vector2) {
                        40 + WIDTH / 2 - 10 - MeasureText(temp->pkg.name, 20) / 2,
                        10 + recs_tsks[i].y
                    }, 20, 2.0f, GRAY);
                    recs_tsks[i].y = 55 + 45 * i;
                    recs_tsks[i].height = 40;
                    DrawLine(40 + WIDTH / 2 - 10 - MeasureText(temp->pkg.name, 20) - 50, recs_tsks[i].y + 20, 40 + WIDTH / 2 - 10 + MeasureText(temp->pkg.name, 20) + 50,recs_tsks[i].y + 20, LIGHTGRAY);
                }
                priority_str[0] = temp->pkg.priority;
                DrawTextEx(fonts[0], priority_str, 
                (Vector2) {
                    10 + 10,
                    10 + recs_tsks[i].y,
                }, 20, 2.0f, WHITE);
            }

            /*                                                                          Add Button                                                                      */
            if(CheckCollisionPointRec(GetMousePosition(), addButton)) {
                DrawRectangleRec(addButton, (Color) {45, 45, 45, 255});
                if(IsGestureDetected(GESTURE_TAP)) 
                    currentState = CREATE;
                
            };
            DrawLine(10 + WIDTH / 2 - 10, addButton.y + addButton.height / 2, 10 + WIDTH / 2 + 10, addButton.y + addButton.height /2, WHITE);
            DrawLine(10 + WIDTH / 2, addButton.y + addButton.height / 2 - 10, 10 + WIDTH / 2, addButton.y + addButton.height  / 2+ 10, WHITE);
            EndDrawing();
        } else if(currentState == CONFIRM) {
            BeginDrawing();
                
                DrawTextEx(fonts[0], "Are You Sure You've Done That ?", (Vector2) {
                    WIDTH / 2 - MeasureText("Are You Sure YOu've Done That ?", 25) / 2,
                    HEIGHT / 2 - 32,
                }, 25.0f, 2.0f, WHITE);
                DrawRectangleLines(70, HEIGHT / 2 + 20, 100, 50, GRAY);
                DrawRectangleLines(WIDTH - 170, HEIGHT / 2 + 20, 100, 50, GRAY);
                DrawTextEx(fonts[0], "Yes", (Vector2) {
                    70 + 50 - MeasureText("Yes", 20)/ 2,
                    HEIGHT / 2 + 20 + 15
                }, 20.0f, 2.0f, WHITE);
                DrawTextEx(fonts[0], "No", (Vector2) {
                    WIDTH - 170 + 50 - MeasureText("No", 20) / 2,
                    HEIGHT / 2 + 20 + 15
                }, 20.0f, 2.0f, WHITE);
                                
                if(IsGestureDetected(GESTURE_TAP)) {
                    if(CheckCollisionPointRec(GetMousePosition(), (Rectangle){70, HEIGHT / 2 +20, 100, 50})) 
                        confirmed = 1;
                    else 
                        confirmed = -1;
                    currentState = DEF;
                }


            EndDrawing();
        } else if(currentState == CREATE) {
            BeginDrawing();
            DrawRectangleLines(200, 100, 720, 480, GRAY);
            DrawTextEx(fonts[3], "Name :", (Vector2) {250, 175}, 20, 2.0f, WHITE);
            DrawRectangleLines(225, 200, 680, 70, focus == NAME ? WHITE : GRAY);

            
            if(isalnum(c = GetCharPressed()) || isspace(c)) {
                switch(focus) {
                    case NAME:
                        task_name[task_name_count++] = c;
                        break;
                    case PRIORITY :
                        prio = c;
                        break;
                }
            }
            if(IsKeyPressed(KEY_LEFT_CONTROL))
                focus = (focus == NAME) ? PRIORITY : NAME;
            if(IsKeyPressed(KEY_ENTER) && strlen(task_name) > 2 && prio) {
                task_name[task_name_count] = '\0';
                root_F = appendElement(root_F, createElement(task_name, prio, *getCurrentTime()));
                for(temp = root_F, pending = done = 0; temp != NULL; temp = temp->next) {
                    recs_tsks[done + pending] = (Rectangle) {10, 55 + (done + pending) * 45, WIDTH -20, 40};
                    if(temp->pkg.isDone)
                        done++;
                    else 
                    pending++;
                }
                addButton = (Rectangle) {10, 55 + (done+pending)*45, WIDTH - 20, 35};
                sprintf(task_str, "Tasks: %d/%d", done, pending+done);
                currentState = DEF;
                hasChanged = 1;
            }
	    task_name[task_name_count] = '_';
            if(IsKeyDown(KEY_BACKSPACE) && task_name_count > 0 && frameCount % 2 == 0)
                task_name[task_name_count--] = '\0';
            task_name[task_name_count] = '_';
            DrawTextEx(fonts[7], task_name, (Vector2){250, 235 - 10}, 20.0f, 2.0f, WHITE);

            DrawTextEx(fonts[3], "Priority :", (Vector2) {250, 350}, 20.0f, 2.0f, WHITE);
            DrawRectangleLines(375, 350, 40, 40, focus == PRIORITY ? WHITE : GRAY);
            priority_str[0] = prio;
            DrawTextEx(fonts[7], priority_str, (Vector2){385, 360}, 20.0f, 2.0f, WHITE);

            EndDrawing();
        }
    frameCount++;
    if(frameCount >= 10000)
        frameCount = 0;
    }
    // eraseData();
    if(hasChanged)
        storeData(root_F);
    // I really want to close the window, but it causes an error, maybe later :()
    return -1;
}


