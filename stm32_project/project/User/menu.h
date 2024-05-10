#ifndef __MENU_H
#define __MENU_H 			   
 
#include "public_include.h"
#include "ctl_as608.h"

void show_menu(void);
void show_select(uint8_t column);
void handle_functon(uint8_t fun_index);
__inline void back_menu(uint8_t select_id);
void check_time(void);

#endif





























