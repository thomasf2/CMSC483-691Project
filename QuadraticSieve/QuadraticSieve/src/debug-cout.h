/*
 * debug-cout.h
 *
 *  Created on: Feb 21, 2013
 *      Author: sky
 */

#ifndef DEBUG_COUT_H_
#define DEBUG_COUT_H_

#include <iostream>


class DebugCout : public std::ostream {

public:
	DebugCout() : std::ostream(std::cout.rdbuf()) { }

};

template <typename T>
DebugCout& operator <<(DebugCout& dcout, const T& v)
{
#if defined(SHOW_STATUS_MESSAGES)
	std::cout << v;
	//static_cast<std::ostream&>(dcout) << v;
#endif
	return dcout;
}

#endif /* DEBUG_COUT_H_ */
