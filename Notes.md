# 2 Pass Assembler
- Pass1: 
    - check input INS valid
    - create vector of INS objects w/ labels (not yet eliminated)
    - create Symbol Table (dup definitions check)
- Pass2: 
    - eliminate labels

# Notes
## Error Checking
- register ranges: [0, 31]
- .word i: 
    - for decimal [-2^31 - 2^32 - 1] - union of signed & unsigned ranges
    - for hex: [0 - 0xffffffff]
- lw/sw/beq/bne Immediates: 
    - decimal: [-2^15 - 2^15 - 1]
    - hex: [0, 0xffff]

## Invalid Programs
- rejecting invalid programs:
    - valid INS name
    - valid # operands
    - valid kind of operands at right position

