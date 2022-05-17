//------------ Package Headers
#include <graphics.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

//------------ Custom Headers
#include <mouse_r.h>
#include <key.h>
#include <bitmap.h>


class Basket {
    public:
        int current_x, current_y;
        void *shape;

        void init(int x, int y) {
            setcolor(4);
            setfillstyle(5, 4);
            sector(x, y, 180, 360, 50, 50);
            current_x = x;
            current_y = y;
            shape = malloc(imagesize(x-50, y, x+50, y+50));
            getimage(x-50, y, x+50, y+50, shape);
        };

        void move(int xpos) {
            putimage(xpos-50, current_y, shape, COPY_PUT);
            current_x = xpos;
            if(current_x != xpos) {
                cleardevice();
            }
        };
};

class Egg {
    public:
        int current_x, current_y;
        // bool status;
        float speed;
        void *shape;

        Egg() {
            // status = true;
            speed = 0.5;
        };
        
        int _y_step() {
            return (current_y + speed);
        };

        void init(int x, int y) {
            setcolor(7);
            setfillstyle(1, 7);
            fillellipse(x, y, 15, 30);
            current_x = x;
            current_y = y;
            speed = 10;
            shape = malloc(imagesize(x-15, y-30, x+30, y+60));
            getimage(x-15, y-30, x+30, y+60, shape);
        };

        void move() {
            if(!(current_y >= 410)) {
                putimage(current_x-15, _y_step() -30, shape, COPY_PUT);
                current_y = _y_step();
            } else {
                // status = false;
            }
            // delay(5);
            // clearviewport();
            // delay(50);
        };
};

int random_pos(int min, int max) {
    return rand() % (max-min + 1) + min;
};

const int FINAL_SCORE = 10;

class EggsCloud {
    public:
        int i, score;
        Egg eggs[FINAL_SCORE];
        Basket basket;

        EggsCloud() {
            i = 0;
            score = 0;
            eggs[i].init(random_pos(50, 500), 50);
            basket.init(330, 400);
        };

        void rain() {
            for(int j = 0; j <= i; ++j) {
                eggs[j].move();
                if((abs(eggs[j].current_x - basket.current_x) <= 50) && (abs(eggs[j].current_y - basket.current_y) == 0)) {
                    score = score + 1;
                }
            }

            if(i < FINAL_SCORE-1) {
                if(eggs[i].current_y > 200) {
                    i++;
                    eggs[i].init(random_pos(50, 500), 50);
                }
            }
        };


        void move_basket(int x) {
            basket.move(x);
        }
};