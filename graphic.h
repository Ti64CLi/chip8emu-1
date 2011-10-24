#ifndef H_CHIP8_GRAPHIC_H_
# define H_CHIP8_GRAPHIC_H_
# define	NB_PIXEL_WIDTH		64
# define	NB_PIXEL_HEIGHT		32
# define	PIXEL_SIZE		8
# define	SCREEN_WIDTH		(NB_PIXEL_WIDTH * PIXEL_SIZE)
# define	SCREEN_HEIGHT		(NB_PIXEL_HEIGHT * PIXEL_SIZE)
# define	MAX_FPS			5
# define	MAX_INSTRUC_TURN	4

typedef struct		s_sdl_pixel
{
  SDL_Rect		pos;
  uint8_t		color;
}			pixel;

extern pixel		gl_video[NB_PIXEL_WIDTH][NB_PIXEL_HEIGHT];
extern SDL_Surface*	gl_screen;
extern SDL_Surface*	gl_square[2];
extern SDL_Event	gl_event;

void		init_sdl(void);
void		init_pixel(void);
void		init_screen(void);
void		clean_screen(void);
void		draw_pixel(uint8_t b1, uint8_t b2, uint8_t b3, chip8* cpu);
void		update_screen(void);
void		exit_sdl(void);
void		pause_sdl(void);

#endif
