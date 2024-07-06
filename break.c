#include <input.h>
#include <zx81.h>
#include <stdio.h>


int __FASTCALL__ scroll_left()
// works on all models, untested.
{
	#asm
	ld	hl,(16396)	; D_FILE
	inc	hl

	ld b, 21		; scrolling 20 lines for now

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

int main()
{
	init_screen(0);

	while (in_Inkey()!='Q')
	{
		scroll_left();

		bpoke (zx81_saddr(combine(rand()%21,30)), 128); // '*'
	}
    
	return 1;
}
