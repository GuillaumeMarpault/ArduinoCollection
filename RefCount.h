
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
