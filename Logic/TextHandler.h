#pragma once

#include "BaseHandler.h"


namespace Logic
{
    class TextHandler : public BaseHandler
    {
    public:
        TextHandler( void );
        ~TextHandler( void );

    public:
        static std::wstring StringToWstring( std::string str );
        void CharArrToWcharArr( const char *charArr, wchar_t **wcharArr );
        std::wstring CharArrToWstring( const char *charArr );

        bool ReadFile( std::wstring fileName );
        void WriteFile( std::wstring fileName );

        const std::wstring & GetText();

    

    protected:
        std::wstring text;
    };
}