// Adrian Pilko 2024
// ZX81 Scroll Zone

// not sure there is a similar game to this like snake but constantly left or right scrolling
// Potential changes:
//    1) maybe reverse scroll left right top bottom when score reaches certain number
//    2) change rate that it all scrolls at, start easier then get faster
//    3) add proper score/game status  on top row: SCORE xxxx HIGH SCORE xxxx LEVEL xxxx

#include <input.h>
#include <zx81.h>
#include <stdio.h>

uint16_t highScore = 0;

// got scroll_left() pretty much from https://www.z88dk.org/forum/viewtopic.php?t=11668 , except I only scroll top half of the screen
// I might re-write using ldir instruction or similar 

int __FASTCALL__ scroll_left()
{
	#asm
	ld	hl,(16396)	; D_FILE
	inc	hl

	ld b, 12		; scrolling 12 lines left
.loop1
	ld a, (hl)
	cp 0x76
	jp z, empty_line

	ld d, h
	ld e, l		; de -> previous char
	inc hl		; hl -> current char

.first
	ld a, (hl)
	cp 0x76
	jr nz, copying

	ld a, 0
	ld (de), a		; fill last char with space
	jr empty_line

.copying
	ld (de), a
	inc de
	inc hl

	jr first

.empty_line

	inc hl
	djnz loop1

	#endasm
}

int __FASTCALL__ scroll_right()
{
	#asm
	ld	hl,(16396)	; D_FILE
	inc	hl
	ld de, 760 ; offset to bottom of screen 
	add hl, de

	ld b, 12		; scrolling 12 lines right
.rloop1
	ld a, (hl)
	cp 0x76
	jp z, rempty_line

	ld d, h
	ld e, l		; de -> previous char
	dec hl		; hl -> current char

.rfirst
	ld a, (hl)
	cp 0x76
	jr nz, rcopying

	ld a, 0
	ld (de), a		; fill last char with space
	jr rempty_line

.rcopying
	ld (de), a
	dec de
	dec hl

	jr rfirst

.rempty_line

	dec hl
	djnz rloop1

	#endasm
}



// combine 2 chars into 1 int
int combine(uchar y, uchar x)
{
	int p;
	p = (y<<8) + x;
	return p;
}


int __FASTCALL__ init_screen(uchar i)
{
	// fill the screen with spaces, so that i don't have to create new lines
	// does the same as filltxt, but i might decide to change resolution later
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


int __FASTCALL__ zx81_saddr(int yx)
// only works for 4k+ models, and only after using filltxt
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

    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	printf("\n");
	printf("+++++++ zx81 scroll zone +++++++\n");
    printf("\n");
    printf("    collect the dollars while\n");
	printf("         avoiding the x\n");
    printf("\n");
	printf("++++++++++++++++++++++++++++++++\n");
	printf("\n");
	printf("            keys:\n");  
	printf("       o=left p=right\n");
	printf("       q=up   a=down\n");
	printf("\n");
	printf("       press s to start\n");
	printf("\n");
	printf("\n");
    printf("          highscore\n");
	printf("              %d\n", highScore);
	printf("\n");
	printf("+++++ by a.pilkington 2024 ++++\n");
	printf("+++++ youtube: byteforever ++++\n");
	printf("+++++ version: 1.0         ++++");
}

int main()
{
	char control = '';
	uint16_t playerY = 23;
	uint16_t playerX = 15;
	uint16_t playerScreenPos = 0;
    uint16_t oldplayerScreenPos = 0;
    uint16_t score = 0;
	uint8_t restart = 0;

RESTART_LABEL:
    playerX = 15;
	playerY = 10;
	printOpeningScreen();

    while (in_Inkey() != 'S')
	{
		//
	}

  	init_screen(0);

	while (1)
	{
#if 0
// for some reason this doesn't just delay for 2 frame cycles, perhaps the FRAMES variable is different
// in the z88dk zx81 runtime??
		#asm		
		ld b,2
waitForTVSync:	
		ld a,(0xf5a3)
		ld c,a
sync:
		ld a,(0xf5a3)
		cp c
		jr z,sync
		djnz waitForTVSync
		#endasm
#endif
		control = in_Inkey();

		switch (control)
		{
			case 'O' : if (playerX > 1) playerX -= 1; break;
			case 'P' : if (playerX < 31) playerX += 1; break;
			case 'Q' : if (playerY > 0) playerY -= 1; break;
			case 'A' : if (playerY < 23) playerY += 1; break;
			default: break;
		};

        // update screen position based on player X Y 

        oldplayerScreenPos = playerScreenPos;
		playerScreenPos = zx81_saddr(combine(playerY,playerX));

		// we have to adjust the screen position based on Y position
		// bottom half needs nudging left above half nudge right
		if (bpeek(playerScreenPos) == 61)
		{
			score = 0;
		#asm
		    ld b, 10
			ld c, 0
		    call 0x08F5    ; print at sets cursor position
		#endasm			
			printf("********* GAME OVER *********\n");
		#asm
		
		    ld b, 0xff 
loopDelay1:
				push b
				ld b, 0x6f
loopDelay2:
				djnz loopDelay2 
			    pop b
            djnz loopDelay1 
		#endasm	
			goto RESTART_LABEL;  // advised use of goto here is best way
		}
		if (bpeek(playerScreenPos) == 13) // check if got a dollar
		{
			score = score + 10;
			if (score > highScore) highScore = score;
		}

		bpoke (playerScreenPos, 189);
        
		bpoke (zx81_saddr(combine((rand()%12),30)), 61); 
		bpoke (zx81_saddr(combine((rand()%12)+12,0)), 61); 

		bpoke (zx81_saddr(combine((rand()%12),30)), 13); 
		bpoke (zx81_saddr(combine((rand()%12)+12,0)), 13); 

		
		scroll_right();
		
		#asm
		    ld b, 0
			ld c, 1
		    call 0x08F5    ; print at sets cursor position
		#endasm
		printf("%d", score);
		
		scroll_left();
		
	}
    
	return 1;
}
