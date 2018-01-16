

// #############################################################################
// #
// # Name		: Collection_Tool.h
// # Version	: 1.0
// # Author		: Guillaume Marpault <guillaume.marpault@sfr.fr>
// # Date		: 16/01/2018
//
// # Description: [FR] Méthodes utilisés par les classes conteneur :
//    - Une implémentation basique des assertions avec une sortie
//    sur le port série.
//    - La méthode nextPowerOfTwo utlisé pour le calcul de l'allocation
//    mémoire de la classe Vector. Cette méthode est extraite du code
//    source de Qt :
//    https://code.woboq.org/qt5/qtbase/src/corelib/kernel/qmath.h.html
//    - Une implémentation basique de l'opérateur 'placement new'.
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
