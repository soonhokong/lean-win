open tactic

constant f : nat → nat
constant foo : ∀ n, f n = n + 1
constant add_zero : ∀ n, n + 0 = n

definition ex1 (n : nat) : f n + 0 = n + 1 :=
by do
  set_basic_attribute `simp `foo,
  set_basic_attribute `simp `add_zero,
  simp

definition ex2 (n : nat) : f n + 0 = n + 1 :=
by do
  unset_attribute `simp `foo,
  simp -- should fail since we remove [simp] attribute from `foo`
