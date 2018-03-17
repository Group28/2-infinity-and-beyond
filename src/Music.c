#include "Music.h"
#include "utils.h"
#include "IO.h"
#include <stdlib.h>


static uint16_t Note_frequency[] = {16, 17, 18, 19, 21, 22, 23, 24, 26, 27, 29, 31, 33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62,
																		65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208,
																		220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622,
																		659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661,
																		1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 4186,
																		4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902, 8372, 8870, 9397, 9956, 10548,
																		11175, 11840, 12544, 13290, 14080, 14917, 15804, 16744, 17740, 18795, 19912, 21096, 22351, 23680, 25088};


static void Music_play_note(uint8_t note){
	if(note < 128)
		IO_setSpeakerFreq(Note_frequency[note]);
}


Music Music_create(uint8_t * notes, uint16_t * delays, uint32_t length){
	Music music = malloc(sizeof(__Music));
	music->length = length;
	music->index = 0;
	
	music->notes = notes;
	music->delays = delays;
	
	return music;
}


void Music_play(Music music, double speed, int8_t offset){
	for(music->index = 0; music->index < music->length; music->index++){
		delay_ms(music->delays[music->index]/speed);
		Music_play_note(music->notes[music->index]+offset);
		
	}
}
