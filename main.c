#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL/SDL.h>
#include "cpu_chip8.h"
#include "graphic.h"
#include "internal_rom.h"

chip8*	gl_cpu = NULL;

int	main(int argc, char** argv)
{
  int	emu_working = 1;
  int	emu_reset = 0;

  printf("Chip 8 Emulator by MEYER Thibault\n");
  printf("==================================\n");
  printf("[ chip8emu ]  Initializing Chip8 CPU\n");
  gl_cpu = (chip8*)malloc(sizeof(chip8) + 1);
  if (!gl_cpu) {
    printf("chip8emu::main::Malloc cpu error");
    exit(-2);
  }
  init_chip8(gl_cpu);

  if (argc >= 2) {
    printf("[ chip8emu ]  Loading Chip8 Rom from '%s'\n", argv[1]);
    load_rom(gl_cpu, argv[1]);
  } else {
    printf("[ chip8emu ]  Loading Internal Chip8 Rom\n");
    memcpy(&gl_cpu->memory[MEMORY_ROM_START], chip8_internal, C8_INTERNAL_SIZE);
  }
  printf("[ chip8emu ]  Initializing SDL\n");
  init_sdl();
  init_pixel();
  init_screen();

  printf("[ chip8emu ]  Ready to work !\n");
  while (emu_working == 1) {
    SDL_PollEvent(&gl_event);
    switch (gl_event.type) {
    case SDL_QUIT:
      emu_working = 0;
      break;
    case SDL_KEYDOWN:
      switch (gl_event.key.keysym.sym) {
      case SDLK_ESCAPE:
	emu_working = 0;
	break;
      case SDLK_a:
	gl_cpu->keyboard[0x0] = 1;
	break;
      case SDLK_z:
	gl_cpu->keyboard[0x1] = 1;
	break;
      case SDLK_e:
	gl_cpu->keyboard[0x2] = 1;
	break;
      case SDLK_q:
	gl_cpu->keyboard[0x3] = 1;
	break;
      case SDLK_s:
	gl_cpu->keyboard[0x4] = 1;
	break;
      case SDLK_d:
	gl_cpu->keyboard[0x5] = 1;
	break;
      case SDLK_w:
	gl_cpu->keyboard[0x6] = 1;
	break;
      case SDLK_x:
	gl_cpu->keyboard[0x7] = 1;
	break;
      case SDLK_c:
	gl_cpu->keyboard[0x8] = 1;
	break;
      case SDLK_r:
	emu_reset = 1;
	break;
      }
      break;
    case SDL_KEYUP:
      switch (gl_event.key.keysym.sym) {
      case SDLK_a:
	gl_cpu->keyboard[0x0] = 0;
	break;
      case SDLK_z:
	gl_cpu->keyboard[0x1] = 0;
	break;
      case SDLK_e:
	gl_cpu->keyboard[0x2] = 0;
	break;
      case SDLK_q:
	gl_cpu->keyboard[0x3] = 0;
	break;
      case SDLK_s:
	gl_cpu->keyboard[0x4] = 0;
	break;
      case SDLK_d:
	gl_cpu->keyboard[0x5] = 0;
	break;
      case SDLK_w:
	gl_cpu->keyboard[0x6] = 0;
	break;
      case SDLK_x:
	gl_cpu->keyboard[0x7] = 0;
	break;
      case SDLK_c:
	gl_cpu->keyboard[0x8] = 0;
	break;
      case SDLK_r:
	if (emu_reset == 1) {
	  emu_reset = 0;
	  printf("[ chip8emu ]  Reset chip8 !\n");
	  reset_chip8(gl_cpu);
	  clean_screen();
	  update_screen();
	  init_pixel();
	}
	break;
      }
      break;
    }
    cpu_next(gl_cpu);
    update_counter(gl_cpu);
    update_screen();
    SDL_Delay(MAX_FPS);
  }

  return (0);
}
