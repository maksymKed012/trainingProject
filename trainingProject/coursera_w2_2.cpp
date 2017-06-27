#include <iostream>
#include <map>
#include <memory>


template <typename T>
class IDataContainer
{
public:
	virtual ~IDataContainer() {}
	virtual void Enqueue(T val) = 0;
	virtual T Dequeue() = 0;
	virtual bool IsEmpty() const = 0;
};

template <typename T>
class ListDataContainer : IDataContainer<T>
{
	struct Node
	{
		T data;
		Node* pNext;

		Node(T val) : data(val), pNext(nullptr) {}
		~Node() {}
	};

	Node* m_pHead;
	Node* m_pTail;

public:

	ListDataContainer() : m_pHead(nullptr), m_pTail(nullptr) {}
	~ListDataContainer()
	{
		while (m_pHead != nullptr)
		{
			Node* pTemp = m_pHead->pNext;
			delete m_pHead;
			m_pHead = pTemp;
		}
	}

	virtual void Enqueue(T val) override
	{
		Node* pNode = new Node(val);
		Node* pOldTail = m_pTail;
		m_pTail = pNode;
		if (IsEmpty())
			m_pHead = m_pTail;
		else
			pOldTail->pNext = m_pTail;
	}

	virtual T Dequeue() override
	{
		Node* pNode = m_pHead;
		m_pHead = m_pHead->pNext;
		if (IsEmpty()) m_pTail = nullptr;
		return pNode->data;
	}

	virtual bool IsEmpty() override
	{
		return m_pHead == nullptr;
	}
};

template <typename T>
class ArrayDataContainer : public IDataContainer<T>
{
	T* m_aArray;
	size_t nCapacity;
	size_t nPosHead;
	size_t nPosTail;

	void Resize(size_t nNewSize)
	{
		
	}

public:

	ArrayDataContainer() : m_aArray(new T[1]), nCapacity(1), nPosHead(0), nPosTail(0) {}
	~ArrayDataContainer() { delete[] m_aArray; }

	virtual void Enqueue(T val) override
	{

	}

	virtual T Dequeue() override
	{
		return T();
	}

	virtual void IsEmpty() const override 
	{
		return true;
	}
};



enum class EDataContainerType
{
	edctNone = -1,
	edctList,
	edctArray
};

template <typename T>
class DataContainersFactory
{
	std::map<EDataContainerType, IDataContainer<T>*> m_aDataContainers;

public:

	DataContainersFactory()
	{
		m_aDataContainers[EDataContainerType::edctArray] = new ArrayDataContainer<T>();
		m_aDataContainers[EDataContainerType::edctList] = new ListDataContainer<T>();
	}

	~DataContainersFactory()
	{
		m_aDataContainers.clear();
	}

	IDataContainer<T>* GetDataContainerForType(EDataContainerType eType)
	{
		if (m_aDataContainers.find(eType) != m_aDataContainers.end())
			return m_aDataContainers[eType];
		else
			return nullptr;
	}

};