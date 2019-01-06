#include "stdafx.h"
#include "MyList.h"



template<typename T>
MyList<T>::MyList()
{
}

template<typename T>
MyList<T>::MyList(const MyList<T>& list)
{
}



template<typename T>
void MyList<T>::Remove(T value)
{
}

template<typename T>
MyList<T> MyList<T>::FilterIf(BOOL Predicate(T value)) const
{
	List<T> result;
	for (POSITION position = GetHeadPosition();
		position != NULL; GetNext(position))
	{
		T value = GetAt(position);
		if (Predicate(value))
		{
			result.AddTail(value);
		}
	}
	return result;
	
}

template<typename T>
int MyList<T>::CountIf(BOOL Predicate(T value)) const
{
	return 0;
}


