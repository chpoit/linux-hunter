#ifndef _MHW_LOOKUP_
#define _MHW_LOOKUP_

#include "memory.h"
#include "../data/data.h"

namespace mhw_lookup {
	struct pattern_data {
		const	memory::pattern	*player,
					*damage,
					*monster,
					*lobby;
	};
	
	extern void get_data(const pattern_data& pd, memory::browser& mb, data::mhw_data& d);
}

#endif //_MHW_LOOKUP_

