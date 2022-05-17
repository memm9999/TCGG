////////////////////////////////
//By AL Bara Ramli           ///
//(Tripoly Libya :14/9/2007)///
//My Email:     sbr_system@yahoo.com       ///
//My Web Site:     sbrsystem.8m.com           ///
////////////////////////////////
#include <bios.h>
#include <ctype.h>

//#define RIGHT  0x01
//#define LEFT  0x02
//#define CTRL  0x04
//#define ALT   0x08
#define Insert 0x5200
#define Home 0x4700
#define Delete 0x5300
#define End 0x4f00
#define Page_Up 0x4900
#define Page_Down 0x5100
#define Up 0x4800
#define Down 0x5000
#define Right 0x4d00
#define Left 0x4b00
#define Back_Space 0xe08
#define Enter 0x1c0d
#define Space 0x3920
#define Tab 0xf09
#define F1 0x3b00
#define F2 0x3c00
#define F3 0x3d00
#define F4 0x3e00
#define F5 0x3f00
#define F6 0x4000
#define F7 0x4100
#define F8 0x4200
#define F9 0x4300
#define F10 0x4400
#define Esc 0x11b
int getkey()
{
int modifiers,key;
/* function 1 returns 0 until a key is pressed */
while (bioskey(1) == 0);
/* function 0 returns the key that is waiting */
key = bioskey(0);
  /* use function 2 to determine if shift keys were used */
  /* modifiers = bioskey(2);
  if (modifiers)
  {
    if (modifiers & RIGHT) //printf("RIGHT");
    if (modifiers & LEFT)  //printf("LEFT");
    if (modifiers & CTRL)  //printf("CTRL");
    if (modifiers & ALT)  //printf("ALT");
  }*/
return key;
}
int it_key(int c)
{
switch(c)
  {
  case Insert:case Home:case Delete:case End:case Page_Up:
  case Page_Down:case Up:case Down:case Right:case Left:
  case Back_Space:case Enter:case Space:case Tab:case F1:
  case F2 :case F3:case F4:case F5:case F6:case F7:
  case F8:case F9:case F10:case Esc: c=1;break;
  default  : c=0; break;
  }
  return c;
}
