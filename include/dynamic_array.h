#include <iostream>

class DynamicArray
{
	private:
		int capacity;
		int* array = NULL;
		int size;

	public:
		DynamicArray()
		{
			capacity = 1;
			array = new int[capacity];
			size = 0;
		}

		DynamicArray(int capacity)
		{
			this->capacity = capacity;
			array = new int[capacity];
			size = 0;
		}

		int getSize() { return size; }

		int getCapacity() { return capacity; }

		int getValue(int index) { return array[index]; }

		void add(int value)
		{
			if (size == capacity) growArray();
			array[size] = value;
			size++;
		}

		int pop()
		{
			int value = array[size - 1];

			array[size - 1] = 0;
			size--;
			if (size == (capacity / 2)) shrinkArray();

			return value;
		}

		void growArray()
		{
			capacity = capacity * 2;
			int* temp = new int[capacity];

			for (int i = 0; i < size; i++) temp[i] = array[i];

			delete[] array;

			array = temp;
		}

		void shrinkArray()
		{
			capacity = size;
			int* temp = new int[capacity];

			for (int i = 0; i < size; i++) temp[i] = array[i];

			delete[] array;

			array = temp;
		}

		int search(int key)
		{
			for (int i = 0; i < size; i++) {
				if (array[i] == key) {
					return i;
				}
			}

			return -1;
		}

		void insertAt(int index, int value)
		{
			if (size == capacity) growArray();

			for (int i = size - 1; i >= index; i--) array[i + 1] = array[i];

			array[index] = value;
			size++;
		}

		void deleteAt(int index)
		{
			for (int i = index; i < size; i++) array[i] = array[i + 1];

			array[size - 1] = 0;
			size--;

			if (size == (capacity / 2)) shrinkArray();
		}
};

// based on code of Aniket Tiwari
