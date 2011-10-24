#include <stdint.h>
#include <SDL/SDL.h>
#include "cpu_chip8.h"
#include "graphic.h"

pixel		gl_video[NB_PIXEL_WIDTH][NB_PIXEL_HEIGHT];
SDL_Surface*	gl_screen = NULL;
SDL_Surface*	gl_square[2] = {NULL};
SDL_Event	gl_event;

void		init_pixel(void)
{
  int		i = 0;
  int		j = 0;

  while (i < NB_PIXEL_WIDTH) {
    j = 0;
    while (j < NB_PIXEL_HEIGHT) {
      gl_video[i][j].pos.x = i * PIXEL_SIZE;
      gl_video[i][j].pos.y = j * PIXEL_SIZE;
      gl_video[i][j].color = 0;
      ++j;
    }
    ++i;
  }
}

void		init_screen(void)
{
  gl_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Chip8 Emulator - Thibault MEYER", NULL);
  if (!gl_screen) {
    printf("chip8emu::SDL_SetVideoMode::Unable to create SDL surface\n");
    exit(-2);
  }

  gl_square[0] = SDL_CreateRGBSurface(SDL_HWSURFACE, PIXEL_SIZE, PIXEL_SIZE, 32, 0, 0, 0, 0);
  if (!gl_square[0]) {
    printf("chip8emu::SDL_SetVideoMode::Unable to create RGB pixel\n");
    exit(-2);
  }
  SDL_FillRect(gl_square[0], NULL, SDL_MapRGB(gl_square[0]->format, 0x00, 0x00, 0x00));

  gl_square[1] = SDL_CreateRGBSurface(SDL_HWSURFACE, PIXEL_SIZE, PIXEL_SIZE, 32, 0, 0, 0, 0);
  if (!gl_square[1]) {
    printf("chip8emu::SDL_SetVideoMode::Unable to create RGB pixel\n");
    exit(-2);
  }
  SDL_FillRect(gl_square[1], NULL, SDL_MapRGB(gl_square[1]->format, 0xFF, 0xFF, 0xFF));
}

void		clean_screen(void)
{
  int		i = 0;
  int		j = 0;

  while (i < NB_PIXEL_WIDTH) {
    while (j < NB_PIXEL_HEIGHT) {
      gl_video[i][j].color = 0;
      ++j;
    }
    ++i;
  }
  SDL_FillRect(gl_screen, NULL, 0);
}

void		update_screen(void)
{
  int		i = 0;
  int		j = 0;

  while (i < NB_PIXEL_WIDTH) {
    j = 0;
    while (j < NB_PIXEL_HEIGHT) {
      SDL_BlitSurface(gl_square[gl_video[i][j].color], NULL, gl_screen, &gl_video[i][j].pos);
      ++j;
    }
    ++i;
  }
  SDL_Flip(gl_screen);
}

void		init_sdl(void)
{
  atexit(exit_sdl);

  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    printf("chip8emu::SDL_Init::Error\n");
    exit(-2);
  }
}

void		draw_pixel(uint8_t b1, uint8_t b2, uint8_t b3, chip8* cpu)
{
  uint8_t	x = 0;
  uint8_t	y = 0;
  uint8_t	k = 0;
  uint8_t	j = 0;
  uint8_t	codage = 0;
  uint8_t	decalage = 0;

  cpu->reg[0xF] = 0x00;
  for (k = 0; k < b1; ++k) {
    codage = cpu->memory[cpu->i + k];
    y = (cpu->reg[b2] + k) % NB_PIXEL_HEIGHT;
    for (j = 0, decalage = 7; j < 8; ++j, --decalage) {
      x = (cpu->reg[b3] + j) % NB_PIXEL_WIDTH;
      if (((codage) & (0x1 << decalage)) != 0) {
	if (gl_video[x][y].color == 1) {
	  gl_video[x][y].color = 0;
	  cpu->reg[0xF] = 1;
	} 
	else
	  gl_video[x][y].color = 1;
      }
    }
  }
}

void		exit_sdl(void)
{
  printf("[ chip8emu ]  Exiting...\n");
  SDL_FreeSurface(gl_square[0]);
  SDL_FreeSurface(gl_square[1]);
  SDL_FreeSurface(gl_screen);
  SDL_Quit();
}

void		pause_sdl(void)
{
  int		pause = 1;

  printf("[ chip8emu ]  Pause\n");
  while (pause == 1) {
    SDL_WaitEvent(&gl_event);
    switch (gl_event.type) {
    case SDL_QUIT:
      pause = 0;
      break;
    case SDL_KEYDOWN:
      pause = 0;
      break;
    }
  }
  printf("[ chip8emu ]  Play\n");
}
