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
class ListDataContainer : public IDataContainer<T>
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

	virtual bool IsEmpty() const override
	{
		return m_pHead == nullptr;
	}
};

template <typename T>
class ArrayDataContainer : public IDataContainer<T>
{
	T* m_aArray;
	size_t nSize;
	size_t nPosHead;
	size_t nPosTail;

	void Resize(size_t nNewSize)
	{
		T* aTempArray = new T[nSize];

		for (size_t i = 0; i < nSize; ++i)
		{
			aTempArray[i] = m_aArray[i];
		}

		delete[] m_aArray;

		//nPosHead = 0;

		m_aArray = new T[nNewSize];

		for (size_t i = 0; i < nSize; ++i)
		{
			m_aArray[i] = aTempArray[i];
		}

		delete[] aTempArray;

		nSize = nNewSize;
	}

	bool IsFull()
	{
		return nPosTail == nSize - 1;
	}

public:

	ArrayDataContainer() : m_aArray(new T[1]), nSize(1), nPosHead(0), nPosTail(0) {}
	~ArrayDataContainer()
	{
		delete [] m_aArray; 
	}

	virtual void Enqueue(T val) override
	{
		if (IsFull())
			Resize(nSize * 2);
		m_aArray[nPosTail++] = val;
	}

	virtual T Dequeue() override
	{
		if (!IsEmpty())
		{
			T val = m_aArray[nPosHead++];
			if (nPosHead == nSize / 4)
				Resize(nSize / 2);
			return val;
		}
		else
			return T();
	}

	virtual bool IsEmpty() const override 
	{
		return nPosHead == nPosTail;
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
		for (std::map<EDataContainerType, IDataContainer<T>*>::iterator it = m_aDataContainers.begin(); it != m_aDataContainers.end(); ++it)
			delete it->second;
	}

	IDataContainer<T>* GetDataContainerForType(EDataContainerType eType)
	{
		if (m_aDataContainers.find(eType) != m_aDataContainers.end())
			return m_aDataContainers[eType];
		else
			return nullptr;
	}

};

template <typename T>
class MyQueue
{
	IDataContainer<T>* m_pDataContainer;

public:

	MyQueue(IDataContainer<T>* pContainer) : m_pDataContainer(pContainer) {}
	~MyQueue() { /*delete m_pDataContainer;*/ }

	void Enqueue(T val)
	{
		m_pDataContainer->Enqueue(val);
	}

	T Dequeue()
	{
		return m_pDataContainer->Dequeue();
	}

	bool IsEmpty() { return m_pDataContainer->IsEmpty(); }
};

const int SIZE = 10;

template <typename T>
void Do_TEST_Queue(MyQueue<T> & queue)
{
	for (int i = 0; i < SIZE; ++i)
	{
		queue.Enqueue(i);
	}

	while (!queue.IsEmpty())
	{
		size_t n = queue.Dequeue();
		std::cout << n << std::endl;
	}
}

void TEST_Queue()
{
	DataContainersFactory<int> factory;

	{
		MyQueue<int> queueA(factory.GetDataContainerForType(EDataContainerType::edctArray));

		Do_TEST_Queue(queueA);
	}

	{
		MyQueue<int> queueL = factory.GetDataContainerForType(EDataContainerType::edctList);

		Do_TEST_Queue(queueL);
	}
}

void main()
{
	TEST_Queue();
}