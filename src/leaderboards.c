#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "headers/globalConst.h"
#include "headers/leaderboards.h"
#include "headers/render.h"
#include "headers/player.h"

// #define SCREEN_WIDTH 640
// #define SCREEN_HEIGHT 480
// #define FONT_SIZE 24

#define TABLE_X 100
#define TABLE_Y 100
#define CELL_WIDTH 200
#define CELL_HEIGHT 50
#define NUM_COLUMNS 2
#define NUM_ROWS 4

#define LEADERBOARDS_Y 250

#define CELL_PADDING 50
#define FOOTER_BOTTOM_PADDING 200
#define RESULT_TOP_PADDING 100

void displayLeaderboard(SDL_Renderer * pRenderer, int * gameState, bool * quit, TTF_Font * font, Player players[], int *gameOver) {
    // printf("leader board is displayed.\n");

    // Define leaderboard data
    // const char* names[NUM_ROWS] = {"Player 1", "Player 2", "Player 3", "Player 4", "Player 5"};
    // const char* scores[NUM_ROWS] = {"100", "200", "150", "300", "250"};
    char* names[NUM_ROWS];
    char* scores[NUM_ROWS];

    // for (int i=0 ; i<NUM_ROWS ; i++ )
    // {
    //     // for (int j=0 ; j<NUM_COLUMNS ; j++ ) 
    //     // {
    //         char buffer1[8];
    //         char buffer2[8];
    //         sprintf(buffer1, "Player %d", players[i].id);
    //         sprintf(buffer2, "%d", players[i].numberOfPoints);

    //         names[i] = buffer1;
    //         scores[i] = buffer2;

            
    //     // }
    // }

    // int num_players = sizeof(names) / sizeof(names[0]);

    // Define table settings
    

    // Define table header
    const char* header[] = {"Name", "Score"};
    SDL_Color text_color = {255, 255, 255, 255};


    // Render backdrop
    renderMenuBackdrop(pRenderer);

    if ((*gameOver)) {
        SDL_Surface* result_surface;
        SDL_Texture* result_texture;
        if (players[0].isAlive)
        {
            // Display You won!
            result_surface = TTF_RenderText_Solid(font, "You Won", text_color);
            if (!result_surface) {
                printf("Failed to create surface: %s\n", SDL_GetError());
            }
            result_texture = SDL_CreateTextureFromSurface(pRenderer, result_surface);
            if (!result_texture) {
                printf("Failed to create texture: %s\n", SDL_GetError());
                SDL_FreeSurface(result_surface);
            }
        } else {
            // Display You Lost!
            result_surface = TTF_RenderText_Solid(font, "You Lost", text_color);
            if (!result_surface) {
                printf("Failed to create surface: %s\n", SDL_GetError());
            }
            result_texture = SDL_CreateTextureFromSurface(pRenderer, result_surface);
            if (!result_texture) {
                printf("Failed to create texture: %s\n", SDL_GetError());
                SDL_FreeSurface(result_surface);
            }
        }
        SDL_Rect result_rect;
        SDL_QueryTexture(result_texture, 0, 0, &result_rect.w, &result_rect.h);
        
        result_rect.x = WINDOW_WIDTH/2 - result_rect.w/2;
        result_rect.y = WINDOW_HEIGHT - (WINDOW_HEIGHT - RESULT_TOP_PADDING);

        SDL_RenderCopy(pRenderer, result_texture, NULL, &result_rect);
        SDL_FreeSurface(result_surface);
        SDL_DestroyTexture(result_texture);
    }


    // Render the table header
    SDL_Rect header_rects[NUM_COLUMNS];

    for (int i = 0; i < NUM_COLUMNS; i++) {
        SDL_Surface* header_surface = TTF_RenderText_Solid(font, header[i], text_color);
        if (!header_surface) {
            printf("Failed to create surface: %s\n", SDL_GetError());
            continue;
        }
        SDL_Texture* header_texture = SDL_CreateTextureFromSurface(pRenderer, header_surface);
        if (!header_texture) {
            printf("Failed to create texture: %s\n", SDL_GetError());
            SDL_FreeSurface(header_surface);
            continue;
        }
        SDL_QueryTexture(header_texture, 0, 0, &header_rects[i].w, &header_rects[i].h);

        if (i == 0)
        {
            header_rects[i].x = WINDOW_WIDTH/2 - header_rects[i].w - CELL_PADDING;
        } else if (i == 1) {
            header_rects[i].x = WINDOW_WIDTH/2 + CELL_PADDING;
        }
        header_rects[i].y = LEADERBOARDS_Y;

        // SDL_Rect header_rect = {WINDOW_WIDTH/2 - header_rects[i].w/2, TABLE_Y, CELL_WIDTH, CELL_HEIGHT};
        SDL_RenderCopy(pRenderer, header_texture, NULL, &header_rects[i]);
        // header_rects[i] = header_rect;
        SDL_FreeSurface(header_surface);
        SDL_DestroyTexture(header_texture);
    }

    // Render the table cells
    SDL_Rect cell_rects[NUM_ROWS][NUM_COLUMNS];
    char buffer1[8];
    char buffer2[8];
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            if (j == 0) {

                sprintf(buffer1, "Player %d", players[i].id);

                SDL_Surface* cell_surface = TTF_RenderText_Solid(font, buffer1, text_color);
                if (!cell_surface) {
                    printf("Failed to create surface: %s\n", SDL_GetError());
                    continue;
                }
                SDL_Texture* cell_texture = SDL_CreateTextureFromSurface(pRenderer, cell_surface);
                if (!cell_texture) {
                    printf("Failed to create texture: %s\n", SDL_GetError());
                    SDL_FreeSurface(cell_surface);
                    continue;
                }
                SDL_QueryTexture(cell_texture, 0, 0, &cell_rects[i][j].w, &cell_rects[i][j].h);

                cell_rects[i][j].x = WINDOW_WIDTH/2 - cell_rects[i]->w - CELL_PADDING;
                cell_rects[i][j].y = ((i+1) * (cell_rects[i][j].h + CELL_PADDING)) + LEADERBOARDS_Y;

                SDL_RenderCopy(pRenderer, cell_texture, NULL, &cell_rects[i][j]);
                SDL_FreeSurface(cell_surface);
                SDL_DestroyTexture(cell_texture);
                // SDL_Rect cell_rect = {TABLE_X + j * CELL_WIDTH, TABLE_Y + (i + 1) * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT};
            } else if(j == 1) {
                sprintf(buffer2, "%d", players[i].numberOfPoints);


                SDL_Surface* cell_surface = TTF_RenderText_Solid(font, buffer2, text_color);
                if (!cell_surface) {
                    printf("Failed to create surface: %s\n", SDL_GetError());
                    continue;
                }
                SDL_Texture* cell_texture = SDL_CreateTextureFromSurface(pRenderer, cell_surface);
                if (!cell_texture) {
                    printf("Failed to create texture: %s\n", SDL_GetError());
                    SDL_FreeSurface(cell_surface);
                    continue;
                }

                SDL_QueryTexture(cell_texture, 0, 0, &cell_rects[i][j].w, &cell_rects[i][j].h);

                cell_rects[i][j].x = WINDOW_WIDTH/2 + CELL_PADDING;
                cell_rects[i][j].y = ((i+1) * (cell_rects[i][j].h + CELL_PADDING)) + LEADERBOARDS_Y;

                SDL_RenderCopy(pRenderer, cell_texture, NULL, &cell_rects[i][j]);
                SDL_FreeSurface(cell_surface);
                SDL_DestroyTexture(cell_texture);

            }
            
        }   
    }

    // Footer
    SDL_Surface* footer_surface = TTF_RenderText_Solid(font, "Press Return to continue...", text_color);
    if (!footer_surface) {
        printf("Failed to create surface: %s\n", SDL_GetError());
    }
    SDL_Texture* footer_texture = SDL_CreateTextureFromSurface(pRenderer, footer_surface);
    if (!footer_texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(footer_surface);
    }

    SDL_Rect footer_rect;
    SDL_QueryTexture(footer_texture, 0, 0, &footer_rect.w, &footer_rect.h);
    
    footer_rect.w *= 0.7;
    footer_rect.h *= 0.7;
    footer_rect.x = WINDOW_WIDTH/2 - footer_rect.w/2;
    footer_rect.y = WINDOW_HEIGHT - FOOTER_BOTTOM_PADDING;

    SDL_RenderCopy(pRenderer, footer_texture, NULL, &footer_rect);
    SDL_FreeSurface(footer_surface);
    SDL_DestroyTexture(footer_texture);


    SDL_Event event;

    while ((*gameState)==5 && !(*quit)) {
        SDL_PollEvent(&event);

        *quit = event.type == SDL_QUIT ? true : false;
        
        switch (event.key.keysym.sym)
        {
            case SDLK_RETURN:
                (*gameState) = (*gameOver) ? 1 : 3;
                break;
            default:
                break;
        }
        SDL_RenderPresent(pRenderer);
    }

}

// // Clean up resources
// TTF_CloseFont(font);
// SDL_DestroypRenderer(pRenderer);
// SDL_DestroyWindow(window);
// TTF_Quit();
// SDL_Quit();

