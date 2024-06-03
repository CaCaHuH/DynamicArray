#pragma once
#include "Chunk.h"
#include "Iterator.h"

template <typename T>
class DynamicArray																// ������������ ������, �������� ����������� ������, ������ ���������� �� �� ������ ��������, � �� ������
{
	Chunk<T>* currentChunk;														// ������� ����������� ����
	int length;																	// ����� ����� �������

	Chunk<T>* GetFirstChunk()													// ��������� ������� ����� �� ������
	{
		Chunk<T>* result = currentChunk;
		while (result->previousChunk != nullptr)
			result = result->previousChunk;
		return result;
	}

	void FreeChunks()															// �����, �������������� ��� ����� �� ������
	{
		if (currentChunk == nullptr)											// ���� � ������� �� ������� �� ���� ����
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

	DynamicArray()																// �����������
	{
		currentChunk = new Chunk<T>();
		length = 0;
	}

	DynamicArray(const DynamicArray<T>& other)									// ����������� �����������
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

	DynamicArray(DynamicArray<T>&& other) noexcept								// ����������� �����������
	{
		currentChunk = other.currentChunk;
		length = other.length;
		other.currentChunk = nullptr;
	}
		
	DynamicArray<T>& operator = (const DynamicArray<T>& other)					// �������� ������������ ������������
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

	DynamicArray<T>& operator = (DynamicArray<T>&& other) noexcept				// �������� ������������ ������������
	{
		FreeChunks();
		currentChunk = other.currentChunk;
		length = other.length;
		other.currentChunk = nullptr;
		return *this;
	}

	~DynamicArray()																// ����������, �������������� ��� ����� �� ������
	{
		FreeChunks();
	}

	int Length()																// ������ ����� ����� �������
	{
		return length;
	}

	void Push(T value)															// ������ ������� � ����� ������
	{
		if (currentChunk->countThisChunk < currentChunk->sizeBuffer)			// ���� � ����� ���� �����
		{
			currentChunk->array[currentChunk->countThisChunk++] = value;
			length++;
			return;
		}

		if (currentChunk->nextChunk != nullptr)									// ���� ��������� ���� ��� ������
		{
			currentChunk = currentChunk->nextChunk;
			Push(value);
			return;
		}

		Chunk<T>* newChunk = new Chunk<T>();									// ���� ���������� �������� ����� ����
		currentChunk->nextChunk = newChunk;
		newChunk->previousChunk = currentChunk;
		currentChunk = newChunk;
		Push(value);
	}

	T TakeLast()																// ����� ��������� ������� �� �������
	{
		return currentChunk->array[currentChunk->countThisChunk - 1];
	}

	void Pop()																	// ������� ������� �� ����� ������
	{
		if (length == 0)														// ������ ������� ������ :)
			return;

		if (currentChunk->countThisChunk != 0)									// ���� � ����� ���-�� �����
		{
			currentChunk->countThisChunk--;
			length--;
			if (currentChunk->countThisChunk == 0)
				currentChunk = currentChunk->previousChunk;
			return;
		}

		currentChunk = currentChunk->previousChunk;								// ���� ���� ������
		Pop();
	}

	T operator[] (int index)													// ��������� � ������ �������� �� �������
	{
		if (index >= length || index < 0)										// ��� ������������ ��������� �������
			throw "DynamicArray. Error: Argument is out of range.\n";

		Chunk<T>* firstChunk = GetFirstChunk();

		while (index >= firstChunk->sizeBuffer)
		{
			index -= firstChunk->sizeBuffer;
			firstChunk = firstChunk->nextChunk;
		}

		return firstChunk->array[index];
	}

	Iterator<T> begin()															// ���������� �������� ������ ������
	{
		Chunk<T>* firstChunk = GetFirstChunk();
		return Iterator<T>(firstChunk, 0);
	}

	Iterator<T> end()															// ���������� �������� ����� ������
	{
		return Iterator<T>(currentChunk, currentChunk->countThisChunk);
	}
};