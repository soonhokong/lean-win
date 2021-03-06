/*
Copyright (c) 2016 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#include "library/attribute_manager.h"

namespace lean {
void initialize_old_attributes() {
    register_system_attribute(basic_attribute("elim", "elimination rule that is eagerly applied by blast grinder"));
    register_system_attribute(basic_attribute("no_pattern",
                                              "do not consider terms containing this declaration in the pattern inference procedure"));
    register_system_attribute(basic_attribute("forward", "forward chaining"));
}
void finalize_old_attributes() {}
}
