#pragma once
#include "Chunk.h"

template <typename T>
class Iterator																					// ���������� ����� ��������� ��� �������� ������� �� ������															
{
	Chunk<T>* currentChunk;																		// ��������� �� ��������������� ����
	int position;																				// ������� � ��������������� �����

public:

    Iterator()                                                                                  // ������� �����������
    {
        currentChunk = 0;
        position = 0;
    }

	Iterator(Chunk<T>* chunk, int position)														// �����������
	{
		currentChunk = chunk;
		this->position = position;
	}

	bool operator == (const Iterator& iter)														// �������� ��������� ���� ���������� ��������� ��������� ��������������� ������ � ������� (��� ��������� ������ ��������� �� ���� � ��� �� �������)
	{
		return currentChunk == iter.currentChunk && position == iter.position;
	}

	bool operator != (const Iterator& iter)														// �������� ����������� - ��������������� �������� ���������
	{
		return !(*this == iter);
	}

	Iterator& operator ++ (int)																	// �������� ���������� ��������� �������� �� ��������� �������
	{
		if (position < currentChunk->sizeBuffer - 1 || currentChunk->nextChunk == nullptr)		// ���� �� ��������� � �������� ������ �����
		{
			position++;
			return *this;
		}

		if (currentChunk->nextChunk == nullptr)													// ���� �������� ��������� �� ���������� ����� ������
			throw "Iterator. Error: Argument is out of range.\n";

		currentChunk = currentChunk->nextChunk;													// ��� ���������� ����� ����� - ��������� � ����������
		position = 0;
		return *this;
	}

	Iterator& operator -- (int)																	// �������� ���������� ��������� �������� �� ���������� �������
	{
		if (position > 0)																		// ���� � �������� �����
		{
			position--;
			return *this;
		}

		if (currentChunk->previousChunk == nullptr)												// ���� �������� ��������� � ������������� �������
			throw "Iterator. Error: Argument is out of range.\n";

		currentChunk = currentChunk->previousChunk;												// ���� �������� ������ ����� - ��������� � �����������
		position = currentChunk->countThisChunk - 1;
		return *this;
	}

	Iterator& operator - (int value)															// �������� ��������� ���������
	{
		if (position - value >= 0)																// ���� � ������� ����� ���������� ������� ������ ��� ����� ����
		{
			position -= value;
			return *this;
		}
		
		if (currentChunk->previousChunk == nullptr)												// ���� �������� ��������� � ������������� �������
			throw "Iterator. Error: Argument is out of range.\n";

		value -= position;
		currentChunk = currentChunk->previousChunk;
		position = currentChunk->countThisChunk;
		*this - value;
		return *this;
	}

	Iterator& operator + (int value)															// �������� �������� ���������
	{
		if (position + value < currentChunk->sizeBuffer)										// ���� � ������� ����� ������������ ������� ������ ������� �����
		{
			position += value;
			return *this;
		}

		if (currentChunk->nextChunk == nullptr)													// ���� �������� ��������� �� ���������� ����� ������
			throw "Iterator. Error: Argument is out of range.\n";

		value -= currentChunk->sizeBuffer - position;
		currentChunk = currentChunk->nextChunk;
		position = currentChunk->countThisChunk;
		*this + value;
		return *this;
	}

    T& operator * ()																				// �������� ������������� ���������� �������, �� ������� ��������� ��������
	{
		return currentChunk->array[position];
	}
};
