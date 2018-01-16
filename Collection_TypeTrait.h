


/*********************************************************************/
//	_____ Collection_TypeTrait _____
//	
//	Traits pour les types fourni aux collections du namespace 'ard_c'.
//
//	Fourni une description des composants du type T fourni
//	à la collection pour définir une stratégie d'optimisation.
//
//	-isStatic : is_enum<T> est il vrai ? -> cf. TypeTrait.h
//	-isPointer : T est un pointeur ?
//	-isLarge : T à une taille > à sizeof(void *) ?
//	-isIntegral : is_integral<T> est il vrai ? -> cf. TypeTrait.h
//	-isComplex : Tout les types 'integral', enum et ceux déclarer
//		explcitement comme complex.
//	-sizeOf : égale à sizeof(T).
//	
/*********************************************************************/

#ifndef COLLECTION_TYPETRAIT_H
#define COLLECTION_TYPETRAIT_H


#include "TypeTrait.h"

namespace ard_c
{
	// TypeTrait
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
	// Spécialisation pour les pointeurs
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
	// Spécialisation void
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