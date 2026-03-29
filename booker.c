#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>     // malloc() free()
#include <string.h>     // memset()
#include "DEV_Config.h"
#include "EPD_2in7_V2.h"
#include "GUI_Paint.h"
#include "fonts.h"

#include "book_data.h"

#define PIN_D1     21   // LED
#define PIN_D2     20   // LED
#define PIN_K1     18   // BUTTON
#define PIN_K2     17   // BUTTON

#define BOOK_FONT        Font12
#define SCREEN_W         EPD_2IN7_V2_WIDTH
#define SCREEN_H         EPD_2IN7_V2_HEIGHT
#define CHAR_W           BOOK_FONT.Width
#define CHAR_H           BOOK_FONT.Height
#define FOOTER_LINES     1
#define MAX_TOTAL_LINES  (SCREEN_H / CHAR_H)
#define CHARS_PER_LINE   (SCREEN_W / CHAR_W)
#define MAX_PAGE_LINES   (MAX_TOTAL_LINES - FOOTER_LINES)
#define FOOTER_LINE_Y    (MAX_PAGE_LINES * CHAR_H)
#define CHARS_PER_PAGE   (MAX_PAGE_LINES * (SCREEN_W / CHAR_W))

extern const char test[];
extern const int test_len;

char ** pages;
int num_pages;

// MAX CHARACTER COUNT: ??? (including spaces and punctuation)
// Dimensions for font12: 25x24 characters
// THATS FOR FULL SCREEN, SHOULD BE LESS AFTER ADDING PAGE COUNTER.
const char* lorem_ipsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.  EXTRA: Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt abcdef";

int generate_pages(const uint8_t* text, size_t text_len) {
    if (text == NULL || text_len == 0) {
        pages = NULL;
        num_pages = 0;
        return 0;
    }

    int max_pages = (int)(text_len / 16) + 4;
    pages = (char**)malloc(max_pages * sizeof(char*));
    if (pages == NULL) {
        num_pages = 0;
        return -1;
    }

    size_t i = 0;
    num_pages = 0;

    while (i < text_len) {
        size_t page_cap = (MAX_PAGE_LINES * CHARS_PER_LINE) + MAX_PAGE_LINES + 1;
        char *page = (char*)malloc(page_cap);
        if (page == NULL) {
            for (int k = 0; k < num_pages; k++) {
                free(pages[k]);
            }
            free(pages);
            pages = NULL;
            num_pages = 0;
            return -1;
        }

        size_t out = 0;
        int lines_used = 0;

        while (lines_used < MAX_PAGE_LINES && i < text_len) {
            char line_buf[CHARS_PER_LINE + 1];
            int line_len = 0;

            // Skip leading spaces at start of line
            while (i < text_len && text[i] == ' ') {
                i++;
            }

            // Paragraph break at start of line: only if 2+ newlines in a row
            if (i < text_len && text[i] == '\n') {
                size_t j = i;
                int newline_count = 0;

                while (j < text_len && text[j] == '\n') {
                    newline_count++;
                    j++;
                }

                if (newline_count >= 2) {
                    i = j;
                    page[out++] = '\n';
                    lines_used++;
                    continue;
                } else {
                    // Single newline acts like a space
                    i = j;
                }
            }

            while (i < text_len) {
                // Collapse spaces before the next word
                while (i < text_len && text[i] == ' ') {
                    i++;
                }

                if (i >= text_len) {
                    break;
                }

                // Check newline run before reading a word
                if (text[i] == '\n') {
                    size_t j = i;
                    int newline_count = 0;

                    while (j < text_len && text[j] == '\n') {
                        newline_count++;
                        j++;
                    }

                    if (newline_count >= 2) {
                        // Real paragraph break: end current line
                        break;
                    } else {
                        // Single newline: treat as a space
                        i = j;
                        if (line_len > 0 && line_len < CHARS_PER_LINE) {
                            line_buf[line_len++] = ' ';
                        }
                        continue;
                    }
                }

                size_t word_start = i;
                char word[CHARS_PER_LINE + 1];
                int word_len = 0;

                while (i < text_len && text[i] != ' ' && text[i] != '\n') {
                    uint8_t c = text[i];

                    // ASCII only for now
                    if (c < 32 || c > 126) {
                        c = ' ';
                    }

                    if (word_len < CHARS_PER_LINE) {
                        word[word_len++] = (char)c;
                    }
                    i++;
                }

                if (word_len == 0) {
                    continue;
                }

                int needed = (line_len == 0) ? word_len : (1 + word_len);

                if (line_len + needed <= CHARS_PER_LINE) {
                    if (line_len > 0) {
                        line_buf[line_len++] = ' ';
                    }
                    memcpy(&line_buf[line_len], word, word_len);
                    line_len += word_len;
                } else {
                    if (line_len == 0) {
                        // Very long word: hard cut only if it starts an empty line
                        int cut = (word_len > CHARS_PER_LINE) ? CHARS_PER_LINE : word_len;
                        memcpy(line_buf, word, cut);
                        line_len = cut;
                    } else {
                        // Move full word to next line
                        i = word_start;
                    }
                    break;
                }
            }

            // Trim trailing spaces
            while (line_len > 0 && line_buf[line_len - 1] == ' ') {
                line_len--;
            }

            if (line_len > 0) {
                memcpy(&page[out], line_buf, line_len);
                out += line_len;
            }

            lines_used++;

            // Look ahead for paragraph break
            if (i < text_len && text[i] == '\n') {
                size_t j = i;
                int newline_count = 0;

                while (j < text_len && text[j] == '\n') {
                    newline_count++;
                    j++;
                }

                if (newline_count >= 2) {
                    i = j;
                    if (lines_used < MAX_PAGE_LINES) {
                        page[out++] = '\n';
                        lines_used++;
                    }
                } else {
                    i = j;
                }
            }

            if (lines_used < MAX_PAGE_LINES && i < text_len) {
                page[out++] = '\n';
            }
        }

        while (out > 0 && (page[out - 1] == '\n' || page[out - 1] == ' ')) {
            out--;
        }

        page[out] = '\0';
        pages[num_pages++] = page;
    }

    return num_pages;
}

static void DrawPageTextLimited(UWORD x0, UWORD y0, const char *data, sFONT *font, UWORD bg, UWORD fg) {
    UWORD x = x0;
    UWORD y = y0;
    int lines_used = 1;

    for (size_t i = 0; data[i] != '\0'; i++) {
        char c = data[i];

        if (c == '\r') {
            continue;
        }

        if (c == '\n') {
            x = x0;
            y += font->Height;
            lines_used++;

            if (lines_used > MAX_PAGE_LINES) {
                break;
            }
            continue;
        }

        if ((x + font->Width) > SCREEN_W) {
            x = x0;
            y += font->Height;
            lines_used++;

            if (lines_used > MAX_PAGE_LINES) {
                break;
            }
        }

        Paint_DrawChar(x, y, c, font, bg, fg);
        x += font->Width;
    }
}

static void draw_page(UBYTE *image, int page_index) {
    char footer[32];

    Paint_SelectImage(image);
    Paint_Clear(WHITE);

    DrawPageTextLimited(0, 0, pages[page_index], &BOOK_FONT, BLACK, WHITE);

    // line above footer
    // Paint_DrawLine(0, FOOTER_LINE_Y - 2, SCREEN_W - 1, FOOTER_LINE_Y - 2,
    //                BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    snprintf(footer, sizeof(footer), "%d/%d", page_index + 1, num_pages);

    int footer_len = (int)strlen(footer);
    int footer_x = (SCREEN_W - footer_len * BOOK_FONT.Width) / 2;
    if (footer_x < 0) footer_x = 0;

    Paint_DrawString_EN((UWORD)footer_x, FOOTER_LINE_Y, footer, &BOOK_FONT, WHITE, BLACK);
}

static void free_pages(void) {
    if (pages != NULL) {
        for (int i = 0; i < num_pages; i++) {
            free(pages[i]);
        }
        free(pages);
        pages = NULL;
    }
    num_pages = 0;
}

int main(void)
{
    // INITIALIZE BUTTONS AND LEDS
    gpio_init(PIN_D1);
    gpio_set_dir(PIN_D1, GPIO_OUT);
    gpio_init(PIN_D2);
    gpio_set_dir(PIN_D2, GPIO_OUT);

    gpio_init(PIN_K1);
    gpio_set_dir(PIN_K1, GPIO_IN);
    gpio_pull_up(PIN_K1);

    gpio_init(PIN_K2);
    gpio_set_dir(PIN_K2, GPIO_IN);
    gpio_pull_up(PIN_K2);

    if(DEV_Module_Init()!=0){
        return -1;
    }
    sleep_ms(2000); // FOR THE SERIAL MONITOR TO BE READY

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN7_V2_Init();
    EPD_2IN7_V2_Clear();

    //Create a new image cache
    UBYTE *PageImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((PageImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for memory...\r\n");
        return -1;
    }

    Paint_NewImage(PageImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 0, WHITE);

    if (generate_pages(book_data, book_data_len) <= 0) {
        printf("Failed to generate pages\r\n");
        free(PageImage);
        return -1;
    }

    int current_page = 0;
    draw_page(PageImage, current_page);
    EPD_2IN7_V2_Display_Fast(PageImage);

    // Button handling loop
    while (1) {
        if (gpio_get(PIN_K2) == 0) {   // next page
            if (current_page < num_pages - 1) {
                current_page++;
                draw_page(PageImage, current_page);
                EPD_2IN7_V2_Display_Fast(PageImage);
                printf("Page %d/%d\r\n", current_page + 1, num_pages);
            }
            sleep_ms(200);
        }

        if (gpio_get(PIN_K1) == 0) {   // previous page
            if (current_page > 0) {
                current_page--;
                draw_page(PageImage, current_page);
                EPD_2IN7_V2_Display_Fast(PageImage);
                printf("Page %d/%d\r\n", current_page + 1, num_pages);
            }
            sleep_ms(200);
        }
    }

    // ---------- PARTIAL REFRESH DEMO ----------
    #if 0
    // Important: set a clean base image first for partial refresh
    EPD_2IN7_V2_Init();
    EPD_2IN7_V2_Display_Base(PageImage);

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
        free(PageImage);
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
    free(PageImage);
    
    #endif
    return 0;
}