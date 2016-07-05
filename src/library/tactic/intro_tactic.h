/*
Copyright (c) 2016 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#pragma once
#include "library/tactic/tactic_state.h"
namespace lean {
optional<tactic_state> intron(unsigned n, tactic_state const & s, buffer<name> & new_Hs);
optional<tactic_state> intron(unsigned n, tactic_state const & s);
/* Low-level versions of the previous procedures, they allow us to intron in any goal.
   The new hypotheses "user names" are generated using \c new_hs_names (when available).
   After execution, the buffer \c new_Hns stores the new interal names for the new hypotheses.

   \remark new_hs_names is an input/output parameter. The procedure will "consume" n elements from the list. */
optional<expr> intron(type_context & ctx, expr const & mvar, unsigned n, list<name> & new_hs_names, buffer<name> & new_Hns);
optional<expr> intron(type_context & ctx, expr const & mvar, unsigned n, list<name> & new_hs_names);
optional<expr> intron(type_context & ctx, expr const & mvar, unsigned n);

void initialize_intro_tactic();
void finalize_intro_tactic();
}
