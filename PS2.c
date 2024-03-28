/*******************************************************************************
 * This file provides address values that exist in the DE1-SoC Computer
 ******************************************************************************/

#ifndef __SYSTEM_INFO__
#define __SYSTEM_INFO__

#define BOARD				"DE1-SoC"

/* Memory */
#define DDR_BASE			0x40000000
#define DDR_END				0x7FFFFFFF
#define A9_ONCHIP_BASE			0xFFFF0000
#define A9_ONCHIP_END			0xFFFFFFFF
#define SDRAM_BASE			0x00000000
#define SDRAM_END			0x03FFFFFF
#define FPGA_PIXEL_BUF_BASE		0x08000000
#define FPGA_PIXEL_BUF_END		0x0803FFFF
#define FPGA_CHAR_BASE			0x09000000
#define FPGA_CHAR_END			0x09001FFF

/* Cyclone V FPGA devices */
#define LED_BASE			0xFF200000
#define LEDR_BASE			0xFF200000
#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define SW_BASE				0xFF200040
#define KEY_BASE			0xFF200050
#define JP1_BASE			0xFF200060
#define JP2_BASE			0xFF200070
#define PS2_BASE			0xFF200100
#define PS2_DUAL_BASE			0xFF200108
#define JTAG_UART_BASE			0xFF201000
#define IrDA_BASE			0xFF201020
#define TIMER_BASE			0xFF202000
#define TIMER_2_BASE			0xFF202020
#define AV_CONFIG_BASE			0xFF203000
#define RGB_RESAMPLER_BASE    		0xFF203010
#define PIXEL_BUF_CTRL_BASE		0xFF203020
#define CHAR_BUF_CTRL_BASE		0xFF203030
#define AUDIO_BASE			0xFF203040
#define VIDEO_IN_BASE			0xFF203060
#define EDGE_DETECT_CTRL_BASE		0xFF203070
#define ADC_BASE			0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE			0xFF709000
#define I2C0_BASE			0xFFC04000
#define I2C1_BASE			0xFFC05000
#define I2C2_BASE			0xFFC06000
#define I2C3_BASE			0xFFC07000
#define HPS_TIMER0_BASE			0xFFC08000
#define HPS_TIMER1_BASE			0xFFC09000
#define HPS_TIMER2_BASE			0xFFD00000
#define HPS_TIMER3_BASE			0xFFD01000
#define FPGA_BRIDGE			0xFFD0501C

#endif

//#include "address_map_nios2.h"
#include <string.h>

//globals
struct fb_t {
    unsigned short volatile pixels[256][512];
};
struct fb_t *const fbp = ((struct fb_t *) 0x8000000);//global variable for frame buffer
//screen sizes
int xn = 320;
int yn = 240;
//player "object"
struct player {
    //string for name?
    int xCoord;
    int yCoord;
    short playerSprite[256];//16 by 16 sprites s
};

struct player playerOne;
struct player PlayerTwo;


//sprites: 16 by 16 RRGGGBB as integers
short sans[] = {
	65535,65535,65535,65535,31727,59196,65535,65535,65535,65535,63454,54970,50744,65535,65535,65535,
	65535,65535,65535,52889,55002,55002,57051,65535,65535,61341,54970,61277,38034,65535,65535,65535,
	65535,65535,65535,44373,65535,8452,10565,65535,63454,46486,0,40147,35921,65503,65535,65535,
	65535,65535,65535,59196,48599,35921,35953,46518,14823,50744,31695,50744,40147,65535,65535,65535,
	65535,65535,65535,48599,40211,25356,44373,40147,38034,42292,38034,40179,16936,65503,65535,65535,
	65535,65535,65535,50744,52825,40147,25356,21130,16904,16872,25388,50776,38034,65535,65535,65535,
	65535,65535,65535,54970,0,21130,33808,38034,38034,38034,27469,10533,21130,65503,65535,65535,
	65535,65535,52857,10862,10565,16937,23275,31727,14791,33808,12678,23276,8683,21163,65503,65535,
	65535,65503,27569,15123,10796,27502,14791,27501,33808,21130,21163,19117,10863,13042,52858,65535,
	65535,59228,13042,17302,12976,10895,12777,59196,59164,35953,10829,8716,15188,17400,25455,65535,
	65535,65535,31859,17334,13041,13042,10697,50712,59196,27501,13009,8716,17399,17235,54970,65535,
	65535,65535,65535,25388,4358,17301,4391,0,0,0,13074,8618,4358,57051,65535,65535,
	65535,65535,65535,61309,0,33,0,0,0,0,33,0,29614,65535,65535,65535,
	65535,65535,65535,52857,0,0,0,21130,52889,4226,0,0,14791,65535,65535,65535,
	65535,65535,65503,35953,25356,46486,33808,54938,65535,25356,44373,44405,25324,50744,65535,65535,
	65535,65535,65535,40179,27501,27501,46518,63422,65535,55002,35953,27501,27501,57115,65535,65535,
};


/* function prototypes */
void HEX_PS2(char, char, char);
void LED_PS2(char b3);
void HANDLE_KEYBOARD_INPUTS(char b3);
void solid_color(struct fb_t *const fbp, unsigned short color);
void sprite_draw(struct fb_t *const fbp, unsigned short sprite[], int spriteWidth, int spriteHeight, int xCord, int yCord);
void W_KEY_PRESSED();
void A_KEY_PRESSED();
void S_KEY_PRESSED();
void D_KEY_PRESSED();




/*******************************************************************************
 * This program demonstrates use of the PS/2 port by displaying the last three
 * bytes of data received from the PS/2 port on the HEX displays.
 ******************************************************************************/
int main(void) {

    //player attributes
    playerOne.xCoord = 100;//starting position
    playerOne.yCoord = 100;
    memcpy(playerOne.playerSprite, sans, sizeof(sans)); // Copy the sans sprite data

    solid_color(fbp, 0xffff);//all white
    sprite_draw(fbp, playerOne.playerSprite, 16, 16, playerOne.xCoord, playerOne.yCoord);

    //keyboard polling
    unsigned char byte1 = 0;
	unsigned char byte2 = 0;
	unsigned char byte3 = 0;
	
  	volatile int * PS2_ptr = (int *) 0xFF200100;  // PS/2 port address
	volatile int * RLEDs = (int *) 0xFF200000;     // Red LEDs base address

	int PS2_data, RVALID;


	while (1) {
		PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
		RVALID = (PS2_data & 0x8000);	// extract the RVALID field
		if (RVALID != 0)
		{
			/* always save the last three bytes received */
			byte1 = byte2;
			byte2 = byte3;
			byte3 = PS2_data & 0xFF;
		}

        HEX_PS2(byte1, byte2, byte3);

		// Check if a key is pressed (not released)
		if (byte2 == 0xF0 && byte3 != 0x00) {
			// If a key is pressed, turn on the LEDs
			*RLEDs = 0x00; // Turn on all LEDs
		} else if (byte2 != 0xF0 && byte3 == 0x1D) {//W
            W_KEY_PRESSED();
			*RLEDs = 0b1000000000; 
		} else if (byte2 != 0xF0 && byte3 == 0x1C) {//A
            A_KEY_PRESSED();
			*RLEDs = 0b0100000000; 
		} else if (byte2 != 0xF0 && byte3 == 0x1B) {//S
            S_KEY_PRESSED();
			*RLEDs = 0b0010000000; 
		} else if (byte2 != 0xF0 && byte3 == 0x23) {//D
            D_KEY_PRESSED();
			*RLEDs = 0b0001000000; 
		} else {
			// Otherwise, turn off the LEDs
			*RLEDs = 0b1; // Turn off all LEDs
		}
	}
}

void solid_color(struct fb_t *const fbp, unsigned short color) {
    int x, y;
    for (x = 0; x < xn; x++)
    for (y = 0; y < yn; y++)
    fbp->pixels[y][x] = color; // set pixel value at x,y
}

// assumes 16x16 sprite
void sprite_draw(struct fb_t *const fbp, unsigned short sprite[], int spriteWidth, int spriteHeight, int xCord, int yCord) {
    int xi, yi; //position for plotting pixles

    //sprite is 16 by 16
    for (int sxi = 0; sxi < spriteWidth; sxi++)
        for (int syi = 0; syi < spriteHeight; syi++) {
        xi = xCord + sxi;
        yi = yCord + syi;
        fbp->pixels[yi][xi] = sprite[syi*16+sxi];
        }
}


void LED_PS2(char b) {

    //cc. reads the bit of the controller and outputs the LED
    volatile int * LED_ptr = (int *)LED_BASE; //cc. LED_ptr

    if (b == 29) {

        *(LED_ptr) = 0b1000000000; //cc. wasd - move up

    } else if (b == 28) {

        *(LED_ptr) = 0b0100000000; //cc. wasd - move left

    } else if (b == 27) {

        *(LED_ptr) = 0b0010000000; //cc. wasd - move down
        //*(LED_ptr) = 0b0001000000; //cc. wasd - move right

    } else if (b == 35) {

        *(LED_ptr) = 0b0001000000; //cc. wasd - move right
        //*(LED_ptr) = 0b0010000000; //cc. wasd - move down

    } else {

        *(LED_ptr) = 0b0; //cc. default zero

    }
}

void W_KEY_PRESSED(){// up
    //update player 1 position
    playerOne.xCoord -= 10;
    //redraw bg
    solid_color(fbp, 0xffff);//all white;
    //redraw sprite
    sprite_draw(fbp, playerOne.playerSprite , 16, 16, playerOne.xCoord, playerOne.yCoord);
}
void S_KEY_PRESSED(){//down
    //update player 1 position
    playerOne.yCoord += 10;
    //redraw bg
    solid_color(fbp, 0xffff);//all white
    //redraw sprite
    sprite_draw(fbp, playerOne.playerSprite , 16, 16, playerOne.xCoord, playerOne.yCoord);
}
void A_KEY_PRESSED(){
    //update player 1 position
    playerOne.xCoord -= 10;
    //redraw bg
    solid_color(fbp, 0xffff);//all white
    //redraw sprite
    sprite_draw(fbp, playerOne.playerSprite , 16, 16, playerOne.xCoord, playerOne.yCoord);
}
void D_KEY_PRESSED(){
    //update player 1 position
    playerOne.xCoord += 10;
    //redraw bg
    solid_color(fbp, 0xffff);//all white
    //redraw sprite
    sprite_draw(fbp, playerOne.playerSprite , 16, 16, playerOne.xCoord, playerOne.yCoord);
}

void HANDLE_KEYBOARD_INPUTS(char b) {

    //cc. reads the bit of the controller and outputs the LED
    volatile int * LED_ptr = (int *)LED_BASE; //cc. LED_ptr

    if (b == 29) {

        W_KEY_PRESSED(); //cc. wasd - move up

    } else if (b == 28) {

        A_KEY_PRESSED(); //cc. wasd - move left

    } else if (b == 27) {

        S_KEY_PRESSED(); //cc. wasd - move down

    } else if (b == 35) {

        D_KEY_PRESSED(); //cc. wasd - move right

    }
}

/****************************************************************************************
 * Subroutine to show a string of HEX data on the HEX displays
****************************************************************************************/
void HEX_PS2(char b1, char b2, char b3) {
    volatile int * HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
    volatile int * HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

    /* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
     * a single 7-seg display in the DE1-SoC Computer, for the hex digits 0 - F
     */
    unsigned char seven_seg_decode_table[] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
        0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    unsigned char hex_segs[] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int  shift_buffer, nibble;
    unsigned char code;
    int           i;

    shift_buffer = (b1 << 16) | (b2 << 8) | b3;
    for (i = 0; i < 6; ++i) {
        nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
        code   = seven_seg_decode_table[nibble];
        hex_segs[i]  = code;
        shift_buffer = shift_buffer >> 4;
    }
    /* drive the hex displays */
    *(HEX3_HEX0_ptr) = *(int *)(hex_segs);
    *(HEX5_HEX4_ptr) = *(int *)(hex_segs + 4);
}
