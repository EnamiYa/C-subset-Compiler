# Questions
- why 64 bits toNumber ?
- where to store word? as part of lis ins - NO!
- lis implementation

# TODO
- range checking in isValindIns
- toBin()
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

#  Thaqib Call
- 1. Parse:
    - prior check for first token - minimize error checks
    -  Other word check if line.size() == 2 and line[0] == word, line[1] is int
    - 
- use bitset
- function that parses that line into an Instruction 
- parser file:
    - unique_ptr<Inst> parse(const vector<Token>& line);
    - if (line.size() == 2 and line[0] == Word) {
    // try parsing a word
    }

- 2. Create INS: only create the INS if valid 

