#include "main.h"


#ifndef __MUSIC_H
#define __MUSIC_H

typedef struct {
  uint8_t  *notes;
	uint16_t *delays;
	uint32_t index;
	uint32_t length;
} __Music, *Music;


Music Music_create(uint8_t * notes, uint16_t * delays, uint32_t length);
void Music_play(Music music, double speed, int8_t offset);


#endif
