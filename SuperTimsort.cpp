#include<iostream>

using namespace std;
enum EWhatMerge{
	WM_NO_MERGE,
	WM_MERGE_XY,
	WM_MERGE_YZ
};

class TimSortParams{
	virtual unsigned int minRun(int len) = 0;
	virtual bool needMerge(unsigned int len_x, unsigned int len_y) = 0;
	virtual EWhatMerge WhatNeedMerge(unsigned int len_x, unsigned int len_y, unsigned int len_z) = 0;
	virtual unsigned int getGallop() = 0;

};
class TimSortParamsDefault:public TimSortParams{
	//реализованные функции
};
template<typename RandomAccessIterator, typename Compare = ...>
void TimSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp = Compare(), const TimsortParams* const params = &TimSortParamsDefault)
{

}
int main(){
	return 0;

}
// В параметрах сорта добавляется параметр const TimSortParams* const params
// 1 конст значит мы не меняем значение по указателю, а второй - сам указатель
/*
EWhatMerge WhatNeedMerge = <params>
whatNeedMerge(...)
{
	if (WhatMerge == WM_NO_MERGE)
		continue;
	else
	if (....)
		merge(X, Y)...

}
*/
