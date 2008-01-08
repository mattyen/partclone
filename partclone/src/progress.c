/**
 * progress.c - part of Partclone project
 *
 * Copyright (c) 2007~ Thomas Tsai <thomas at nchc org tw>
 *
 * progress bar
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdio.h>
#include <locale.h>
#include <time.h>
#include "config.h"
#include "progress.h"
#include "gettext.h"
#define _(STRING) gettext(STRING)

/// initial progress bar
extern void progress_init(struct progress_bar *p, int start, int stop, int res, int size)
{
	time_t now;
	time(&now);
        p->start = start;
        p->stop = stop;
        p->unit = 100.0 / (stop - start);
	p->time = now;
	p->block_size = size;
        p->resolution = res;
}

/// update number
extern void progress_update(struct progress_bar *p, int current)
{
        setlocale(LC_ALL, "");
        bindtextdomain(PACKAGE, LOCALEDIR);
        textdomain(PACKAGE);
	
	float percent;
        float speed;
        int total_time;
        time_t remained;
	time_t elapsed;
	time_t total;
	char *format = "%H:%M:%S";
	char Rformated[10], Eformated[10], Tformated[10];
	struct tm *Rtm, *Etm, *Ttm;

        percent  = p->unit * current;
        speed    = (float)p->block_size * (float)current / (float)(time(0) - p->time);
	remained = (time_t)(p->block_size * (p->stop- current)/(int)speed);
        elapsed  = (time(0) - p->time);

	/// format time string
	Rtm = gmtime(&remained);
	strftime(Rformated, sizeof(Rformated), format, Rtm);

	Etm = gmtime(&elapsed);
	strftime(Eformated, sizeof(Eformated), format, Etm);

        if (current != p->stop) {
                if ((current - p->start) % p->resolution)
                        return;
                fprintf(stderr, _("Elapsed: %s, "), Eformated);
                fprintf(stderr, _("Remained: %s, "), Rformated);
                fprintf(stderr, _("Completed: %6.2f%%, "), percent);
                fprintf(stderr, _("Rate: %6.2fMB/s"), speed/1000000.00);
                fprintf(stderr, ("\r"));
        } else{
		total = (time(0) - p->time);
		Ttm = gmtime(&total);
		strftime(Tformated, sizeof(Tformated), format, Ttm);
                fprintf(stderr, _("\nTotal Time : %s, "), Tformated);
                fprintf(stderr, _("100.00%% completed!\n"));
	}
}
