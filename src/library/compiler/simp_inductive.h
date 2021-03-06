/*
Copyright (c) 2016 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#pragma once
#include "kernel/environment.h"

namespace lean {
/** \brief Replaces cases_on, projections and constructor applications with _cases.idx, _proj.idx and _cnstr.idx
    It also removes irrelevant fields from constructors.
    \remark nat.cases_on, nat.succ and nat.zero are ignored. */
expr simp_inductive(environment const & env, expr const & e);
void simp_inductive(environment const & env, buffer<pair<name, expr>> & procs);

/** \brief Return non-none idx iff \c e is of the form _cnstr.idx */
optional<unsigned> is_internal_cnstr(expr const & e);
/** \brief Return non-none idx iff \c e is of the form _proj.idx */
optional<unsigned> is_internal_proj(expr const & e);
/** \brief Return non-none n iff \c e is of the form _cases.n */
optional<unsigned> is_internal_cases(expr const & e);

void initialize_simp_inductive();
void finalize_simp_inductive();
}
