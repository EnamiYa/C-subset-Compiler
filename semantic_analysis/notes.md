# TODO NEXT:
- lvalue STAR factor rule in inferType
- multiple dcls => out_of_range
- fix seg fault: WHEN multiple dcls and statements
EX1:
int wain(int* a, int b) {
    b = 3;
    b = 4;
    return *a;
}
EX2:
int wain(int* a, int b) {
    int* c = 1;
    int e = 1;
    return 1;
}
- test &E dereference
- check my inferType cases are correct
- check I am infering all possible expressions
- test all well typed cases

- check exns free memo correctly
- check if I leak memo valgrind
- catch f(int c) { return &c; } - not well typed

# Questions
- how to catch invalid: int p(int p) { return p(p); } - rule is that p is the variable
- how to know which children to traverse to get types?
- how to catch declarations all at top?

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
            - no undeclared call - rule: factor -> ID LPAREN* - done / tested
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
