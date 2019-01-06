#pragma once

//
// MyList.h
// this is a template class
// all code is in this header file !!!

template <class T>
class MyList : public CList<T>
{
public:
	MyList();
	MyList(const MyList<T>& list);
	void Remove(T value);
	MyList<T> FilterIf(BOOL Predicate(T value)) const;
	int CountIf(BOOL Predicate(T value)) const;

};




template<class T>
MyList<T>::MyList()
{
}

template<class T>
MyList<T>::MyList(const MyList<T>& list)
{
}



template<class T>
void MyList<T>::Remove(T value)
{
}

template<class T>
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

template<class T>
int MyList<T>::CountIf(BOOL Predicate(T value)) const
{
	return 0;
}




