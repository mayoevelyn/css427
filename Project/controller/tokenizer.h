#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <arduino.h>

class tokenizer
{
    public:
        tokenizer();       
        ~tokenizer();

        String getToken(String data, char delimiter, byte token);
        int getTokenCount(String data, char delimiter);

    private:
};

#endif
