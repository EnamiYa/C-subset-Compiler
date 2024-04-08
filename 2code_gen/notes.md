# To Do
- remove all asserts when done

## maybe to dos
- maybe delete the params field in Procedure class
- remove unused includes

# Questions
- callee-save or caller-save for registers?
- how to determine which regs will procedure need? just save all $5-7?
- will fp start from 0 work with procedures taking 0 args too

# Plan
- wain code then rest
- use jr $31 at end of each procedure to avoid fall through
- procedure to dos: fp - setup vars + args
    - push dcls then save registers
- wain:
    - prologue: 
        - imports + conventions + call init
        - store $1 and $2 - no need to restore them


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
