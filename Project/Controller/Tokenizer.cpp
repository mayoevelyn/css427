#include "Tokenizer.h"

// Constructor
Tokenizer::Tokenizer()
{   
}

// Destructor
Tokenizer::~Tokenizer()
{
}

// Get Token
String Tokenizer::getToken(String data, char delimiter, byte token)
{
    int index = data.indexOf(delimiter);
    if (index < 0)
    {
        // at the end, so return
        return data;
    }
    
    if (token == 0)
    {
        // truncate any other data and return this token
        return data.substring(0, index);
    }

    // continue searching
    return getToken(data.substring(index + 1), delimiter, token - 1);
}

// Get Token Count
int Tokenizer::getTokenCount(String data, char delimiter)
{
    int count = 1;
    int index = data.indexOf(delimiter);
    int lastIndex = index;
    while (lastIndex > 0)
    {
        count++;
        index = data.indexOf(delimiter, lastIndex + 1);
        lastIndex = index;
    }

    return count;
}

