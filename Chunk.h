#pragma once

template <typename T>
struct Chunk							// Структура "Чанк" - составной элемент динамического массива
{
	static const int sizeBuffer = 500;	// Фиксированный размер создаваемых чанков (настраивается вручную)

	T array[sizeBuffer];				// Массив элементов
	int countThisChunk;					// Счетчик элементов в текущем чанке
	Chunk<T>* previousChunk;			// Указаетль на предыдущий чанк
	Chunk<T>* nextChunk;				// Указатель на следующий чанк

	Chunk()
	{
		countThisChunk = 0;
		previousChunk = nullptr;
		nextChunk = nullptr;
	}
};

