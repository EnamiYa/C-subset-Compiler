    //   if (getST)
    //   {
    //     //* 1st pass - sym table
    //     for (int i = 0; i < len; ++i)
    //     {
    //       Token token = tokenLine[i];
    //       Token::Kind kind = tokenLine[i].getKind();
    //       // Cases: Label - WORD - INS

    //       if (kind == Token::Kind::ID)
    //       {
    //         // check it's an INS ID, not a label use...
    //         pc += INS.find(token.getLexeme()) != INS.end() ? 1 : 0;
    //       }

    //       else if (kind == Token::Kind::WORD)
    //       {
    //         outWord(token.toNumber());
    //         ++pc;
    //       }

    //       else if (kind == Token::Kind::LABEL)
    //       {
    //         auto it = symbolTable.find(token.getLexeme());
    //         if (it != symbolTable.end())
    //         {
    //           symbolTable[token.getLexeme()] = pc;
    //         }
    //       }
    //     }
    //   }