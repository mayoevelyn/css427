// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Peng Yang
//
// Controller  - String Tokenizer
// ----------------------------------------------------------------------------

#ifndef Tokenizer_H
#define Tokenizer_H

#include <arduino.h>

class Tokenizer
{
public:
    Tokenizer();       
    ~Tokenizer();

    String getToken(String data, char delimiter, byte token);
    int getTokenCount(String data, char delimiter);

private:
};

#endif
