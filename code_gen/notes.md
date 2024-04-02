# To Do
- dcls
- assignment statements




## old
- test:
  - build tree w/ types - done
  - \# children is correct in nodes - done
  - sym table type extraction + offset - done

- maybe delete the params field in Procedure class
- remove unused includes

# Questions
- how to evaluate expr in order priority if ()
- will fp start from 0 work with procedures taking 0 args too

# Plan
input: wlp4 typed parse tree
output: mips assembly

## P1
- rebuild parse tree - done
- extend ST with offset from frame pointer - done

Code gen:
- store vars in stack
- include comments in generated asm for debug

## P2

## P3

## P4

## P5

# Conventions
$1 and $2: wain inputs
$3: return value of wain
$4: value 4
$10: print address
$11: value 1
$29: frame pointer
$30: stack pointer
$31: return address

#### expression nodes

- T: NUM, NULL or ID tokens
- NT: LHS of rule is: expr, term, factor, or lvalue