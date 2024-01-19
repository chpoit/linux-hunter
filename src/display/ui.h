#ifndef _UI_H_
#define _UI_H_

#include <string>
#include "../timer.h"
#include "../data/data.h"
#include "vbrush.h"

namespace ui
{

	enum draw_flags
	{
		SHOW_MONSTER_DATA = 1,
		SHOW_CROWN_DATA = 2,
	};

	extern void draw(vbrush::Interface *b, const size_t flags, const data::app_data &ad, const data::mhw_data &d, const bool no_color, const bool compact_display, const bool monsters_only = false);
}

#endif // _UI_H_
