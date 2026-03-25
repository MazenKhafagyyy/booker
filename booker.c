#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>     // malloc() free()
#include "DEV_Config.h"
#include "EPD_2in7_V2.h"
#include "GUI_Paint.h"
#include "fonts.h"

#define TEST_FONT Font20

int EPD_2IN7_V2_test(void) {
    printf("EPD_2IN7_V2_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
	EPD_2IN7_V2_Init();
    EPD_2IN7_V2_Clear();


    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);
	Paint_Clear(WHITE);
	
#if 0   // show bmp
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in7);
    EPD_2IN7_V2_Display(BlackImage);
    DEV_Delay_ms(500);

#endif

#if 1  // Drawing on the image
	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);

    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    EPD_2IN7_V2_Display_Base(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 1  // Fast Drawing on the image
    // Fast refresh
    printf("This is followed by a quick refresh demo\r\n");
    printf("First, clear the screen\r\n");
    EPD_2IN7_V2_Init();
    EPD_2IN7_V2_Clear();

    printf("e-Paper Init Fast\r\n");
    EPD_2IN7_V2_Init_Fast();
	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // printf("show window BMP-----------------\r\n");
    // Paint_SelectImage(BlackImage);
    // Paint_Clear(WHITE);
    // // Paint_DrawBitMap(gImage_2in7);
    // EPD_2IN7_V2_Display_Fast(BlackImage);
    // DEV_Delay_ms(500);

    // 2.Drawing on the image
    Paint_Clear(WHITE);
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);

    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    EPD_2IN7_V2_Display_Fast(BlackImage);
    DEV_Delay_ms(3000);

#endif

#if 1   //Partial refresh, example shows time    	
    // If you didn't use the EPD_2IN7_V2_Display_Base() function to refresh the image before,
    // use the EPD_2IN7_V2_Display_Base_color() function to refresh the background color, 
    // otherwise the background color will be garbled 
    EPD_2IN7_V2_Init();
    // EPD_2IN7_V2_Display_Base_color(WHITE);
	Paint_NewImage(BlackImage, 50, 120, 90, WHITE);
    
    printf("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);
	Paint_SetScale(2);
    Paint_Clear(WHITE);
    
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 150;
    for (;;) {
        sPaint_time.Sec = sPaint_time.Sec + 1;
        if (sPaint_time.Sec == 60) {
            sPaint_time.Min = sPaint_time.Min + 1;
            sPaint_time.Sec = 0;
            if (sPaint_time.Min == 60) {
                sPaint_time.Hour =  sPaint_time.Hour + 1;
                sPaint_time.Min = 0;
                if (sPaint_time.Hour == 24) {
                    sPaint_time.Hour = 0;
                    sPaint_time.Min = 0;
                    sPaint_time.Sec = 0;
                }
            }
        }
        
        Paint_Clear(WHITE);
		Paint_DrawRectangle(1, 1, 120, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawTime(10, 15, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }
		printf("Part refresh...\r\n");
        EPD_2IN7_V2_Display_Partial(BlackImage, 60, 134, 110, 254); // Xstart must be a multiple of 8
        DEV_Delay_ms(500);
    }
#endif

#if 1 // show image for array
    free(BlackImage);
    printf("show Gray------------------------\r\n");
    Imagesize = ((EPD_2IN7_V2_WIDTH % 4 == 0)? (EPD_2IN7_V2_WIDTH / 4 ): (EPD_2IN7_V2_WIDTH / 4 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    EPD_2IN7_V2_Init_4GRAY();
    printf("4 grayscale display\r\n");
    Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);
    Paint_SetScale(4);
    Paint_Clear(0xff);
    
    Paint_DrawPoint(10, 80, GRAY4, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, GRAY4, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, GRAY4, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, GRAY2, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, GRAY4, GRAY1);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, GRAY3, GRAY1);
    Paint_DrawNum(10, 33, 123456789, &Font12, GRAY4, GRAY2);
    Paint_DrawNum(10, 50, 987654321, &Font16, GRAY1, GRAY4);
     Paint_DrawString_CN(150, 0,"���abc", &Font12CN, GRAY4, GRAY1);
    Paint_DrawString_CN(150, 20,"���abc", &Font12CN, GRAY3, GRAY2);
    Paint_DrawString_CN(150, 40,"���abc", &Font12CN, GRAY2, GRAY3);
    Paint_DrawString_CN(150, 60,"���abc", &Font12CN, GRAY1, GRAY4);
    Paint_DrawString_CN(10, 130, "΢ѩ����", &Font24CN, GRAY1, GRAY4);
    EPD_2IN7_V2_4GrayDisplay(BlackImage);
    DEV_Delay_ms(3000);

    Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 0, WHITE);
    Paint_SetScale(4);
    Paint_Clear(WHITE);
    // EPD_2IN7_V2_4GrayDisplay(gImage_2in7_4Gray);
    DEV_Delay_ms(3000);

#endif

#if 1

    EPD_2IN7_V2_Init();
    EPD_2IN7_V2_Clear();
    printf("Goto Sleep...\r\n");
    EPD_2IN7_V2_Sleep();

    int key=0; 

    gpio_set_dir(KEY0, GPIO_IN);
    gpio_pull_up(KEY0);//Need to pull up

    gpio_set_dir(KEY1, GPIO_IN);
    gpio_pull_up(KEY1);//Need to pull up

    gpio_set_dir(KEY2, GPIO_IN);
    gpio_pull_up(KEY2);//Need to pull up
    while(1)
    {
        if(DEV_Digital_Read(KEY0 ) == 0 && key==0)
        {
            key=1;
            EPD_2IN7_V2_Init();
            // Paint_DrawBitMap(gImage_2in7);
            EPD_2IN7_V2_Display(BlackImage);
            DEV_Delay_ms(3000);
        }

        if(DEV_Digital_Read(KEY1 ) == 0 && key==0)
        {
            key=1;
            EPD_2IN7_V2_Init_4GRAY();
            // EPD_2IN7_V2_4GrayDisplay(gImage_2in7_4Gray);
            DEV_Delay_ms(3000);
        }

        if(DEV_Digital_Read(KEY2 ) == 0 && key==0)
        {
            key=1;
            EPD_2IN7_V2_Init_4GRAY();
            // EPD_2IN7_V2_4GrayDisplay(gImage_2in7_4Gray_1);
            DEV_Delay_ms(3000);
        }

        if(DEV_Digital_Read(KEY0 ) == 1&&DEV_Digital_Read(KEY1 ) == 1&&DEV_Digital_Read(KEY2 ) == 1 && key == 1 )
        {
            key=0;
            EPD_2IN7_V2_Init();
            EPD_2IN7_V2_Clear();
            printf("Goto Sleep...\r\n");
            EPD_2IN7_V2_Sleep();
        }
            
    }

#endif

	printf("Clear...\r\n");
	EPD_2IN7_V2_Init();
    EPD_2IN7_V2_Clear();
	
    printf("Goto Sleep...\r\n");
    EPD_2IN7_V2_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    return 0;
}

const char* lorem_ipsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

int main(void)
{
    #if 1 // MY CODE
    printf("PUT SOME TEXT IN\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }
    sleep_ms(2000); // FOR THE SERIAL MONITOR TO BE READY

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN7_V2_Init();
    EPD_2IN7_V2_Clear();

    //Create a new image cache
    UBYTE *TextImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((TextImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for memory...\r\n");
        return -1;
    }

    Paint_NewImage(TextImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 0, WHITE);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(8, 8, lorem_ipsum, &Font12, WHITE, BLACK);

    EPD_2IN7_V2_Display(TextImage);
    DEV_Delay_ms(9000);

    // ---------- PARTIAL REFRESH DEMO ----------

    // Important: set a clean base image first for partial refresh
    EPD_2IN7_V2_Init();
    EPD_2IN7_V2_Display_Base(TextImage);

    // Define a small partial-update region
    #define PART_X      40    // must be multiple of 8
    #define PART_Y      80
    #define PART_W      96
    #define PART_H      80

    UBYTE *PartialImage;
    UWORD PartialImageSize = ((PART_W % 8 == 0) ? 
                            (PART_W / 8) : 
                            (PART_W / 8 + 1)) * PART_H;

    if((PartialImage = (UBYTE *)malloc(PartialImageSize)) == NULL) {
        printf("Failed to allocate partial buffer...\r\n");
        free(TextImage);
        return -1;
    }

    // Create an image buffer for ONLY the partial region
    Paint_NewImage(PartialImage, PART_W, PART_H, 0, WHITE);

    for (int i = 0; i < 5; i++) {
        Paint_SelectImage(PartialImage);
        Paint_Clear(WHITE);

        // draw border so you can see the region
        Paint_DrawRectangle(0, 0, PART_W - 1, PART_H - 1,
                            BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

        // draw i+1 horizontal lines
        for (int j = 0; j <= i; j++) {
            int y = 10 + j * 12;
            if (y < PART_H - 2) {
                Paint_DrawLine(8, y, PART_W - 8, y,
                            BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
            }
        }

        printf("Partial refresh step %d\r\n", i + 1);

        EPD_2IN7_V2_Display_Partial(
            PartialImage,
            PART_X,
            PART_Y,
            PART_X + PART_W,
            PART_Y + PART_H
        );

        DEV_Delay_ms(1000);
    }

    free(PartialImage);
    free(TextImage);

    EPD_2IN7_V2_Sleep();

    #endif

    // EPD_2IN7_V2_test();

    return 0;
}