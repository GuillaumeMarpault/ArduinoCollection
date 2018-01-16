

// #############################################################################
// #
// # Name		: Stack.h
// # Version	: 1.0
// # Author		: Guillaume Marpault <guillaume.marpault@sfr.fr>
// # Date		: 16/01/2018
//
// # Description: [FR] Classe basé sur Vector. Fourni juste les méthodes
//    de comodités push() et pop() pour coller au nommage standard.
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

