// Adrian Pilko 2024
// ZX81 Scroll Zone

// not sure there is a similar game to this like snake but constantly left or right scrolling
// Potential changes:
//    1) maybe reverse scroll left right top bottom when score reaches certain number. DONE
//    2) change rate that it all scrolls at, start easier then get faster
//    3) add proper score/game status  on top row: SCORE xxxx HIGH SCORE xxxx LEVEL xxxx

#include <input.h>
#include <zx81.h>
#include <stdio.h>
#include <math.h>

uint16_t highScore = 0;



// got from https://www.z88dk.org/forum/viewtopic.php?t=11668 
// combine 2 chars into 16 bit int
int combine(uchar y, uchar x)
{
	int p;
	p = (y<<8) + x;
	return p;
}


// got from https://www.z88dk.org/forum/viewtopic.php?t=11668 
int __FASTCALL__ init_screen(uchar i)
{
	#asm
	ld a, l
	ld hl,(16396)	; D_FILE
	inc	hl

	ld b, 23

.loop_init
	push bc
	ld b, 32

.loop_row1
	ld (hl), a
	inc hl
	djnz loop_row1

	ld (hl), 0x76
	inc hl
	pop bc

	djnz loop_init
	#endasm
}


// got from https://www.z88dk.org/forum/viewtopic.php?t=11668 
int __FASTCALL__ zx81_saddr(int yx)
{
	#asm
	ld b, h
	ld c, l

	ld hl,(16396)	; D_FILE
	inc hl
	ld de, 33		; line size (might change later), this only works for 4k+ models

	ld a, b
	and a
	jr z, no_rows1

.loop_rows1
	add hl, de
	djnz loop_rows1

.no_rows1
	ld b, 0
	add hl, bc

	#endasm
}

int __FASTCALL__ printOpeningScreen()
{
	#asm
	    call 0x0A2A   ; clear screen
	#endasm

    printf("ooooooooooooooooooooooooooooooo\n");
	printf("\n");
	printf("++++++++  zx81 roids  ++++++++\n");
    printf("\n");
    printf("       shoot the asteroids!\n");
	printf("\n");
    printf("\n");
	printf("ooooooooooooooooooooooooooooooo\n");
	printf("\n");
	printf("            keys:\n");  
	printf("  o=rotate left, p=rotate right\n");
	printf("       space=fire rockets\n");
	printf("        z= fire missiles\n");
	printf("        press s to start\n");
	printf("\n");
	printf("\n");
    printf("          highscore\n");
	printf("              %d\n", highScore);
	printf("\n");
	printf("+++++ by a.pilkington 2024 ++++\n");
	printf("+++++ youtube: byteforever ++++\n");
	printf("+++++ version: 0.1         ++++");
}

int main()
{
	char control = '';
	int16_t playerY = 23;
	int16_t playerX = 15;
	int16_t playerScreenPos = 0;
    int16_t oldplayerScreenPos = 0;
    int16_t score = 0;
	int8_t restart = 0;
    int8_t enemyChar = 61;
    int8_t playerChar = 189;
    int8_t engine = 0;
    int16_t angle = 0;
	int8_t missile = 0;
	int8_t velocity = 0;
    int8_t scoreSwitch = 0;

// ok so using "goto", call the cops! but we have a mix of assembly anyway!
RESTART_LABEL:
    engine = 0;
    score = 0;
    angle = 0;
	engine = 0;
	missile = 0;
	velocity = 0;
    playerX = 15;
	playerY = 10;
    enemyChar = 61;
    playerChar = 189;
	printOpeningScreen();

    while (in_Inkey() != 'S')
	{
		// just hard loop waiting
	}

  	init_screen(0);

	while (1)
	{
		control = in_Inkey();

		switch (control)
		{
			case 'O' : if (angle > 0) angle-=10; if (angle == 0) angle = 359; break;
			case 'P' : if (angle < 359) angle+=10; if (angle == 359) angle = 0; break;
			case 'Z' : missile=1; break;
			case ' ' : engine=1; break;
			default: break;
		};

        if ((engine == 1) && (velocity < 4)) velocity+=0.2;
        engine = 0;
		playerX += sin(angle*0.0175) * velocity;
		playerY += cos(angle*0.0175) * velocity;
        
		if (playerX > 30) playerX = 30;
		if (playerX < 0)  playerX = 0;
		if (playerY > 21) playerY = 21;
		if (playerY < 0)  playerY = 0;
		
		#asm
		    ld b, 0
			ld c, 0
		    call 0x08F5    ; print at sets cursor position
		#endasm
		printf("x = %d\n", playerX);
		printf("y = %d\n", playerY);
		printf("angle = %d\n", angle);
		printf("angle radians= %f\n", angle*0.0175);
		printf("velocity = %d\n", velocity);		

        bpoke (playerScreenPos, 128);
        // update screen position based on player X Y 
        oldplayerScreenPos = playerScreenPos;
		playerScreenPos = zx81_saddr(combine(playerY,playerX));

        bpoke (playerScreenPos, 8);

		if (velocity > 0) velocity -= 1;
	}
    
	return 1;
}
