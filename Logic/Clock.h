#pragma once

namespace Logic
{
	class Clock
	{
	protected:
	    static const int BufferSize;
	    static const int SecInMillisec;
	
	    clock_t startTime;
	
	
	public:
	    Clock(void);
	    virtual ~Clock(void);
	
	
	public:
	    void ShowTimeSpan( PWCHAR caption );
	};
}

