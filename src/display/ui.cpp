#include "ui.h"

extern void ui::draw(vbrush::Interface *b, const size_t flags, const  data::app_data &ad, const data::mhw_data &d, const bool no_color, const bool compact_display, const bool monsters_only)
{
	char buf[256]; // local buffer for strings
	if (!b->init())
		return;
	/*
	24
	XXXXXXXXXXXXXXXXXXXXXXXX
	linux-hunter 0.0.6

	24                      7      32                                      1 = 64
	OOOOOOOOOOOOOOOOOOOOOOOOHHHHHHHEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEH
	SessionId:[Camw+P?+dNLP] Host:[EmettaXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX]

	Player Name 32                          Id 4Damage 10 % 6
	EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEIIIIDDDDDDDDDDPPPPPP
	 */
	// if crown data is enabled, the total h_offset has ot be increased by 20
	const unsigned h_add_offset = ((flags & draw_flags::SHOW_CROWN_DATA) ? 20 : 0);

	if (!monsters_only)
	{
		if (!compact_display)
		{
			// print title
			{
				std::snprintf(buf, 256, "linux-hunter %-*s(%4ld/%4ld/%4ld w/u/s)", 19 + h_add_offset, ad.version, ad.tm.wall, ad.tm.user, ad.tm.system);
				b->draw_text(buf);
				b->next_row();
			}
			// print main stats
			{
				b->draw_text("SessionId:[");
				b->set_attr_on(vbrush::Interface::attr::BOLD);
				b->draw_text(d.session_id.c_str());
				b->set_attr_off(vbrush::Interface::attr::BOLD);
				b->draw_text("] Host:[");
				b->set_attr_on(vbrush::Interface::attr::BOLD);
				b->draw_text(d.host_name.c_str());
				b->set_attr_off(vbrush::Interface::attr::BOLD);
				b->draw_text("]");
				b->next_row(2);
			}
		}
		// print header
		{
			std::snprintf(buf, 256, "%-*s%-4s%-10s%-8s", 32 + h_add_offset, "Player Name", "Id", "Damage", "%");
			b->set_attr_on(vbrush::Interface::attr::REVERSE);
			b->draw_text(buf);
			b->set_attr_off(vbrush::Interface::attr::REVERSE);
			b->next_row();
		}
		// compute total damage
		int total_damage = 0;
		for (size_t i = 0; i < sizeof(d.players) / sizeof(d.players[0]); ++i)
			total_damage += (d.players[i].used) ? d.players[i].damage : 0;
		// print players data
		static const vbrush::Interface::attr v_colors[] = {vbrush::Interface::attr::C_BLUE, vbrush::Interface::attr::C_MAGENTA, vbrush::Interface::attr::C_YELLOW, vbrush::Interface::attr::C_GREEN};
		for (size_t i = 0; i < sizeof(d.players) / sizeof(d.players[0]); ++i)
		{
			if (!d.players[i].used)
			{
				// in compact mode, skip displaying empty player lines
				if (!compact_display)
				{
					std::snprintf(buf, 256, "%-*s%-4d                  ", 32 + h_add_offset, "<N/A>", (int)i);
					b->set_attr_on(vbrush::Interface::attr::DIM);
					b->draw_text(buf);
					b->set_attr_off(vbrush::Interface::attr::DIM);
					b->next_row();
				}
				continue;
			}
			const auto name_attr = (d.players[i].left_session) ? vbrush::Interface::attr::DIM : v_colors[i];
			// set attribute when no_color is false
			// OR the player has left the session
			if (!no_color || d.players[i].left_session)
				b->set_attr_on(name_attr);
			b->draw_text((d.players[i].left_session) ? L"Left the session" : d.players[i].name.c_str(), 32 + h_add_offset);
			// only remove the attribute here
			// when no_color has not been set
			if (!no_color && !d.players[i].left_session)
				b->set_attr_off(name_attr);
			std::snprintf(buf, 256, "%-4d", (int)i);
			b->draw_text(buf);
			std::snprintf(buf, 256, "%10d", d.players[i].damage);
			b->draw_text(buf);
			std::snprintf(buf, 256, "%8.2f", (total_damage > 0) ? 100.0 * d.players[i].damage / total_damage : 0);
			b->draw_text(buf);
			if (d.players[i].left_session)
				b->set_attr_off(name_attr);
			b->next_row();
		}
		// now just the total
		{
			std::snprintf(buf, 256, "%-*s%-4s%10d%8s", 32 + h_add_offset, "Total", "", total_damage, (total_damage > 0) ? "100.00" : "0.0");
			b->set_attr_on(vbrush::Interface::attr::BOLD);
			b->draw_text(buf);
			b->set_attr_off(vbrush::Interface::attr::BOLD);
		}
	}
	// flasg to check monster data
	if (flags & draw_flags::SHOW_MONSTER_DATA)
	{
		if (!monsters_only)
		{
			b->next_row(compact_display ? 1 : 2);
		}
		// then Monsters - first header
		if (flags & draw_flags::SHOW_CROWN_DATA)
			std::snprintf(buf, 256, "%-36s%-14s%-8s%-8s%-8s", "Monster Name", "HP", "%", "Size", "Crown");
		else
			std::snprintf(buf, 256, "%-36s%-14s%-8s", "Monster Name", "HP", "%");
		b->set_attr_on(vbrush::Interface::attr::REVERSE);
		b->draw_text(buf);
		b->set_attr_off(vbrush::Interface::attr::REVERSE);
		// print the monster data
		const int max_monsters = sizeof(d.monsters) / sizeof(d.monsters[0]);
		int cur_monster = 0;
		while (cur_monster < max_monsters)
		{
			const data::mhw_data::monster_info &mi = d.monsters[cur_monster];
			if (!mi.used)
			{
				++cur_monster;
				continue;
			}
			b->next_row();
			if (mi.hp_current <= 0.001)
				b->set_attr_on(vbrush::Interface::attr::DIM);
			if (flags & draw_flags::SHOW_CROWN_DATA)
				std::snprintf(buf, 256, "%-36s %6d/%6d%8.2f%8.2f%8s", mi.name, (int)mi.hp_current, (int)mi.hp_total, 100.0 * mi.hp_current / mi.hp_total, mi.body_size, mi.crown);
			else
				std::snprintf(buf, 256, "%-36s %6d/%6d%8.2f", mi.name, (int)mi.hp_current, (int)mi.hp_total, 100.0 * mi.hp_current / mi.hp_total);
			b->draw_text(buf);
			if (mi.hp_current <= 0.001)
				b->set_attr_off(vbrush::Interface::attr::DIM);
			++cur_monster;
		}
	}
	b->display();
}
