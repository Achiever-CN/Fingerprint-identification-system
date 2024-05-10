#ifndef __SYN6288_H
#define __SYN6288_H

#include "sys.h"


void SYN_FrameInfo(u8 Music, u8 *HZdata);
void YS_SYN_Set(u8 *Info_data);
void SYN_Play(uint8_t background, uint8_t volumeLevel, uint8_t backgroundLevel, uint8_t speechRate, char *text);
void SYN_DefaultPlay(char *text);

#endif

