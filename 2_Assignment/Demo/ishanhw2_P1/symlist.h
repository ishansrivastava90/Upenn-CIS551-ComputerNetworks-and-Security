
#include "symbol.h"
struct sym_list {
	struct symbol s_sym;
	struct sym_list *s_next;
};
