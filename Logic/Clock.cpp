#include "stdafx.h"
#include "Clock.h"

using namespace Logic;

const int Clock::BufferSize = 20;
const int Clock::SecInMillisec = 1000;


Clock::Clock(void)
{
    startTime = std::clock();
}

Clock::~Clock(void)
{
}


void Clock::ShowTimeSpan( PWCHAR caption )
{
    WCHAR buffer[BufferSize];  
    swprintf( buffer, BufferSize, L"%.0f milliseconds", ( clock() - startTime ) / ( double )( CLOCKS_PER_SEC / SecInMillisec ) );
    MessageBox( NULL, buffer, caption, MB_ICONINFORMATION );
}


