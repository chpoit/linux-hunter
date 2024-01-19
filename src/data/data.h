#ifndef _DATA_H_
#define _DATA_H_

#include <string>
#include "../timer.h"

namespace data {
	struct app_data {
		const char*	version;
		timer::cpu_ms	tm;
	};

	struct mhw_data {
		struct player_info {
			bool		used = false,
					left_session = false;
			std::wstring	name;
			int32_t		damage = 0;

		};

		struct monster_info {
			bool		used = false;
			const char*	name = "<N/A>";
			float		hp_total = -1.0,
					hp_current = -1.0,
					body_size = 0;
			const char*	crown = "";
		};

		std::wstring	session_id,
				host_name;
		player_info	players[4];
		monster_info	monsters[3];
	};

}

#endif // _DATA_H_

