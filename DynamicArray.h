#pragma once
#include "Chunk.h"
#include "Iterator.h"

template <typename T>
class DynamicArray																// Динамический массив, подобный двусвязному списку, только выделяется не по одному элементу, а по чанкам
{
	Chunk<T>* currentChunk;														// Текущий заполняемый чанк
	int length;																	// Общая длина массива

	Chunk<T>* GetFirstChunk()													// Получение первого чанка из списка
	{
		Chunk<T>* result = currentChunk;
		while (result->previousChunk != nullptr)
			result = result->previousChunk;
		return result;
	}

	void FreeChunks()															// Метод, высвобождающий все чанки из памяти
	{
		if (currentChunk == nullptr)											// Если у объекта не выделен ни один чанк
			return;

		Chunk<T>* firstChunk = GetFirstChunk();
		while (firstChunk->nextChunk != nullptr)
		{
			firstChunk = firstChunk->nextChunk;
			delete firstChunk->previousChunk;
		}
		delete firstChunk;
	}

public:

	DynamicArray()																// Конструктор
	{
		currentChunk = new Chunk<T>();
		length = 0;
	}

	DynamicArray(const DynamicArray<T>& other)									// Конструктор копирования
	{
		currentChunk = new Chunk<T>();
		length = 0;

		Chunk<T>* tmp = other.currentChunk;
		while (tmp->previousChunk != nullptr)
			tmp = tmp->previousChunk;

		while (tmp != nullptr)
		{
			for (int i = 0; i < tmp->countThisChunk; i++)
				Push(tmp->array[i]);
			tmp = tmp->nextChunk;
		}
	}

	DynamicArray(DynamicArray<T>&& other) noexcept								// Конструктор перемещения
	{
		currentChunk = other.currentChunk;
		length = other.length;
		other.currentChunk = nullptr;
	}
		
	DynamicArray<T>& operator = (const DynamicArray<T>& other)					// Оператор присваивания копированием
	{
		FreeChunks();
		currentChunk = new Chunk<T>();
		length = 0;

		Chunk<T>* tmp = other.currentChunk;
		while (tmp->previousChunk != nullptr)
			tmp = tmp->previousChunk;

		while (tmp != nullptr)
		{
			for (int i = 0; i < tmp->countThisChunk; i++)
				Push(tmp->array[i]);
			tmp = tmp->nextChunk;
		}

		return *this;
	}

	DynamicArray<T>& operator = (DynamicArray<T>&& other) noexcept				// Оператор присваивания перемещением
	{
		FreeChunks();
		currentChunk = other.currentChunk;
		length = other.length;
		other.currentChunk = nullptr;
		return *this;
	}

	~DynamicArray()																// Деструктор, высвобождающий все чанки из памяти
	{
		FreeChunks();
	}

	int Length()																// Геттер общей длины массива
	{
		return length;
	}

	void Push(T value)															// Кладет элемент в конец списка
	{
		if (currentChunk->countThisChunk < currentChunk->sizeBuffer)			// Если в чанке есть место
		{
			currentChunk->array[currentChunk->countThisChunk++] = value;
			length++;
			return;
		}

		if (currentChunk->nextChunk != nullptr)									// Если следующий чанк уже создан
		{
			currentChunk = currentChunk->nextChunk;
			Push(value);
			return;
		}

		Chunk<T>* newChunk = new Chunk<T>();									// Если необходимо выделить новый чанк
		currentChunk->nextChunk = newChunk;
		newChunk->previousChunk = currentChunk;
		currentChunk = newChunk;
		Push(value);
	}

	T TakeLast()																// Берет последний элемент из массива
	{
		return currentChunk->array[currentChunk->countThisChunk - 1];
	}

	void Pop()																	// Удаляет элемент из конца списка
	{
		if (length == 0)														// Нельзя удалить ничего :)
			return;

		if (currentChunk->countThisChunk != 0)									// Если в чанке что-то лежит
		{
			currentChunk->countThisChunk--;
			length--;
			if (currentChunk->countThisChunk == 0)
				currentChunk = currentChunk->previousChunk;
			return;
		}

		currentChunk = currentChunk->previousChunk;								// Если чанк пустой
		Pop();
	}

	T operator[] (int index)													// Обращение к любому элементу по индексу
	{
		if (index >= length || index < 0)										// При недопустимых значениях индекса
			throw "DynamicArray. Error: Argument is out of range.\n";

		Chunk<T>* firstChunk = GetFirstChunk();

		while (index >= firstChunk->sizeBuffer)
		{
			index -= firstChunk->sizeBuffer;
			firstChunk = firstChunk->nextChunk;
		}

		return firstChunk->array[index];
	}

	Iterator<T> begin()															// Возвращает итератор начала списка
	{
		Chunk<T>* firstChunk = GetFirstChunk();
		return Iterator<T>(firstChunk, 0);
	}

	Iterator<T> end()															// Возвращает итератор конца списка
	{
		return Iterator<T>(currentChunk, currentChunk->countThisChunk);
	}
};