#ifndef H_CPU_CHIP8_H_
# define H_CPU_CHIP8_H_
# define	C8_MEMORY_SIZE		4096
# define	C8_NB_REGISTER		16
# define	C8_MAX_JUMP		16
# define	C8_NB_OPCODE		35
# define	MEMORY_ROM_START	512
# define	KEYBOARD_MAX_KEY	16

typedef struct		s_chip8_cpu
{
  uint8_t		memory[C8_MEMORY_SIZE];
  uint8_t		reg[C8_NB_REGISTER];
  uint16_t		i;
  uint16_t		mem_jump[C8_MAX_JUMP];
  uint8_t		nb_jump;
  uint8_t		counter_sync;
  uint8_t		counter_sound;
  uint16_t		pc;
  uint16_t		current_opcode;
  uint8_t		keyboard[KEYBOARD_MAX_KEY];
}			chip8;

typedef struct		s_chip8_mask
{
  uint16_t		mask[C8_NB_OPCODE];
  uint16_t		id[C8_NB_OPCODE];
}			chip8_mask;

chip8_mask	gl_mask;

void		init_chip8(chip8* cpu);
void		update_counter(chip8* cpu);
void		load_rom(chip8* cpu, const char* filename);
void		cpu_next(chip8* cpu);
uint16_t	get_opcode(chip8* cpu);
uint8_t		get_offset_instruction(uint16_t opcode);
void		show_debug(chip8* cpu);
void		reset_chip8(chip8* cpu);

#endif
