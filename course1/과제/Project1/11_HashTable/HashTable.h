#pragma once
#define OUT
#define IN

const int TABLE_SIZE = 10;

template<typename T>
class List;

template<typename T>
class Iterator;

template<typename Key, typename Value>
struct Pair
{
	Pair() = default;
	Pair(const Key &key) : m_Key(key) {}
	Pair(const Key &key, const Value &value) : m_Key(key), m_Value(value) {}
	Key m_Key;
	Value m_Value;
};

template<typename Key, typename Value>
class HashTable
{
public:
	HashTable()
	{
		for (int i = 0; i < TABLE_SIZE; i++)
		{
			m_ListTable[i] = new List<Pair<Key, Value>>();
		}
	}

	~HashTable()
	{
		for (int i = 0; i < TABLE_SIZE; i++)
		{
			if (m_ListTable[i] != nullptr)
			{
				delete m_ListTable[i];
				m_ListTable[i] = nullptr;
			}
		}
	}
	bool Insert(const Key &key, const Value &value)
	{
		if (Search(key))
		{
			return false;
		}

		int encryptKey = Encryption(key);
		m_ListTable[encryptKey]->PushBack({ key, value });

		return true;
	}

	bool Delete(const Key &key)
	{
		int encryptKey = Encryption(key);

		for (auto it = m_ListTable[encryptKey]->begin(); it != m_ListTable[encryptKey]->end(); ++it)
		{
			Pair<Key, Value> pair = *it;
			if (pair.m_Key == key)
			{
				m_ListTable[encryptKey]->Delete(it.GetNode());
				return true;
			}
		}
		
		return false;
	}

	bool Search(const Key &key)
	{
		int encryptKey = Encryption(key);
		
		for (auto it = m_ListTable[encryptKey]->begin(); it != m_ListTable[encryptKey]->end(); ++it)
		{
			Pair<Key, Value> pair = *it;
			if (pair.m_Key == key)
				return true;
		}

		return false;
	}

	Value &operator[](const Key &key)
	{
		int encryptKey = Encryption(key);
		Value *retVal = nullptr;

		// 데이터가 없다면
		if (!Search(key))
		{
			m_ListTable[encryptKey]->PushBack({ key });
		}

		// Value&를 반환
		SearchValue(key, &retVal);
		return *retVal;
	}

private:
	int Encryption(Key key) {
		
		size_t keyByteSize = sizeof(Key);
		unsigned char* pKeySlice = reinterpret_cast<unsigned char*>(&key);
		int sum = 0;

		for (size_t i = 0; i < keyByteSize; i++)
		{
			sum += *(pKeySlice + i);
		}

		return sum % TABLE_SIZE;
	}

	bool SearchValue(const Key& key, OUT Value **retVal)
	{
		int encryptKey = Encryption(key);
		for (auto it = m_ListTable[encryptKey]->begin(); it != m_ListTable[encryptKey]->end(); ++it)
		{
			Pair<Key, Value> *pair = &*it;
			if (pair->m_Key == key)
			{
				*retVal = &pair->m_Value;
				return true;
			}
		}

		return false;
	}

private:
	List<Pair<Key, Value>> *m_ListTable[TABLE_SIZE];
};