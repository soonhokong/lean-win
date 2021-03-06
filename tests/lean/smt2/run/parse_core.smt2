(assert true)
(assert (not false))
(assert (and true true))
(assert (or true false))
(assert (xor true false))

(assert (= true true))
(assert (distinct true false))
(assert (not (distinct true false true)))
(assert (not (distinct true false true false)))
(assert (ite true true true))

(declare-sort X)
(declare-const x1 X)
(declare-const x2 X)
(declare-const x3 X)
(assert (= (ite true x1 x2) (ite false x2 x1)))
