


/*********************************************************************/
//	_____ Collection_TypeTrait _____
//	
//	Traits pour les types fourni aux collections du namespace 'ard_c'.
//
//	Fourni une description des composants du type T fourni
//	� la collection pour d�finir une strat�gie d'optimisation.
//
//	-isStatic : is_enum<T> est il vrai ? -> cf. TypeTrait.h
//	-isPointer : T est un pointeur ?
//	-isLarge : T � une taille > � sizeof(void *) ?
//	-isIntegral : is_integral<T> est il vrai ? -> cf. TypeTrait.h
//	-isComplex : Tout les types 'integral', enum et ceux d�clarer
//		explcitement comme complex.
//	-sizeOf : �gale � sizeof(T).
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
	// Sp�cialisation pour les pointeurs
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
	// Sp�cialisation void
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