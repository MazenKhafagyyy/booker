#include <stdio.h>
#include "pico/stdlib.h"

#include "DEV_Config.h"
#include "EPD_2in7_V2.h"
#include "GUI_Paint.h"
#include "fonts.h"

#define TEST_FONT Font20

int main(void)
{
    stdio_init_all();
    sleep_ms(2000);

    printf("Starting 2.7in V2 e-paper test...\n");

    if (DEV_Module_Init() != 0) {
        printf("DEV_Module_Init failed\n");
        while (1) sleep_ms(1000);
    }

    printf("Init display...\n");
    EPD_2IN7_V2_Init();

    printf("Clear display...\n");
    EPD_2IN7_V2_Clear();
    DEV_Delay_ms(500);

    static UBYTE image[EPD_2IN7_V2_WIDTH * EPD_2IN7_V2_HEIGHT / 8];

    Paint_NewImage(image, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 0, WHITE);
    Paint_Clear(WHITE);

    Paint_DrawRectangle(0, 0,
                        EPD_2IN7_V2_WIDTH - 1,
                        EPD_2IN7_V2_HEIGHT - 1,
                        BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

    Paint_DrawLine(0, 30, EPD_2IN7_V2_WIDTH - 1, 30,
                   BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawString_EN(8, 8, "Pico E-Paper Test", &TEST_FONT, BLACK, WHITE);
    Paint_DrawString_EN(8, 50, "Hello Mazen", &TEST_FONT, BLACK, WHITE);
    Paint_DrawString_EN(8, 90, "Using 2.7 V2 driver", &TEST_FONT, BLACK, WHITE);

    printf("Display image...\n");
    EPD_2IN7_V2_Display(image);

    printf("Done\n");

    // while (1) {
    //     sleep_ms(1000);
    // }
}