# TODO NEXT:
- finish fn call checks: no call of not declared
- check exns free memo correctly
- check if I leak memo valgrind

# Questions
- how to catch invalid: int p(int p) { return p(p); } - rule is that p is the variable
- how to know which children to traverse to get types?

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
            - no dup vars - rule: dcl -> type ID - done
            - no undeclared use - rule: statements -> ... - done
        - procedures: NT: procedure/main - DS: map<string, Procedure>
            - no dup procedure name - rule: procedure/main -> ... - done
            - no undeclared call - rule: factor -> ID LPAREN*
            - correct arglist: # args + types - rule: params -> ...

        ->> how:
        - node.rule == dcl type ID <=> declaration found

    3. Type Checking
        - Recursive type inference ALGO:
            - get statement type -> get expressions type
        - store type info if not violated in exp nodes as we recurse

        - exp node:
    - T: NUM, NULL or ID tokens
    - NT: LHS of rule is: expr, term, factor, or lvalue
