

#ifndef VECTOR_H
#define VECTOR_H


#include "RefCount.h"
#include "Collection_Tool.h"
#include "Collection_TypeTrait.h"

#define LAUNCH_ASSERT

namespace ard_c
{

	template<typename T>
	struct VectorData
	{
		RefCount _ref;
		int _size;
		int _capacity;
		T *_d;


		void resize()
		{
			if (!_capacity)
			{
				_d = reinterpret_cast<T*>(::malloc(sizeof(T)));
				if (!_d) failed_alloc_purge();
				_capacity++;
			}
			else if (_size == _capacity)
			{
				int newCap = (int)nextPowerOfTwo(_capacity);
				realloc(newCap);
			}
		}
		void resize(int n)
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X(n >= 0, "VectorData::resize", "allocation must be a positive integer");
#endif
			if (n <= _capacity) return;
			int newCap = (int)nextPowerOfTwo(n);
			if (!_capacity)
			{
				_d = reinterpret_cast<T*>(::malloc(sizeof(T) * newCap));
				if (!_d) failed_alloc_purge();
				_capacity = newCap;
			}
			else realloc(newCap);
		}

		void realloc(int growth)
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X(growth >= 0, "VectorData::realloc", "allocation must be a positive integer");
#endif
			_d = reinterpret_cast<T*>(::realloc(_d, sizeof(T) * growth));
			if (!_d) failed_alloc_purge();
			_capacity = growth;
		}
		void failed_alloc_purge()
		{
			free(_d);
			_capacity = 0;
			_size = 0;
#ifdef LAUNCH_ASSERT
			ASSERT_X(false, "VectorData::realloc", "bad alloc");
#endif
		}
		

		void *deep_copy()
		{
			VectorData<T> *dest = new VectorData<T>();
			dest->_ref = RefCount::init_ref();
			dest->_capacity = _capacity;
			dest->_size = _size;
			if (TypeTrait<T>::isAtomic)
			{
				T *d = reinterpret_cast<T*>(::malloc(sizeof(T) * _capacity));
				if (!d) failed_alloc_purge();
				::memcpy(d, _d, sizeof(T) * _size);
				dest->_d = d;
			}
			else
			{
				T *c = reinterpret_cast<T*>(::malloc(sizeof(T) * _capacity));
				if (!c) failed_alloc_purge();
				dest->_d = c;
				int n = 0;
				while (n < _size)
				{
					new (c) T(_d[n]);
					++c;
					++n;
				}
			}
			return dest;
		}

		void append(const T &v)
		{
			resize();
			_d[_size] = v;
			++_size;
		}
		void append(T *range, int size)
		{
			int rc = _size - _capacity + size;
			if (rc > 0) resize(_capacity + rc);
			int newSize = _size + size;
			while (_size != newSize)
			{
				_d[_size] = *range;
				++_size;
				++range;
			}
		}
		void insert(const T &v, int i)
		{
			if (i == _size || _size == 0) { append(v); return; }
			resize();
			int x = _size - i;
			::memmove(_d + i + 1, _d + i, sizeof(T) * x);
			_d[i] = v;
			++_size;
		}
		void prepend(const T &v)
		{
			insert(v, 0);
		}
		void remove(int i)
		{
			if (int c = _size - i - 1)
			{
				::memmove(_d + i, _d + i + 1, sizeof(T) * c);
			}
			--_size;
		}

		T &at(int i) { return _d[i]; }
		const T &at(int i) const { return _d[i]; }
	};


	template<typename T>
	class Vector
	{
		VectorData<T> *_d;

	public:
		Vector()
		{
			construct_data();
		}
		Vector(int alloc)
		{
			construct_data();
			reserve(alloc);
		}
		Vector(const Vector<T> &other) : _d(other._d)
		{
			_d->_ref.ref();
		}
		~Vector()
		{
			if (!_d->_ref.deref())
			{
				free(_d);
			}
		}


		int size() const { return _d->_size; }
		bool isEmpty() const { return _d->_size == 0; }
		int capacity() const { return _d->_capacity; }
		const T &at(int index) const
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X((index < _d->_size && index >= 0), "Vector::at", "index out of range");
#endif
			return _d->at(index);
		}
		T &first()
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X(!isEmpty(), "Vector::first", "vector is empty");
#endif
			return *begin();
		}
		const T &first() const
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X(!isEmpty(), "Vector::first", "vector is empty");
#endif
			return _d->at(0);
		}
		T &last()
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X(!isEmpty(), "Vector::last", "vector is empty");
#endif
			return *(--end());
		}
		const T &last() const
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X(!isEmpty(), "Vector::last", "vector is empty");
#endif
			return _d->at(size() - 1);
		}

		void reserve(int alloc)
		{
			detach();
			_d->resize(alloc);
		}

		void append(const T &value)
		{
			detach();
			_d->append(value);
		}
		void append(const Vector<T> &other)
		{
			detach();
			_d->append(other._d->_d, other._d->_size);
		}
		void insert(const T &value, int before)
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X((before >= 0 && before < _d->_size + 1), "Vector::insert", "index out of range");
#endif
			detach();
			_d->insert(value, before);
		}
		void prepend(const T &value)
		{
			detach();
			_d->prepend(value);
		}
		void remove(int index)
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X((index >= 0 && index < _d->_size), "Vector::remove", "index out of range");
#endif
			detach();
			_d->remove(index);
		}
		void removeFirst() { remove(0); }
		void removeLast() { remove(_d->_size - 1); }

		T take(int index)
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X((index >= 0 && index < _d->_size), "Vector::take", "index out of range");
#endif
			detach();
			T t = _d->at(index);
			_d->remove(index);
			return t;
		}
		T takeFirst() { return take(0); }
		T takeLast() { return take(_d->_size - 1); }


		T &operator[](int index)
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X((index < _d->_size && index >= 0), "Vector::operator[]", "index out of range");
#endif
			detach();
			return _d->at(index);
		}
		const T &operator[](int index) const
		{
#ifdef LAUNCH_ASSERT
			ASSERT_X((index < _d->_size && index >= 0), "Vector::operator[]", "index out of range");
#endif
			return _d->at(index);
		}
		Vector<T> &operator=(const Vector<T> &other) 
		{ 
			if (_d->_ref.isShared()) _d->_ref.deref();
			_d = other._d;
			_d->_ref.ref();
			return *this;
		}
		bool operator==(const Vector<T> &other) const { return _d == other._d; }
		Vector<T> &operator<<(const T &value) { append(value); return *this; }
		Vector<T> &operator<<(const Vector<T> &other) { append(other); return *this; }


		class ConstIterator;

		class Iterator
		{
		public:
			T *_i;
			inline Iterator() {}
			inline Iterator(T *n) { _i = n; }

			inline T &operator*() const { return *_i; }
			inline T *operator->() const { return _i; }
			inline T &operator[](int i) const { return _i[i]; }
			inline bool operator==(const Iterator &other) const { return _i == other._i; }
			inline bool operator==(const ConstIterator &other) const { return _i == other._i; }
			inline bool operator!=(const Iterator &other) const { return _i != other._i; }
			inline bool operator!=(const ConstIterator &other) const { return _i != other._i; }
			inline bool operator>(const Iterator &other) const { return _i > other._i; }
			inline bool operator>(const ConstIterator &other) const { return _i > other._i; }
			inline bool operator>=(const Iterator &other) const { return _i >= other._i; }
			inline bool operator>=(const ConstIterator &other) const { return _i >= other._i; }
			inline bool operator<(const Iterator &other) const { return _i < other._i; }
			inline bool operator<(const ConstIterator &other) const { return _i < other._i; }
			inline bool operator<=(const Iterator &other) const { return _i <= other._i; }
			inline bool operator<=(const ConstIterator &other) const { return _i <= other._i; }
			inline Iterator &operator++() { ++_i; return *this; }
			inline Iterator operator++(int) { T *n = _i; ++_i; return n; }
			inline Iterator &operator--() { --_i; return *this; }
			inline Iterator operator--(int) { T *n = _i; --_i; return n; }
			inline Iterator &operator+=(int i) { _i += i; return *this; }
			inline Iterator &operator-=(int i) { _i -= i; return *this; }
			inline Iterator operator+(int i) const { return Iterator(_i + i); }
			inline Iterator operator-(int i) const { return Iterator(_i - i); }
			inline int operator-(Iterator other) const { return (int)(_i - other._i); }
		};
		friend class Iterator;

		class ConstIterator
		{
		public:
			T *_i;
			inline ConstIterator() {}
			inline ConstIterator(T *n) { _i = n; }

			inline const T &operator*() const { return *_i; }
			inline const T *operator->() const { return _i; }
			inline const T &operator[](int i) const { return _i[i]; }
			inline bool operator==(const Iterator &other) const { return _i == other._i; }
			inline bool operator==(const ConstIterator &other) const { return _i == other._i; }
			inline bool operator!=(const Iterator &other) const { return _i != other._i; }
			inline bool operator!=(const ConstIterator &other) const { return _i != other._i; }
			inline bool operator>(const Iterator &other) const { return _i > other._i; }
			inline bool operator>(const ConstIterator &other) const { return _i > other._i; }
			inline bool operator>=(const Iterator &other) const { return _i >= other._i; }
			inline bool operator>=(const ConstIterator &other) const { return _i >= other._i; }
			inline bool operator<(const Iterator &other) const { return _i < other._i; }
			inline bool operator<(const ConstIterator &other) const { return _i < other._i; }
			inline bool operator<=(const Iterator &other) const { return _i <= other._i; }
			inline bool operator<=(const ConstIterator &other) const { return _i <= other._i; }
			inline ConstIterator &operator++() { ++_i; return *this; }
			inline ConstIterator operator++(int) { T *n = _i; ++_i; return n; }
			inline ConstIterator &operator--() { --_i; return *this; }
			inline ConstIterator operator--(int) { T *n = _i; --_i; return n; }
			inline ConstIterator &operator+=(int i) { _i += i; return *this; }
			inline ConstIterator &operator-=(int i) { _i -= i; return *this; }
			inline ConstIterator operator+(int i) const { return ConstIterator(_i + i); }
			inline ConstIterator operator-(int i) const { return ConstIterator(_i - i); }
			inline int operator-(ConstIterator other) const { return (int)(_i - other._i); }
		};
		friend class ConstIterator;


		inline Iterator begin() { detach(); return Iterator(_d->_d); }
		inline ConstIterator cbegin() const { return ConstIterator(_d->_d); }
		inline Iterator end() { detach(); return Iterator(_d->_d + _d->_size); }
		inline ConstIterator cend() const { return ConstIterator(_d->_d + _d->_size); }


	private:
		void detach()
		{
			if (_d->_ref.isShared())
			{
				VectorData<T> *d = reinterpret_cast<VectorData<T>*>(_d->deep_copy());
				_d->_ref.deref();
				_d = d;
			}
		}

		void construct_data()
		{
			VectorData<T> *d = new VectorData<T>();
			d->_ref = RefCount::init_ref();
			d->_capacity = 0;
			d->_size = 0;
			d->_d = 0;
			_d = d;
		}
	};

}

#endif // !VECTOR_H




