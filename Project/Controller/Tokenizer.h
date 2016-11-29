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
