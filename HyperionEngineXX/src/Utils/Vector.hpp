#pragma once

#include "SatAlloc.hpp"

template <typename V, size_t initialCapacity = 2>
class Vector
{
private:
	size_t capacity;
	size_t size;
	V *vector;

public:
	Vector()
	{
		capacity = initialCapacity;
		vector = SatAlloc::Malloc<V>(capacity);
		size = 0;
	}

	~Vector() { SatAlloc::Free(vector); }
	size_t Capacity() const { return capacity; }
	bool Empty() const { return size == 0; }
	size_t Size() const { return size; }
	size_t Last() const { return size ? size - 1 : 0; }
	void Clear() { size = 0; }

	void Push(const V &data)
	{
		if (size >= capacity)
		{
			// capacity = (capacity << 1) - (capacity >> 1);
			capacity *= 2;
			SatAlloc::Realloc(&vector, capacity);
		}

		if (vector)
		{
			vector[size] = data;
			size++;
		}
	}

	void Compact()
	{
		size_t newCapacity = (size << 1) - (size >> 1) - (size >> 2);
		if (newCapacity < capacity && newCapacity > 2)
		{
			capacity = newCapacity;
			SatAlloc::Realloc(&vector, capacity);
		}
	}

	void Pop()
	{
		if (!Empty())
			size--;
	}

	template <bool checkRange = false>
	V *At(const size_t &pos) const
	{
		if constexpr (checkRange)
			if (pos >= size)
				return nullptr;
		return &this->vector[pos];
	}

	V &operator[](const size_t &pos) const
	{
		return this->vector[pos];
	}
};
