#include <graphics.h>
#include <stdio.h>

int x [] = {
    0, 32, 65, 100, 100, 85, 120, 135, 130, 196, 141, 168,
    168, 200, 250, 139, 108, 44, 60, 30, 0,
    -1, 120};
int y [] = {
    162, 130, 145, 145, 130, 114, 100, 128, 108, 108, 141, 141,
    133, 126, 151, 170, 170, 165, 182, 190, 162,
    -1, 144};
int main (void) {
    int coordinate;
    int size;
    int index;
    clg ();
    draw (8, 8, 92, 8);
    draw (92, 8, 92, 92);
    draw (92, 92, 8, 92);
    draw (8, 92, 8, 8);
    coordinate = 10;
    for (size = 81; size > 4; size -= 4) {
        drawb(coordinate, coordinate, size, size);
        coordinate++;
    }
//    uncircle (50, 50, 20, 1);
 //   uncircle (50, 50, 25, 1);
  //  uncircle (50, 50, 30, 1);
   // uncircle (50, 50, 35, 1);
//    drawr (84, 0);
 //   drawr (0, 84);
  //  drawr (-84, 0);
   // drawr (0, -84);
   // xorborder (110, 10, 81, 81);
    //xorborder (113, 13, 31, 31);
    circle (150, 50, 20, 1);
    circle (150, 50, 25, 1);
    circle (150, 50, 30, 1);
    circle (150, 50, 35, 1);
    for (index = 1; x[index] >= 0; index++) {
        draw (x[index - 1], y[index - 1], x[index],y [index]);
    }
    fill (x [index + 1], y [index + 1]);
    return 0;
}
