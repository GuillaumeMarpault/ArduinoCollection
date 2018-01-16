

#ifndef STACK_H
#define STACK_H

#include "Vector.h"

namespace ard_c
{
	template<typename T>
	class Stack : public Vector<T>
	{
	public:
		void push(const T &value) { Vector<T>::append(value); }
		T pop() { return Vector<T>::takeLast(); }
	};
}


#endif // !STACK_H

