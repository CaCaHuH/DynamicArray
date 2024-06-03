#pragma once
#include "Chunk.h"

template <typename T>
class Iterator																					// —амописный класс итератора дл€ быстрого прохода по чанкам															
{
	Chunk<T>* currentChunk;																		// ”казатель на рассматриваемый чанк
	int position;																				// ѕозици€ в рассматриваемом чанке

public:

    Iterator()                                                                                  // Ќулевой конструктор
    {
        currentChunk = 0;
        position = 0;
    }

	Iterator(Chunk<T>* chunk, int position)														//  онструктор
	{
		currentChunk = chunk;
		this->position = position;
	}

	bool operator == (const Iterator& iter)														// ќператор сравнени€ двух итераторов провер€ет равенство рассматриваемых чанков и позиций (оба итератора должны ссылатьс€ на один и тот же элемент)
	{
		return currentChunk == iter.currentChunk && position == iter.position;
	}

	bool operator != (const Iterator& iter)														// ќператор неравенства - противоположна€ операци€ сравнению
	{
		return !(*this == iter);
	}

	Iterator& operator ++ (int)																	// ќператор инкремента переводит итератор на следующий элемент
	{
		if (position < currentChunk->sizeBuffer - 1 || currentChunk->nextChunk == nullptr)		// ≈сли мы находимс€ в пределах одного чанка
		{
			position++;
			return *this;
		}

		if (currentChunk->nextChunk == nullptr)													// ≈сли пытаемс€ двигатьс€ за выделенную длину списка
			throw "Iterator. Error: Argument is out of range.\n";

		currentChunk = currentChunk->nextChunk;													// ѕри достижении конца чанка - переходим к следующему
		position = 0;
		return *this;
	}

	Iterator& operator -- (int)																	// ќператор декремента переводит итератор на предыдущий элемент
	{
		if (position > 0)																		// ѕока в пределах чанка
		{
			position--;
			return *this;
		}

		if (currentChunk->previousChunk == nullptr)												// ≈сли пытаемс€ двигатьс€ в отрицательные индексы
			throw "Iterator. Error: Argument is out of range.\n";

		currentChunk = currentChunk->previousChunk;												// ≈сли достигли начала чанка - переходим к предыдущему
		position = currentChunk->countThisChunk - 1;
		return *this;
	}

	Iterator& operator - (int value)															// ќператор вычитани€ итератора
	{
		if (position - value >= 0)																// ≈сли в текущем чанке вычитаема€ позици€ больше или равна нулю
		{
			position -= value;
			return *this;
		}
		
		if (currentChunk->previousChunk == nullptr)												// ≈сли пытаемс€ двигатьс€ в отрицательные индексы
			throw "Iterator. Error: Argument is out of range.\n";

		value -= position;
		currentChunk = currentChunk->previousChunk;
		position = currentChunk->countThisChunk;
		*this - value;
		return *this;
	}

	Iterator& operator + (int value)															// ќператор сложени€ итератора
	{
		if (position + value < currentChunk->sizeBuffer)										// ≈сли в текущем чанке складываема€ позици€ меньше размера чанка
		{
			position += value;
			return *this;
		}

		if (currentChunk->nextChunk == nullptr)													// ≈сли пытаемс€ двигатьс€ за выделенную длину списка
			throw "Iterator. Error: Argument is out of range.\n";

		value -= currentChunk->sizeBuffer - position;
		currentChunk = currentChunk->nextChunk;
		position = currentChunk->countThisChunk;
		*this + value;
		return *this;
	}

    T& operator * ()																				// ќператор разыменовани€ возвращает элемент, на который указывает итератор
	{
		return currentChunk->array[position];
	}
};
