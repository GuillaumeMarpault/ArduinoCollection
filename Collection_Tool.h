

#ifndef COLLECTION_TOOL_H
#define COLLECTION_TOOL_H


#include <arduino.h>

namespace ard_c {

	inline void no_assert(void) {}
	void assert(const char *assertion, const char *file, int line)
	{
		Serial.print(assertion);
		Serial.print(" | file : ");
		Serial.print(file);
		Serial.print(", line : ");
		Serial.println(line);
		delay(50);
		abort();
	}
	void assert_x(const char *where, const char *what, const char *file, int line)
	{
		Serial.print(where);
		Serial.print(" : ");
		Serial.print(what);
		Serial.print(" | file : ");
		Serial.print(file);
		Serial.print(", line : ");
		Serial.println(line);
		delay(50);
		abort();
	}


#define ASSERT(condition) ((!(condition)) ? assert(#condition,__FILE__,__LINE__) : no_assert())
#define ASSERT_X(condition, where, what) ((!(condition)) ? assert_x(where,what,__FILE__,__LINE__) : no_assert())


	unsigned int nextPowerOfTwo(unsigned int a)
	{
		a |= a >> 1;
		a |= a >> 2;
		a |= a >> 4;
		a |= a >> 8;
		a |= a >> 16;
		++a;
		return a;
	}

}

void *operator new(size_t s, void *dest)
{
	return dest;
}

#endif	// COLLECTION_TOOL_H
