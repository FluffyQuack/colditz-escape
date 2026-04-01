/*
 *  Colditz Escape - Rewritten Engine for "Escape From Colditz"
 *  copyright (C) 2008-2017 Aperture Software
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  ---------------------------------------------------------------------------
 *  game.h: Game runtime definitions
 *  ---------------------------------------------------------------------------
 */

#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/*
 *	These game macro will come handy later on
 */

// Reads the tile index at (x,y) from compressed map and rotate
#define comp_readtile(x,y)			\
	((uint32_t)(readlong((uint8_t*)fbuffer[COMPRESSED_MAP], ((y)*room_x+(x))*4) & 0x1FF00) >> 8)
#define room_readtile(x,y)			\
	((uint32_t)(readword((uint8_t*)(fbuffer[ROOMS]+offset), ((y)*room_x+(x))*2) & 0xFF80) >> 7)
#define readtile(x,y)				\
	(is_outside?comp_readtile(x,y):room_readtile(x,y))

// Reads the exit index, which we need to get to the target room index
#define comp_readexit(x,y)			\
	((uint32_t)(readlong((uint8_t*)fbuffer[COMPRESSED_MAP], ((y)*room_x+(x))*4) & 0x1F))
#define room_readexit(x,y)			\
	((uint32_t)(readword((uint8_t*)(fbuffer[ROOMS]+offset), ((y)*room_x+(x))*2) & 0x1F))
#define readexit(x,y)				\
	(is_outside?comp_readexit(x,y):room_readexit(x,y))

// Returns the offset of the byte that describes the exit status (open/closed, key level...)
#define room_get_exit_offset(x,y)	\
	(offset + ((y)*room_x+(x))*2 + 1)
#define comp_get_exit_offset(x,y)	\
	(comp_readexit(x,y) << 3)
#define get_exit_offset(x,y)		\
	(is_outside?comp_get_exit_offset(x,y):room_get_exit_offset(x,y))

// Toggle the exit open flag
#define toggle_open_flag(x_flags)  x_flags ^= 0x10

// Checks that an overlay is visible onscreen (with generous margins)
#define is_offscreen_x(x) ((x < -64) || (x > (PSP_SCR_WIDTH+64)))
#define is_offscreen_y(y) ((y < -64) || (y > (PSP_SCR_HEIGHT+64)))
#define ignore_offscreen_x(ovl)	{if is_offscreen_x(overlay[ovl].x) continue;}
#define ignore_offscreen_y(ovl)	{if is_offscreen_y(overlay[ovl].y) continue;}

// Get the current animated SID
#define get_guybrush_sid(x)												\
	( (((guybrush[x].state & (STATE_MOTION|STATE_ANIMATED))				\
	&& (!(guybrush[x].state & STATE_BLOCKED))) && !paused)?				\
	get_animation_sid(x, true):get_stop_animation_sid(x, true))

// Safe increments for our stacks
#define safe_nb_animations_increment() {	\
	if (nb_animations <= (MAX_ANIMATIONS-1))\
		nb_animations++;					\
	else									\
		perr("Too many animations!\n");	}

#define safe_overlay_index_increment() {	\
	if (overlay_index <= (MAX_OVERLAYS-1))	\
		overlay_index++;					\
	else									\
		perr("Too many overlays!\n");		}


// A few definitions to make prop handling and status messages more readable
extern uint64_t t_status_message_timeout[NB_NATIONS];
static __inline void set_status_message(int nation, void* msg, int priority, uint64_t timeout_duration)
{
    int from = 0, to = 3; //Default to assigning message to all nations
    if(nation >= 0 && nation < 4) from = to = nation; //Assign message to one nation

    for(int i = from; i <= to; i++)
    {
	    if (priority >= status_message_priority[i])
	    {
		    t_status_message_timeout[i] = game_time + timeout_duration;
		    status_message[i] = (char*)(msg);
		    status_message_priority[i] = priority;
	    }
    }
}

static __inline void consume_prop(int nationIdx)
{	// we can use an __inline here because we deal with globals
	if (!opt_keymaster)
	{	// consume the prop
		props[nationIdx][selected_prop[nationIdx]]--;
		if (props[nationIdx][selected_prop[nationIdx]] == 0)
		// display the empty box if last prop
			selected_prop[nationIdx] = 0;
	}
}

#define update_props_message(nation_idx, prop_id)									\
	nb_props_message[nation_idx][1] = (props[nation_idx][prop_id] / 10) + 0x30;	\
	nb_props_message[nation_idx][2] = (props[nation_idx][prop_id] % 10) + 0x30;	\
	strcpy(nb_props_message[nation_idx]+6, (char*) fbuffer[LOADER] + readlong(fbuffer[LOADER],	\
		PROPS_MESSAGE_BASE + 4*(prop_id-1)) + 1);

#define show_prop_count(nation_idx)													\
	update_props_message(nation_idx, selected_prop[nation_idx]);					\
	set_status_message(nation_idx, nb_props_message[nation_idx], 1, PROPS_MESSAGE_TIMEOUT)


/*
 *	Global variables
 */
extern uint8_t		nb_animations;
extern s_animation	animations[MAX_ANIMATIONS];
extern s_guybrush	guybrush[NB_GUYBRUSHES];


/*
 *	Public prototypes
 */
void free_data();
void load_all_files();
void reload_files();
void newgame_init();
bool save_game(char* save_name);
bool load_game(char* load_name);
void depack_loadtune();
void set_room_props(int nationIdx);
void set_sfxs();
void move_guards();
void toggle_exit(uint32_t exit_nr);
int16_t check_footprint(int16_t dx, int16_t d2y, int nationIdx);
int16_t check_tunnel_io();
bool check_guard_footprint(uint8_t g, int16_t dx, int16_t d2y);
void switch_nation(uint8_t new_nation);
void switch_room(int16_t exit, bool tunnel_io, int nationIdx);
void fix_files(bool reload);
void timed_events(uint16_t hours, uint16_t minutes_high, uint16_t minutes_low);
void check_on_prisoners();
void play_sfx(int sfx_id);
void go_to_jail(uint32_t p);
void set_room_xy(uint16_t room);
void set_props_overlays(int nationIdx);
void crm_set_overlays(int16_t x, int16_t y, uint16_t current_tile);
void cmp_set_overlays();
void removable_walls();
void add_guybrushes();
void sort_overlays(uint8_t a[], uint8_t n);
void play_cluck();
void thriller_toggle();
void process_motion(int nationIdx);

#ifdef	__cplusplus
}
#endif
