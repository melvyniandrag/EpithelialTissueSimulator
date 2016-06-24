/*
* The coordinate class needed to be included somewhere. 
* I am putting it here, because cells use this class, and 
* since the file will be included in the main file, we will have 
* this class available there as well.
*/

#ifndef COORDINATE
#define COORDINATE
class coordinate
{
public:
	coordinate(int idx, bool t) : index(idx), IsInner(t){};
	coordinate(){index = -1; IsInner = 0;};
	int index;
	bool IsInner;
	inline bool operator==(const coordinate& rhs){return index == rhs.index;};
  void operator=(const coordinate &C)
  {
    index = C.index;
    IsInner = C.IsInner;
  }
};
#endif
