#ifndef _GUI_H_
#define _GUI_H_
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "PCD8544.h"
#include "text.h"
#define LC_INCLUDE "lc-addrlabels.h"
#include "pt.h"
#include "pt-sem.h"

void gui_init(void);
PT_THREAD(gui_task(struct pt *pt));
extern struct pt gui_pt;
#endif //_GUI_H
