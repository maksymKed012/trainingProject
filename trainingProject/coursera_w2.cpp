#include <iostream>

template <typename T>
class IDataContainer
{
public:
	virtual ~IDataContainer() {}
	virtual void push(const T val) = 0;
	virtual T pop() = 0;
	virtual bool IsEmpty() const = 0;
};

template <typename T>
class ListDataContainer : public IDataContainer<T>
{
	struct Node
	{
		T m_pData;
		Node* m_pNext;

		Node(T val) : m_pData(val), m_pNext(nullptr) {}
		~Node() { /*delete m_pData;*/ }
	};

	Node* m_pHead;

public:

	ListDataContainer() :  m_pHead(nullptr) {}
	ListDataContainer(T val) : m_pData(val), m_pHead(nullptr) {}

	~ListDataContainer()
	{
		Node* pNode = m_pHead;
		while (pNode != nullptr)
		{
			Node* pTemp = pNode->m_pNext;
			delete pNode;
			pNode = pTemp;

		}
	}

	virtual void push(const T val) override
	{
		Node* pNode = new Node(val);
		Node* pOldHead = m_pHead;
		m_pHead = pNode;
		m_pHead->m_pNext = pOldHead;
	}

	virtual T pop() override
	{
		Node* pNode = m_pHead;
		m_pHead = pNode->m_pNext;
		return pNode->m_pData;
	}

	virtual bool IsEmpty() const override
	{
		return m_pHead == nullptr;
	}
};

const int DEF_SIZE = 2;

template <typename T>
class ArrayDataContainer : public IDataContainer<T>
{

	T* m_aArray;
	size_t nSize;
	size_t nPos;
	
	void IncreaseCapacity()
	{
		T* temp = new T[nSize];

		for (size_t i = 0; i < nSize; ++i)
		{
			temp[i] = m_aArray[i];
		}

		delete[] m_aArray;

		nSize *= 2;

		m_aArray = new T[nSize];

		for (size_t i = 0; i < nSize / 2; ++i)
		{
			m_aArray[i] = temp[i];
		}

		delete[] temp;
	}

	bool IsFull() const
	{
		return nPos == nSize - 1;
	}

public:

	ArrayDataContainer() : nSize(DEF_SIZE), nPos(0), m_aArray(new T[nSize]) {}
	~ArrayDataContainer() {}

	virtual void push(const T val) override
	{
		if (IsFull())
			IncreaseCapacity();

		m_aArray[nPos++] = val;
	}

	virtual T pop() override
	{
		if (!IsEmpty())
		{
			return m_aArray[--nPos];
		}
		else
			return  T();
	}

	virtual bool IsEmpty() const override
	{
		return nPos == 0;
	}


};

template <typename T>
class MyStack
{
	IDataContainer<T>* m_pDataContainer;
public:

	MyStack() : m_pDataContainer(nullptr) {}
	MyStack(IDataContainer<T>* pDataContainer) : m_pDataContainer(pDataContainer) {}
	~MyStack()
	{
		delete m_pDataContainer;
	}

	bool IsEmpty() const { return m_pDataContainer->IsEmpty(); }

	void push(const T val)
	{
		m_pDataContainer->push(val);
	}

	T pop()
	{
		return m_pDataContainer->pop();
	}
};

void TEST_stack()
{
	MyStack<int> stack(new ListDataContainer<int>());

	for (int i = 0; i < 10; ++i)
	{
		stack.push(i);
	}

	while (!stack.IsEmpty())
	{
		int n = stack.pop();
		std::cout << n << std::endl;
	}
}

void main()
{
	TEST_stack();
}