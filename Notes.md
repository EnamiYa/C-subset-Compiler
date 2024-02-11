# Questions
- why 64 bits toNumber ?
- where to store word? as part of lis ins - NO!
- lis implementation

# Done Tests:
- ST offset and address label elimination

# TODO
- range checking in isValindIns
- toBin() - DONE
- pass 2

- Pass1: 
    - seperate INS + Symbol Table (dup defs check)
- Pass2: 
    - sub labels with address/ offset

# Error Checking
- register ranges
- .word i: check tut
- IMM (hex, dec, label):
    - lw/sw doesn't take labels
    - 16 bits
- rejecting invalid programs:
    - focus on finding valid
        - valid MIPS INS?
        - valid format for INS?
        - valid operands?

