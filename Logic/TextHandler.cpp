#include "stdafx.h"
#include "Clock.h"
#include "TextHandler.h"

using namespace std;
using namespace Logic;


class ReadFileException 
{
};


TextHandler::TextHandler()
{
    //Clock clock;
    //ReadFile( L"qm2-w08_20130108.log" );
    //ReadFile( L"output.log" );
    //clock.ShowTimeSpan( L"For file reading" );
    //WriteFile( L"output.log" );
}

TextHandler::~TextHandler()
{
}

//Without exceptions!
std::wstring TextHandler::StringToWstring( std::string str ) throw()
{
    const char* source( str.c_str() );
    size_t size( mbstowcs( NULL, source, 0 ) );
    wchar_t *dest = new wchar_t[size];

    wmemset( dest, 0, size );
    mbstowcs( dest, source, size );
    
    wstring wStr( dest );
    delete [] dest;
    return wStr;
}

//Without exceptions!
void TextHandler::CharArrToWcharArr( const char *charArr, wchar_t **wcharArr )
{
    size_t size = mbstowcs( NULL, charArr, 0 );
    *wcharArr = new wchar_t[size];

    wmemset( *wcharArr, 0, size );
    mbstowcs( *wcharArr, charArr, size );
}

std::wstring TextHandler::CharArrToWstring( const char * charArr )
{
    try 
    {
        size_t size( mbstowcs( NULL, charArr, 0 ) );
        wchar_t *dest = new wchar_t[size];
        wstring wStr;

        try
        {
            wmemset( dest, 0, size );
            mbstowcs( dest, charArr, size );
            wStr = wstring(dest);
        }
        catch (CException *e)
        {
            delete [] dest;
            throw ReadFileException();
        }

        delete [] dest;
        return wStr;
    }
    catch (CException *e)
    {
        throw ReadFileException();
    }

}


bool TextHandler::ReadFile( std::wstring fileName )
{
    HANDLE file( CreateFile( fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 
        FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_READONLY, NULL) );
    if (!file)
        return false;
    DWORD size( GetFileSize( file, NULL ) );
    HANDLE mapFile( ::CreateFileMapping( file, NULL, PAGE_READONLY  , 0, size, NULL ) );
    if (!mapFile)
    {
        CloseHandle( file );
        return false;
    }
    PVOID mapView( ::MapViewOfFile( mapFile, FILE_MAP_READ, 0, 0, 0 ) );
    if (!mapView)
    {
        CloseHandle( mapFile );
        CloseHandle( file );
        return false;
    }
    try
    {
        text = CharArrToWstring( static_cast<const char*>( mapView ) );
    }
    catch (ReadFileException e)
    {
        UnmapViewOfFile( mapView );
        CloseHandle( mapFile );
        CloseHandle( file );
        return false;
    }
        

    UnmapViewOfFile( mapView );
    CloseHandle( mapFile );
    CloseHandle( file );
    return true;
}

void TextHandler::WriteFile( std::wstring fileName )
{
     std::wofstream outputStream( fileName, ios_base::binary );
     for (int i = 50; --i;)
     {
         outputStream.write( text.c_str(), text.length() );
     }
     
     outputStream.close();
}


const wstring & TextHandler::GetText()
{
    return text;
}
