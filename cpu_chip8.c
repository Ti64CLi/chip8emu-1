#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <SDL/SDL.h>
#include "cpu_chip8.h"
#include "graphic.h"

static void	init_font(chip8* cpu)
{
  if (cpu) {
    /*
    ** Memory from 0 to 512 contain 0-9 A-F
    **
    **  1 1 1 1 0 0 0 0    0xF0
    **  1 0 0 1 0 0 0 0    0x90
    **  1 0 0 1 0 0 0 0    0x90      0
    **  1 0 0 1 0 0 0 0    0x90
    **  1 1 1 1 0 0 0 0    0xF0
    */
    cpu->memory[0] = 0xF0; /* 0 */
    cpu->memory[1] = 0x90;
    cpu->memory[2] = 0x90;
    cpu->memory[3] = 0x90;
    cpu->memory[4] = 0xF0;

    cpu->memory[5] = 0x20; /* 1 */
    cpu->memory[6] = 0x60;
    cpu->memory[7] = 0x20;
    cpu->memory[8] = 0x20;
    cpu->memory[9] = 0x70;

    cpu->memory[10] = 0xF0; /* 2 */
    cpu->memory[11] = 0x10;
    cpu->memory[12] = 0xF0;
    cpu->memory[13] = 0x80;
    cpu->memory[14] = 0xF0;

    cpu->memory[15] = 0xF0; /* 3 */
    cpu->memory[16] = 0x10;
    cpu->memory[17] = 0x70;
    cpu->memory[18] = 0x10;
    cpu->memory[19] = 0xF0;

    cpu->memory[20] = 0x20; /* 4 */
    cpu->memory[21] = 0x40;
    cpu->memory[22] = 0xA0;
    cpu->memory[23] = 0xF0;
    cpu->memory[24] = 0x20;

    cpu->memory[25] = 0xF0; /* 5 */
    cpu->memory[26] = 0x80;
    cpu->memory[27] = 0xF0;
    cpu->memory[28] = 0x10;
    cpu->memory[29] = 0xF0;

    cpu->memory[30] = 0xF0; /* 6 */
    cpu->memory[31] = 0x80;
    cpu->memory[32] = 0xF0;
    cpu->memory[33] = 0x90;
    cpu->memory[34] = 0xF0;

    cpu->memory[35] = 0xF0; /* 7 */
    cpu->memory[36] = 0x20;
    cpu->memory[37] = 0x40;
    cpu->memory[38] = 0x40;
    cpu->memory[39] = 0x40;

    cpu->memory[40] = 0xF0; /* 8 */
    cpu->memory[41] = 0x90;
    cpu->memory[42] = 0xF0;
    cpu->memory[43] = 0x90;
    cpu->memory[44] = 0xF0;

    cpu->memory[45] = 0xF0; /* 9 */
    cpu->memory[46] = 0x90;
    cpu->memory[47] = 0xF0;
    cpu->memory[48] = 0x10;
    cpu->memory[49] = 0xF0;

    cpu->memory[50] = 0xF0; /* A */
    cpu->memory[51] = 0x90;
    cpu->memory[52] = 0xF0;
    cpu->memory[53] = 0x90;
    cpu->memory[54] = 0x90;

    cpu->memory[55] = 0xE0; /* B */
    cpu->memory[56] = 0x90;
    cpu->memory[57] = 0xE0;
    cpu->memory[58] = 0x90;
    cpu->memory[59] = 0xE0;

    cpu->memory[60] = 0xF0; /* C */
    cpu->memory[61] = 0x80;
    cpu->memory[62] = 0x80;
    cpu->memory[63] = 0x80;
    cpu->memory[64] = 0xF0;

    cpu->memory[65] = 0xE0; /* D */
    cpu->memory[66] = 0x90;
    cpu->memory[67] = 0x90;
    cpu->memory[68] = 0x90;
    cpu->memory[69] = 0xE0;

    cpu->memory[70] = 0xF0; /* E */
    cpu->memory[71] = 0x80;
    cpu->memory[72] = 0xF0;
    cpu->memory[73] = 0x80;
    cpu->memory[74] = 0xF0;

    cpu->memory[75] = 0xF0; /* F */
    cpu->memory[76] = 0x80;
    cpu->memory[77] = 0xF0;
    cpu->memory[78] = 0x80;
    cpu->memory[79] = 0x80;
  }
}

static void	init_chip8mask(void)
{
  gl_mask.mask[0] = 0x0000; /* 0NNN */
  gl_mask.id[0] = 0x0FFF;
  gl_mask.mask[1] = 0xFFFF; /* 00E0 */
  gl_mask.id[1] = 0x00E0;
  gl_mask.mask[2] = 0xFFFF; /* 00EE */
  gl_mask.id[2] = 0x00EE;
  gl_mask.mask[3] = 0xF000; /* 1NNN */
  gl_mask.id[3] = 0x1000;
  gl_mask.mask[4] = 0xF000; /* 2NNN */
  gl_mask.id[4] = 0x2000;
  gl_mask.mask[5] = 0xF000; /* 3XNN */
  gl_mask.id[5] = 0x3000;
  gl_mask.mask[6] = 0xF000; /* 4XNN */
  gl_mask.id[6] = 0x4000;
  gl_mask.mask[7] = 0xF00F; /* 5XY0 */
  gl_mask.id[7] = 0x5000;
  gl_mask.mask[8] = 0xF000; /* 6XNN */
  gl_mask.id[8] = 0x6000;
  gl_mask.mask[9] = 0xF000; /* 7XNN */
  gl_mask.id[9] = 0x7000;
  gl_mask.mask[10] = 0xF00F; /* 8XY0 */
  gl_mask.id[10] = 0x8000;
  gl_mask.mask[11] = 0xF00F; /* 8XY1 */
  gl_mask.id[11] = 0x8001;
  gl_mask.mask[12] = 0xF00F; /* 8XY2 */
  gl_mask.id[12] = 0x8002;
  gl_mask.mask[13] = 0xF00F; /* BXY3 */
  gl_mask.id[13] = 0x8003;
  gl_mask.mask[14] = 0xF00F; /* 8XY4 */
  gl_mask.id[14] = 0x8004;
  gl_mask.mask[15] = 0xF00F; /* 8XY5 */
  gl_mask.id[15] = 0x8005;
  gl_mask.mask[16] = 0xF00F; /* 8XY6 */
  gl_mask.id[16] = 0x8006;
  gl_mask.mask[17] = 0xF00F; /* 8XY7 */
  gl_mask.id[17] = 0x8007;
  gl_mask.mask[18] = 0xF00F; /* 8XYE */
  gl_mask.id[18] = 0x800E;
  gl_mask.mask[19] = 0xF00F; /* 9XY0 */
  gl_mask.id[19] = 0x9000;
  gl_mask.mask[20] = 0xF000; /* ANNN */
  gl_mask.id[20] = 0xA000;
  gl_mask.mask[21] = 0xF000; /* BNNN */
  gl_mask.id[21] = 0xB000;
  gl_mask.mask[22] = 0xF000; /* CXNN */
  gl_mask.id[22] = 0xC000;
  gl_mask.mask[23] = 0xF000; /* DXYN */
  gl_mask.id[23] = 0xD000;
  gl_mask.mask[24] = 0xF0FF; /* EX9E */
  gl_mask.id[24] = 0xE09E;
  gl_mask.mask[25] = 0xF0FF; /* EXA1 */
  gl_mask.id[25] = 0xE0A1;
  gl_mask.mask[26] = 0xF0FF; /* FX07 */
  gl_mask.id[26] = 0xF007;
  gl_mask.mask[27] = 0xF0FF; /* FX0A */
  gl_mask.id[27] = 0xF00A;
  gl_mask.mask[28] = 0xF0FF; /* FX15 */
  gl_mask.id[28] = 0xF015;
  gl_mask.mask[29] = 0xF0FF; /* FX18 */
  gl_mask.id[29] = 0xF018;
  gl_mask.mask[30] = 0xF0FF; /* FX1E */
  gl_mask.id[30] = 0xF01E;
  gl_mask.mask[31] = 0xF0FF; /* FX29 */
  gl_mask.id[31] = 0xF029;
  gl_mask.mask[32] = 0xF0FF; /* FX33 */
  gl_mask.id[32] = 0xF033;
  gl_mask.mask[33] = 0xF0FF; /* FX55 */
  gl_mask.id[33] = 0xF055;
  gl_mask.mask[34] = 0xF0FF; /* FX65 */
  gl_mask.id[34] = 0xF065;
}

void		init_chip8(chip8* cpu)
{
  if (cpu) {
    memset(cpu->memory, 0x00, C8_MEMORY_SIZE);
    memset(cpu->reg, 0x00, C8_NB_REGISTER);
    memset(cpu->mem_jump, 0x00, C8_MAX_JUMP);
    memset(cpu->keyboard, 0x00, KEYBOARD_MAX_KEY);
    cpu->i = 0;
    cpu->nb_jump = 0;
    cpu->counter_sync = 0;
    cpu->counter_sound = 0;
    cpu->pc = MEMORY_ROM_START;
    cpu->current_opcode = 0x00;
    init_font(cpu);
    init_chip8mask();
    srand(time(0));
  }
}

void		update_counter(chip8* cpu)
{
  if (cpu) {
    if (cpu->counter_sync > 0)
      --cpu->counter_sync;
    if (cpu->counter_sound > 0) {
      if (cpu->counter_sound == 1)
	printf("\a");
      --cpu->counter_sound;
    }
  }
}

void		load_rom(chip8* cpu, const char* filename)
{
  int		fd = 0;
  struct stat	nfo;

  nfo.st_size = 0;
  fd = open(filename, O_RDONLY);
  if (!fd) {
    printf("chip8emu::load_rom::Unable to Open file\n");
    exit(-1);
  }
  stat(filename, &nfo);
  if (S_ISDIR(nfo.st_mode)) {
    printf("chip8emu::load_rom::It is a directory. Buy you new fingers...\n");
    exit(-1);
  }
  if (nfo.st_size == 0) {
    printf("chip8emu::load_rom::File not found\n");
    exit(-1);
  }
  read(fd, &cpu->memory[MEMORY_ROM_START], nfo.st_size);
  close(fd);
}

void		cpu_next(chip8* cpu)
{
  uint8_t	offset = 0x00;
  uint8_t	i = 0;
  uint8_t	b1 = 0x00;
  uint8_t	b2 = 0x00;
  uint8_t	b3 = 0x00;

  if (cpu) {
    cpu->current_opcode = get_opcode(cpu);
    offset = get_offset_instruction(cpu->current_opcode);
    b3 = (uint8_t)((cpu->current_opcode & 0x0F00) >> 8);
    b2 = (uint8_t)((cpu->current_opcode & 0x00F0) >> 4);
    b1 = (uint8_t)(cpu->current_opcode & 0x000F);
    switch (offset) {
    case 0:
      /* 0NNN */
      cpu->pc = (b3 << 8) + (b2 << 4) + b1;
      cpu->pc -= 2;
      break;
    case 1:
      /* 00E0 */
      clean_screen();
      break;
    case 2:
      /* OOEE */
      if (cpu->nb_jump > 0) {
	--cpu->nb_jump;
	cpu->pc = cpu->mem_jump[cpu->nb_jump];
      }
      break;
    case 3:
      /* 1NNN */
      cpu->pc = (b3 << 8) + (b2 << 4) + b1;
      cpu->pc -= 2;
      break;
    case 4:
      /* 2NNN */
      cpu->mem_jump[cpu->nb_jump] = cpu->pc;
      if(cpu->nb_jump < 15)
	++cpu->nb_jump;
      cpu->pc = (b3 << 8) + (b2 << 4) + b1;
      cpu->pc -= 2;
      break;
    case 5:
      /* 3XNN */
      if (cpu->reg[b3] == ((b2 << 4) + b1))
	cpu->pc += 2;
      break;
    case 6:
      /* 4XNN */
      if(cpu->reg[b3] != ((b2 << 4) + b1))
	cpu->pc += 2;
      break;
    case 7:
      /* 5XY0 */
      if(cpu->reg[b3] == cpu->reg[b2])
	cpu->pc += 2;
      break;
    case 8:
      /* 6XNN */
      cpu->reg[b3] = (b2 << 4) + b1;
      break;
    case 9:
      /* 7XNN */
      cpu->reg[b3] += ((b2 << 4) + b1);
      break;
    case 10:
      /* 8XY0 */
      cpu->reg[b3] = cpu->reg[b2];
      break;
    case 11:
      /* 8XY1 */
      cpu->reg[b3] = cpu->reg[b3] | cpu->reg[b2];
      break;
    case 12:
      /* 8XY2 */
      cpu->reg[b3] = cpu->reg[b3] & cpu->reg[b2];
      break;
    case 13:
      /* BXY3 */
      cpu->reg[b3] = cpu->reg[b3] ^ cpu->reg[b2];
      break;
    case 14:
      /* 8XY4 */
      if ((cpu->reg[b3] + cpu->reg[b2]) > 0xFF)
	cpu->reg[0xF] = 1;
      else
	cpu->reg[0xF] = 0;
      cpu->reg[b3] += cpu->reg[b2];
      break;
    case 15:
      /* 8XY5 */
      if (cpu->reg[b3] < cpu->reg[b2])
	cpu->reg[0xF] = 0;
      else
	cpu->reg[0xF] = 1;
      cpu->reg[b3] -= cpu->reg[b2];
      break;
    case 16:
      /* 8XY6 */
      cpu->reg[0xF] = cpu->reg[b3] & 0x01;
      cpu->reg[b3] = cpu->reg[b3] >> 1;
      break;
    case 17:
      /* 8XY7 */
      if (cpu->reg[b2] < cpu->reg[b3])
	cpu->reg[0xF] = 0;
      else
	cpu->reg[0xF] = 1;
      cpu->reg[b3] = cpu->reg[b2] - cpu->reg[b3];
      break;
    case 18:
      /* 8XYE */
      cpu->reg[0xF] = cpu->reg[b3] >> 7;
      cpu->reg[b3] = cpu->reg[b3] << 1;
      break;
    case 19:
      /* 8XY0 */
      if (cpu->reg[b3] != cpu->reg[b2])
	cpu->pc += 2;
      break;
    case 20:
      /* ANNN */
      cpu->i = (b3 << 8) + (b2 << 4) + b1;
      break;
    case 21:
      /* BNNN */
      cpu->pc = (b3 << 8) + (b2 << 4) + b1 + cpu->reg[0];
      cpu->pc -= 2;
      break;
    case 22:
      /* CXNN */
      cpu->reg[b3] = rand() % ((b2 << 4) + b1 + 1);
      break;
    case 23:
      /* DXYN */
      draw_pixel(b1, b2, b3, cpu);
      break;
    case 24:
      /* EX9E */
      if (cpu->keyboard[cpu->reg[b3]] == 1)
	cpu->pc += 2;
      break;
    case 25:
      /* EXA1 */
      if (cpu->keyboard[cpu->reg[b3]] == 0)
	cpu->pc += 2;
      break;
    case 26:
      /* FX07 */
      cpu->reg[b3] = cpu->counter_sync;
      break;
    case 27:
      /* FX0A */
      break;
    case 28:
      /* FX15 */
      cpu->counter_sync = cpu->reg[b3];
      break;
    case 29:
      /* FX18 */
      cpu->counter_sound = cpu->reg[b3];
      break;
    case 30:
      /* FX1E */
      if ((cpu->i + cpu->reg[b3]) > 0xFFF)
	cpu->reg[0xF] =  1;
      else
	cpu->reg[0xF] = 0;
      cpu->i += cpu->reg[b3];
      break;
    case 31:
      /* FX29 */
      cpu->i = 5 * cpu->reg[b3];
      break;
    case 32:
      /* FX33 */
      cpu->memory[cpu->i] = (cpu->reg[b3] - cpu->reg[b3] % 100) / 100;
      cpu->memory[cpu->i + 1] = (((cpu->reg[b3] - cpu->reg[b3] % 10) / 10) % 10);
      cpu->memory[cpu->i + 2] = cpu->reg[b3] - cpu->memory[cpu->i] * 100 - cpu->memory[cpu->i + 1] * 10;
      break;
    case 33:
      /* FX55 */
      i = 0;
      while (i <= b3) {
	cpu->memory[cpu->i + i] = cpu->reg[i];
	++i;
      }
      break;
    case 34:
      /* FX65 */
      i = 0;
      while (i <= b3) {
	cpu->reg[i] = cpu->memory[cpu->i + i];
	++i;
      }
      break;
    default:
      printf("Oops!\n");
      show_debug(cpu);
      printf("op:%X   b1:%X  b2:%X  b3:%X  PC:%d\n", cpu->current_opcode, b1, b2, b3, cpu->pc);
      pause_sdl();
      exit(-1);
    }
    cpu->pc += 2;
  }
}

uint8_t		get_offset_instruction(uint16_t opcode)
{
  uint8_t	offset = 0x00;

  while (offset < C8_NB_OPCODE) {
    if ((gl_mask.mask[offset] & opcode) == gl_mask.id[offset])
      return (offset);
    ++offset;
  }
  return (0xFF);
}

uint16_t	get_opcode(chip8* cpu)
{
  if (cpu)
    return ((cpu->memory[cpu->pc] << 8) + cpu->memory[cpu->pc + 1]);
  return (0x00);
}

void		show_debug(chip8* cpu)
{
  int		i = 0;

  if (cpu) {
    printf("====== DEBUG LOG ======\n");
    printf("OPCODE : 0x%X\n", cpu->current_opcode);
    printf("PC     : %d\n", cpu->pc);
    printf("I      : %d\n", cpu->i);
    printf("NB JMP : %d\n", cpu->nb_jump);
    while (i <= 0xF) {
      if (cpu->reg[i] != 0)
	printf("register[%d] 0x%X\n", i, cpu->reg[i]);
      ++i;
    }
    if (cpu->nb_jump > 0) {
      i = 0;
      while (i < cpu->nb_jump) {
	printf("jump[%d] 0x%X\n", i, cpu->mem_jump[i]);
	++i;
      }
    }
    printf("\n=====================\n\n");
  }
}

void		reset_chip8(chip8* cpu)
{
  uint8_t	i = 0;

  if (cpu) {
    for(i=0;i<16;i++) {
      cpu->reg[i] = 0;
      cpu->mem_jump[i] = 0;
      cpu->keyboard[i] = 0;
    }
    cpu->pc = MEMORY_ROM_START;
    cpu->nb_jump = 0;
    cpu->counter_sync = 0;
    cpu->counter_sound = 0;
    cpu->i=0;
  }
}
