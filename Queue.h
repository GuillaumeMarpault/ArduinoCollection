
#ifndef QUEUE_H
#define QUEUE_H

#include "RefCount.h"
#include "Collection_Tool.h"
#include "Collection_TypeTrait.h"

namespace ard_c
{

	template<typename T>
	struct QueueData
	{
		struct Node
		{
			T *_d;
			Node *_p;
			Node *_n;
		};

		RefCount _ref;
		int _size;
		Node *_first;
		Node *_last;
		Node *_end;

		static Node *get_end_ptr() { return new Node(); }


		void enqueue(const T &v)
		{
			Node *e = construct_node(v);
			if (!_first) _first = e;
			if (_last)
			{
				_last->_n = e;
				e->_p = _last;
			}
			_last = e;
			_last->_n = _end;
			_end->_p = e;
			++_size;
		}
		T dequeue()
		{
			Node *d = _first;
			_first = _first->_n;
			--_size;
			if (!_size) _end->_p = 0;
			T r = *d->_d;
			free(d);
			return r;
		}

		T &at(int i)
		{
			if (i == 0) return *_first->_d;
			int n = 0;
			Node *c = _first;
			while (n < i)
			{
				ASSERT_X(c->_n, "QueueData::at", "index out of range");
				c = c->_n;
				++n;
			}
			return *c->_d;
		}
		const T &at(int i) const
		{
			return at(i);
		}

		Node *construct_node(const T &v)
		{
			Node *n = new Node();
			if (TypeTrait<T>::isAtomic)
			{
				::memcpy(n->_d, &v, sizeof(T));
			}
			else
			{
				n->_d = new T(v);
			}
			return n;
		}
		Node *copy_node(Node *o)
		{
			Node *n = new Node();
			if (TypeTrait<T>::isAtomic)
			{
				::memcpy(n->_d, o->_d, sizeof(T));
			}
			else
			{
				n->_d = new T(*o->_d);
			}
			return n;
		}
		void clear()
		{
			Node *l = _last;
			Node *p = _last->_p;
			while (p)
			{
				free(l);
				Node *b = p->_p;
				l = p;
				p = b;
			}
			free(_end);
		}
		void *deep_copy()
		{
			QueueData<T> *d = new QueueData<T>();
			d->_end = get_end_ptr();
			d->_ref = RefCount::init_ref();
			d->_size = _size;
			if (_first)
			{
				Node *n = _first;
				Node *c = copy_node(n);
				d->_first = c;
				while (n->_n != _end)
				{
					n = n->_n;
					Node *c2 = copy_node(n);
					c->_n = c2;
					c2->_p = c;
					c = c2;
				}
				d->_last = c;
				d->_last->_n = d->_end;
				d->_end->_p = d->_last;
			}
			
			return d;
		}
	};



	template<typename T>
	class Queue
	{
		QueueData<T> *_d;

	public:
		Queue()
		{
			construct_data();
		}
		Queue(const Queue<T> &other)
		{
			_d = other._d;
			_d->_ref.ref();
		}
		~Queue()
		{
			if (!_d->_ref.deref())
			{
				_d->clear();
				free(_d);
			}
		}

		int size() const { return _d->_size; }
		bool isEmpty() const { return _d->_size == 0; }
		const T &at(int index) const
		{
			ASSERT_X(index < _d->_size, "Queue::at", "index out of range");
			return _d->at(index);
		}

		void enqueue(const T &value)
		{
			detach();
			_d->enqueue(value);
		}

		T dequeue()
		{
			ASSERT_X(!isEmpty(), "Queue::dequeue", "Queue is empty");
			detach();
			return _d->dequeue();
		}

		T &first() { detach(); return *_d->_first; }
		const T &first() const { return *_d->_first; }
		T &last() { detach(); return *_d->_last; }
		const T &last() const { return *_d->_last; }


		T &operator[](int index)
		{
			ASSERT_X(index < _d->_size, "Queue::operator[]", "index out of range");
			detach();
			return _d->at(index);
		}
		const T &operator[](int index) const
		{
			ASSERT_X(index < _d->_size, "Queue::operator[]", "index out of range");
			return _d->at(index);
		}
		bool operator==(const Queue<T> &other) const { return _d == other._d; }
		Queue<T> operator=(const Queue<T> &other) const { return Queue<T>(other); }



		class ConstIterator;

		class Iterator
		{
		public:
			typename QueueData<T>::Node *_n;
			Iterator() {}
			Iterator(typename QueueData<T>::Node *n) : _n(n) {}

			T &operator*() { return *_n->_d; }
			T *operator->() { return _n->_d; }
			bool operator==(const Iterator &other) const { return _n == other._n; }
			bool operator==(const ConstIterator &other) const { return _n == other._n; }
			bool operator!=(const Iterator &other) const { return _n != other._n; }
			bool operator!=(const ConstIterator &other) const { return _n != other._n; }
			Iterator &operator++() { _n = _n->_n; return *this; }
			Iterator operator++(int) { Iterator i = *this; _n = _n->_n; return i; }
			Iterator &operator--() { _n = _n->_p; return *this; }
			Iterator operator--(int) { Iterator i = *this; _n = _n->_p; return i; }
		};
		friend class Iterator;

		class ConstIterator
		{
		public:
			typename QueueData<T>::Node *_n;
			ConstIterator() {}
			ConstIterator(typename QueueData<T>::Node *n) : _n(n) {}

			const T &operator*() const { return *_n->_d; }
			const T *operator->() const { return _n->_d; }
			bool operator==(const Iterator &other) const { return _n == other._n; }
			bool operator==(const ConstIterator &other) const { return _n == other._n; }
			bool operator!=(const Iterator &other) const { return _n != other._n; }
			bool operator!=(const ConstIterator &other) const { return _n != other._n; }
			ConstIterator &operator++() { _n = _n->_n; return *this; }
			ConstIterator operator++(int) { ConstIterator i = *this; _n = _n->_n; return i; }
			ConstIterator &operator--() { _n = _n->_p; return *this; }
			ConstIterator operator--(int) { ConstIterator i = *this; _n = _n->_p; return i; }
		};
		friend class ConstIterator;

		Iterator begin() { detach(); return Iterator(_d->_first); }
		ConstIterator cbegin() const { return ConstIterator(_d->_first); }
		Iterator end() { detach(); return Iterator(_d->_end); }
		ConstIterator cend() const { return ConstIterator(_d->_end); }

	private:
		void detach()
		{
			if (_d->_ref.isShared())
			{
				QueueData<T> *d = reinterpret_cast<QueueData<T>*>(_d->deep_copy());
				_d->_ref.deref();
				_d = d;
			}
		}

		void construct_data()
		{
			QueueData<T> *d = new QueueData<T>();
			d->_ref = RefCount::init_ref();
			d->_end = QueueData<T>::get_end_ptr();
			d->_size = 0;
			d->_first = 0;
			d->_last = 0;
			_d = d;
		}
	};

}

#endif // !QUEUE_H

