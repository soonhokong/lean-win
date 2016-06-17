/*
Copyright (c) 2016 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#include "kernel/instantiate.h"
#include "library/lazy_abstraction.h"
#include "library/vm/vm_name.h"
#include "library/vm/vm_nat.h"
#include "library/tactic/tactic_state.h"

namespace lean {
optional<tactic_state> intron(unsigned n, tactic_state const & s, buffer<name> & new_Hs) {
    if (n == 0) return some_tactic_state(s);
    optional<metavar_decl> g = s.get_main_goal_decl();
    if (!g) return none_tactic_state();
    metavar_context mctx = s.mctx();
    type_context ctx     = mk_type_context_for(s, mctx);
    expr type            = g->get_type();
    type_context::tmp_locals new_locals(ctx);
    for (unsigned i = 0; i < n; i++) {
        if (!is_pi(type) && !is_lambda(type)) {
            type = ctx.whnf(type);
            if (!is_pi(type))
                return none_tactic_state();
        }
        lean_assert(is_pi(type) || is_lambda(type));
        if (is_pi(type)) {
            expr H  = new_locals.push_local(binding_name(type), binding_domain(type), binding_info(type));
            type    = instantiate(binding_body(type), H);
            new_Hs.push_back(mlocal_name(H));

        } else {
            expr H  = new_locals.push_let(let_name(type), let_type(type), let_value(type));
            type    = instantiate(let_body(type), H);
            new_Hs.push_back(mlocal_name(H));
        }
    }
    local_context lctx = ctx.lctx();
    expr new_M   = mctx.mk_metavar_decl(lctx, type);
    expr new_val = new_locals.mk_lambda(mk_lazy_abstraction(new_M, new_Hs));
    mctx.assign(head(s.goals()), new_val);
    list<expr> new_gs(new_M, tail(s.goals()));
    return some_tactic_state(set_mctx_goals(s, mctx, new_gs));
}

optional<tactic_state> intron(unsigned n, tactic_state const & s) {
    buffer<name> tmp;
    return intron(n, s, tmp);
}

vm_obj tactic_intron(vm_obj const & num, vm_obj const & s) {
    optional<metavar_decl> g = to_tactic_state(s).get_main_goal_decl();
    if (!g) return mk_no_goals_exception(to_tactic_state(s));
    buffer<name> new_Hs;
    if (auto new_s = intron(force_to_unsigned(num, 0), to_tactic_state(s), new_Hs))
        return mk_tactic_success(*new_s);
    else
        return mk_tactic_exception("intron tactic failed, insufficient binders", to_tactic_state(s));
}

vm_obj intro(name const & n, tactic_state const & s) {
    optional<metavar_decl> g = s.get_main_goal_decl();
    if (!g) return mk_no_goals_exception(s);
    metavar_context mctx = s.mctx();
    type_context ctx     = mk_type_context_for(s, mctx);
    expr type            = g->get_type();
    if (!is_pi(type) && !is_let(type)) {
        type             = ctx.whnf(type);
        if (!is_pi(type))
            return mk_tactic_exception("intro tactic failed, Pi/let expression expected", s);
    }
    local_context lctx   = g->get_context();
    if (is_pi(type)) {
        name n1              = n == "_" ? binding_name(type) : n;
        expr H               = lctx.mk_local_decl(n1, binding_domain(type), binding_info(type));
        expr new_type        = instantiate(binding_body(type), H);
        expr new_M           = mctx.mk_metavar_decl(lctx, new_type);
        expr new_val         = mk_lambda(n1, binding_domain(type), mk_lazy_abstraction(new_M, mlocal_name(H)));
        mctx.assign(head(s.goals()), new_val);
        list<expr> new_gs(new_M, tail(s.goals()));
        return mk_tactic_success(set_mctx_goals(s, mctx, new_gs));
    } else {
        lean_assert(is_let(type));
        name n1              = n == "_" ? let_name(type) : n;
        expr H               = lctx.mk_local_decl(n1, let_type(type), let_value(type));
        expr new_type        = instantiate(let_body(type), H);
        expr new_M           = mctx.mk_metavar_decl(lctx, new_type);
        expr new_val         = mk_let(n1, let_type(type), let_value(type), mk_lazy_abstraction(new_M, mlocal_name(H)));
        mctx.assign(head(s.goals()), new_val);
        list<expr> new_gs(new_M, tail(s.goals()));
        return mk_tactic_success(set_mctx_goals(s, mctx, new_gs));
    }
}

vm_obj tactic_intro(vm_obj const & n, vm_obj const & s) {
    return intro(to_name(n), to_tactic_state(s));
}

void initialize_intro_tactic() {
    DECLARE_VM_BUILTIN(name({"tactic", "intro"}),   tactic_intro);
    DECLARE_VM_BUILTIN(name({"tactic", "intron"}),  tactic_intron);
}

void finalize_intro_tactic() {
}
}