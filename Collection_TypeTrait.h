
#ifndef COLLECTION_TYPETRAIT_H
#define COLLECTION_TYPETRAIT_H


#include "TypeTrait.h"

namespace ard_c
{
	template<typename T>
	class TypeTrait
	{
	public:
		enum
		{
			isStatic = is_enum<T>::value,
			isPointer = false,
			isAtomic = is_atomic<T>::value,
			isComplex = !is_enum<T>::value && !isAtomic,
			isLarge = sizeof(T) > sizeof(void*),
			sizeOf = sizeof(T)
		};
	};

	template<typename T>
	class TypeTrait<T*>
	{
	public:
		enum
		{
			isStatic = false,
			isPointer = true,
			isAtomic = false,
			isComplex = false,
			isLarge = false,
			sizeOf = sizeof(T*)
		};
	};

	template<>
	class TypeTrait<void>
	{
	public:
		enum
		{
			isStatic = false,
			isPointer = false,
			isAtomic = false,
			isComplex = false,
			isLarge = false,
			sizeOf = 0
		};
	};
}

#endif	// COLLECTION_TYPETRAIT_H
