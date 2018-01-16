


// #############################################################################
// #
// # Name		: Collection_TypeTrait.h
// # Version	: 1.0
// # Author		: Guillaume Marpault <guillaume.marpault@sfr.fr>
// # Date		: 16/01/2018
//
// # Description: [FR] Classes de traits renvoyant certaines propriétés
//	  des types T fournis. Ces classes sont utilisés par les classes 
//    conteneur pour définir leurs comportements.
//    Les struct 'is_enum' et 'is_atomic' sont défini dans TypeTrait.h.
//
// #  This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//	
// #############################################################################



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