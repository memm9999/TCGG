//------------ Package Headers
#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <PROCESS.h>

//------------ Custom Headers
#include <egc.h>
// #include <mouse_r.h>
// #include <key.h>
// #include <bitmap.h>

void main() {
    int gdriver=DETECT,
        gmode;

    initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

    int b, x, y;

    EggsCloud eggs_cloud;

    while(!kbhit()) {
        getmouse(&b, &x, &y);
        eggs_cloud.rain();
        eggs_cloud.move_basket(x);
        gotoxy(72, 2);
        printf("score:%d", eggs_cloud.score);
        delay(75);
        clearviewport();
        // printf("%x", x);
    }

    // getch();
    // closegraph();
}