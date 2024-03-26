# TODO NEXT:

- procedure IDs don't have types!
- test all well typed cases
- how to catch invalid: int p(int p) { return p(p); } - rule is that p is the variable

# ROAD PLAN

- input: parse tree
- output:

  - VALID: parse tree + type info on expression nodes
  - INVALID: ERROR to stderr

- TODO:

  1. rebuild parse tree - DONE

     - build AST: remove augmented grammar nodes: first two lines of input + last line
     - build tree

  2. symbol table / name errors:

     - vars: NT: dcls/params - DS: field in Procedure
       - no dup vars - rule: dcl -> type ID - done / tested
       - no undeclared use - rule: statements -> ... - done / tested
     - procedures: NT: procedure/main - DS: map<string, Procedure>
       - no dup procedure name - rule: procedure/main -> ... - done / tested
       - no undeclared call - rule: factor -> ID LPAREN\* - done / tested
       - correct arglist: # args + types - rule: params -> ...

     ->> how:

     - node.rule == dcl type ID <=> declaration found

  3. Type Checking

     - Recursive type inference ALGO: postorder
       - get statement type -> get expressions type
     - store type info if not violated in exp nodes as we recurse

     - exp node:

  - T: NUM, NULL or ID tokens
  - NT: LHS of rule is: expr, term, factor, or lvalue

# Reflections

- don't write code until you need it
- test gradually
- take breaks between debugging - it's easy to get frustrated and make things worse
- include guards
- exceptions are great with RAII :DD
