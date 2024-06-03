#pragma once

template <typename T>
struct Chunk							// ��������� "����" - ��������� ������� ������������� �������
{
	static const int sizeBuffer = 500;	// ������������� ������ ����������� ������ (������������� �������)

	T array[sizeBuffer];				// ������ ���������
	int countThisChunk;					// ������� ��������� � ������� �����
	Chunk<T>* previousChunk;			// ��������� �� ���������� ����
	Chunk<T>* nextChunk;				// ��������� �� ��������� ����

	Chunk()
	{
		countThisChunk = 0;
		previousChunk = nullptr;
		nextChunk = nullptr;
	}
};

