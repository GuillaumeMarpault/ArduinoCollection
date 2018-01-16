

// #############################################################################
// #
// # Name		: RefCount.h
// # Version		: 1.0
// # Author		: Guillaume Marpault <guillaume.marpault@sfr.fr>
// # Date		: 16/01/2018
//
// # Description: [FR] Fourni un comptage de référence basique utilisé
//    par les classes conteneur.
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


#ifndef REF_COUNT_H
#define REF_COUNT_H

namespace ard_c 
{
	struct RefCount
	{
		int _count;

		bool isShared() const { return _count > 1; }
		int ref() { return ++_count; }
		int deref() { return --_count; }

		static RefCount init_ref()
		{
			RefCount r = { 1 };
			return r;
		}
	};
}

#endif	// REF_COUNT_H
