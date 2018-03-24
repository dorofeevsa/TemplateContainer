#pragma once
#include <stdlib.h>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <string>
#include <ctime>

using std::string;
using std::cout;


template <class TData>
class customIterator
{

private:

	
public:
	
	typedef TData value_type;
	typedef ptrdiff_t difference_type;
	typedef const TData& reference;
	typedef const TData* pointer;
	typedef size_t size_type;
	typedef std::random_access_iterator_tag iterator_category;

	customIterator(TData*p)
	{
		itPtr = p;
	}

	customIterator(const customIterator &it)
	{
		itPtr = it.itPtr;
	}


	customIterator()
	{
		itPtr = 0;
	}

	customIterator operator+ (const size_type& i)
	{
		return customIterator(itPtr + i);

	}
	customIterator operator+= (const size_type& i)
	{
		itPtr += i;
		return *this;
	}
	customIterator& operator ++()
	{
		++itPtr;
		return *this;
	}
	
	
	customIterator operator-= (const size_type& i)
	{
		itPtr -= i;
		return *this;
	}

	
	customIterator& operator --()
	{
		--itPtr;
		return *this;
	}
		
	customIterator operator --(int i)
	{
		customIterator Temp(*this);
		--(*this);
		return Temp;
	}
	
	customIterator operator -(const difference_type& idx) 
	{
		customIterator Tmp(*this);
		return Tmp -= idx;
	}

	difference_type operator -(const customIterator& Other) const
	{
		return itPtr - Other.itPtr;
	}
	
	
	customIterator& operator =(const customIterator& Other)
	{
		itPtr  = Other.itPtr;
		return *this;
	}

	
	bool operator<(const customIterator &other) const
	{
		return itPtr < other.itPtr;
	}
	bool operator>(const customIterator &other) const
	{
		return itPtr > other.itPtr;
	}
	bool operator!=(const customIterator &other) const
	{
		return (!(*this == other));
	}
	bool operator==(const customIterator &other) const
	{
		return itPtr == other.itPtr;
	}


	reference operator [](const size_type& idx) 
	{
		return *(itPtr + idx);
	}

	pointer operator ->() 
	{
		return &**itPtr;
	}

	typename customIterator::reference operator*()
	{
		return *itPtr;
	}

private:

	TData * itPtr;
};




template <class TData>
class cArray
{
public: 
	typedef customIterator<TData> iterator;
	
/* little hint
	typedef IterType value_type;
	typedef ptrdiff_t difference_type;
	typedef const IterType& reference;
	typedef const IterType* pointer;
	typedef size_t size_type;
	typedef random_access_iterator_tag iterator_category;
*/
		// default c-tor
	cArray();

	// c-tor with size reserve
	cArray(
		unsigned int _size
	);
	// copy c-tor
	cArray(
		const cArray & _array
	);

	// destructor
	~cArray();

	// 
	void push_back(
		const TData & _value
	);

	// insert in index place
	void insert(
		unsigned int _index,
		const TData & _value
	);

	// erasing element
	void erase(
		unsigned int _index
	);

	// erasing all container
	void clear();

	//  get size of container
	unsigned int size() const;

	// random access
	TData & operator[](
		unsigned int _index
		);

	void sort(bool(*compFunc)(TData, TData) = NULL); //TODO: refact, or delete

	iterator begin()
	{
		return customIterator<TData>(stPtr);
	
	}
	iterator end()
	{
		return customIterator<TData>(backIt);
	
	}

	//========== custom Iterator section 

	

	//========== end custom iterator section

protected: // Attributes
	TData * stPtr; // start pointer
	TData* backIt; // back pointer
	unsigned int count; // current  elements count
	unsigned int capacity; // max capacity before reallocate

	//  reallocate for new capacity
	void reallocate(
		unsigned int _newCapacity
	);
};

template<class TData>
cArray<TData>::cArray()
{
	// def ctor  - reserve size for 4  elements
	stPtr = (TData*)malloc(sizeof(TData) * 4);
	if (stPtr == NULL) throw std::bad_alloc();
	backIt = stPtr;
	capacity = 4;
	count = 0;
}

template<class TData>
cArray<TData>::cArray(unsigned int _size)
{
	// ctor - reserve size for current count of elements
	stPtr = (TData*)malloc(sizeof(TData) * _size);
	if (stPtr == NULL) throw std::bad_alloc();
	backIt = stPtr;
	capacity = _size;
	count = 0;
}
template<class TData>
cArray<TData>::cArray(
	const cArray & _array
)
{// copy ctor
	unsigned int size = _array.size();
	if (size > 0)
	{
		stPtr = (TData*)malloc(sizeof(TData) * size);
		if (stPtr == NULL) throw std::bad_alloc();
		cArray *arr = const_cast<cArray*>(&_array);
		for (uint32_t i = 0; i < size; i++)
		{
			new(stPtr + i)TData((*arr)[i]); // by-element coping
		}

		backIt = stPtr + (size);
		capacity = count = size;
	}
	else
	{
		// ???
	}

}

template<class TData>
void cArray<TData>::reallocate(unsigned int _newCapacity)
{//
	stPtr = (TData*)realloc(stPtr, sizeof(TData) * _newCapacity);
	if (stPtr == NULL) throw std::bad_alloc();
	capacity = _newCapacity;
}

template<class TData>
void cArray<TData>::push_back(const TData & _value)
{
	if (count == capacity)
	{ //
		reallocate(capacity + capacity / 2);
		backIt = stPtr + count;
	}

	new(backIt)TData(_value); // 
	count++;
	backIt++;
}

template<class TData>
TData & cArray<TData>::operator[](unsigned int index)
{
	if (index < count)
	{
		TData* ptr = stPtr;
		ptr += index;
		return (TData&)(*(ptr));
	}
	else
	{
		throw std::out_of_range("Index is out of range!");
	}
}

template<class TData>
unsigned int cArray<TData>::size() const
{
	return count;
}


template<class TData>
void cArray<TData>::insert(
	unsigned int _index,
	const TData & _value
)
{
	if (_index < count)
	{
		//check that insert is possible
		//if not, reallocate
		if (count == capacity)
		{
			reallocate(capacity + capacity / 2);
			backIt = stPtr + count;
		}

		//
		TData* fromIt;
		TData* toIt;
		TData* last = stPtr + count;
		new(last)TData();

		//
		for (uint32_t i = count; i != _index; i--)
		{
			toIt = stPtr + i;
			fromIt = stPtr + i - 1;
			*toIt = *fromIt;
		}
		//
		toIt = stPtr + _index;
		*toIt = _value;
		count++;
		backIt++;
	}
	else
	{
		throw std::out_of_range("Index is out of range!");
	}

}

template<class TData>
void cArray<TData>::erase(
	unsigned int _index
)
{
	if (_index < count)
	{
		//
		TData* fromIt;
		TData* toIt;
		count--;
		backIt--;
		//
		for (uint32_t i = _index; i != count; i++)
		{
			toIt = stPtr + i;
			fromIt = stPtr + i + 1;
			*toIt = *fromIt;
		}
	}
	else
	{
		throw std::out_of_range("Index is out of range!");
	}
}

// 
template<class TData>
void cArray<TData>::clear()
{
	for (uint32_t i = 0; i < count; i++)
	{
		TData* ptr = ((stPtr + i)); // 
		ptr->~TData();
	}

	count = 0;
	backIt = stPtr;
}

template<class TData>
cArray<TData>::~cArray()
{
	clear();// 
	realloc(stPtr, 0); // 
}

template<class TData>
void cArray<TData>::sort(bool(*compFunc)(TData a, TData b))
{	//

	if (compFunc == NULL)std::sort(stPtr, stPtr + count);
	else
	{
		//
		std::sort(stPtr, stPtr + count, compFunc);
	}
}
/*
bool customStringLengthSort(
	string _a,
	string _b
)
{
	//
	return _a.length() < _b.length();
};
*/

/*
template<class type>
void containerTestOutput(
	cArray<type> &_arr
)
{
	// 
	cout << "\ncontainer: ";
	for (uint16_t i = 0; i < _arr.size(); i++)
	{
		cout << _arr[i] << " ";
	}
}
*/
/*
void integerTest(
	cArray<int>& _arr
)
{
	cout << "===========INTEGER TEST============\n";

	srand(unsigned int(time(0)));
	cout << "\n\n20 random numbers from 0 to 100 added in array:";
	for (unsigned int i = 0; i < 20; i++) _arr.push_back(rand() % 100);
	containerTestOutput(_arr);

	cout << "\n\nSorting upper:";
	_arr.sort();
	containerTestOutput(_arr);


	cout << "\n\nRemove every 2nd element:";
	for (unsigned int i = 0; i<_arr.size(); i++)
	{
		if (i + 1<_arr.size()) _arr.erase(i + 1);
	}
	containerTestOutput(_arr);


	cout << "\n\nInsert 10 random numbers in random position:\n ";
	unsigned int arraySize = _arr.size() - 1;
	for (unsigned int i = 0; i < 10; i++)
	{
		_arr.insert(unsigned int(rand() % arraySize), unsigned int((rand() % 100) + 1));
	}
	containerTestOutput(_arr);


	cout << "\n\nClear array:\n";
	_arr.clear();
	containerTestOutput(_arr);

	cout << "\n\n===========END OF INTEGER TEST============\n\n";
}
*/
/*
string randomWordGenerator(int minLength, int maxLength)
{
	string word;
	char tempChar;
	for (unsigned int i = 1; i < unsigned int((rand() % maxLength) + minLength); i++)
	{
		tempChar = rand() % (122 - 97) + 97;
		word += tempChar;
	}
	return word;
}
*/
/*
void stringTest(
	cArray<string>& _arr
)
{
	cout << "\n\n===========STRING TEST============\n\n";
	srand(unsigned int(time(0)));
	cout << "\n\nADD RANDOM WORDS TO ARRAY:";


	string tempstr;
	for (unsigned int randWordCount = 0; randWordCount < 15; randWordCount++)
	{
		tempstr = randomWordGenerator(3, 5);
		_arr.push_back(tempstr);
		tempstr = "";
	}

	containerTestOutput(_arr);

	cout << "\n\nSORTING UPPER DEFAULT(by first character):";

	_arr.sort();
	containerTestOutput(_arr);

	cout << "\n\nSORTING UPPER WITH CUSTOM COMPARE FUNCTION(by word length):";
	_arr.sort(customStringLengthSort);
	containerTestOutput(_arr);

	cout << "\n\nREMOVE WORDS, WHO INCLUDES 'A-E' SYMBOLS:";

	for (unsigned int i = 0; i < _arr.size(); i++)
	{
		if (std::strpbrk(_arr[i].c_str(), "abcde") != NULL) { _arr.erase(i); i = -1; }
	}

	containerTestOutput(_arr);

	cout << "\n\nADD 3 RANDOM WORD:";
	for (unsigned int randWordCount = 0; randWordCount < 3; randWordCount++)
	{
		tempstr = randomWordGenerator(1, 5);

		_arr.insert((rand() % (_arr.size() - 1)), tempstr);
		tempstr = "";
	}

	containerTestOutput(_arr);

	cout << "\n\n===========END OF STRING TEST============\n\n";
}
*/

//============ ITERATOR REALESE============

/*
template<class TData>
customIterator::customIterator(const customIterator &it)
{
	
}

template<class TData>
bool customIterator<TData>::operator<(customIterator const &other) const
{
}

template<class TData>
bool customIterator<TData>::operator>(customIterator const &other) const
{
}


template<class TData>
bool customIterator<TData>::operator!=(customIterator const &other) const
{
}

template<class TData>
bool customIterator<TData>::operator==(customIterator const &other) const
{
}

template<class TData>
customIterator<TData> &customIterator<TData>::operator++()
{
	++itPtr;
	return *this;
}

template<class TData>
typename cArray::customIterator<TData>::reference operator*()const
{
	
}
*/



