// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include <string>
#include <list>

#define WINMTR_DIALOG_TIMER 100
#define TIMER_DELAY 1000 * 60 // 1 minute
#define WM_TRAYICON (WM_USER + 1)
#define MAXHOOP 30
const std::string FIELD_SEPARATOR = ",";



#ifdef _DEBUG
#	define TRACE_MSG(msg)									\
	{														\
	std::ostringstream dbg_msg(std::ostringstream::out);	\
	dbg_msg << msg << std::endl;							\
	OutputDebugString(dbg_msg.str().c_str());				\
	}
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#else
#	define TRACE_MSG(msg)
#endif


#include "framework.h"

#endif //PCH_H
