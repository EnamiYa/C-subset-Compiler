# changes
- input: tokens VS components - DONE
    - insert BOF BOF and EOF EOF - DON'T CONTRIBUTE TO K/input
    - no components - change control flow
    - read components from strings provided

- output: tree vs prints
    - different print: only parse tree required -> remove reduction
    - tree stack/ stack of nodes - struct node w/ bool field for isTerm
        - shift = create T + push
        - reduce = create NT + add n nodes (subtrees) + reverse children + pop those children
    - non terminal

- ERROR: no transition on KIND of next token

# WARNINGS - TO READ IF I HAVE BUG
- BOF BOF and EOF EOF DON'T CONTRIBUTE TO K/[input] string
