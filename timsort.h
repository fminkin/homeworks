#include<vector>

enum EWhatMerge{
	WM_NO_MERGE,
	WM_MERGE_XY,
	WM_MERGE_YZ
};
template <typename RandomAccessIterator>
class Run{
public:
	RandomAccessIterator begin;
	unsigned int size;
	Run(RandomAccessIterator iter, unsigned int s){
		size = s;
		begin = iter;
	}
	Run() {};
};
template<class Compare>
class ReverseComparator{
public:
	Compare comp;
	template<class ElementType>
	bool operator() (ElementType &first, ElementType &second) 
	{
		return comp(second, first);
	}
};

class TimSortParams{
public:
	virtual unsigned int minRun(int len) const = 0;
	virtual bool needMerge(unsigned int len_x, unsigned int len_y) const = 0;
	virtual EWhatMerge WhatNeedMerge(unsigned int len_x, unsigned int len_y, unsigned int len_z) const = 0;
	virtual unsigned int getGallop() const = 0;

};

class TimSortParamsDefault :public TimSortParams{
public:
	unsigned int minRun(int len) const{
		int r = 0;
		while (len >= 64) {
			r |= len & 1;
			len >>= 1;
		}
		return len + r;
	}
	bool needMerge(unsigned int len_x, unsigned int len_y)const {
		if (len_y <= len_x)
			return true;
		else
			return false;
	}
	EWhatMerge WhatNeedMerge(unsigned int len_x, unsigned int len_y, unsigned int len_z)const {
		if (len_z <= len_x + len_y)
		{
			if (len_z < len_x)
				return WM_MERGE_YZ;
			else
				return WM_MERGE_XY;
		}
		else if (len_y <= len_x){
			return WM_MERGE_XY;
		}
		else
			return WM_NO_MERGE;
	}
	unsigned int getGallop() const {
		return 7;
	}
};

template<class T>
class MyStack{
private:
	std::vector<T> stack_base;
public:
	MyStack() {};
	~MyStack() {
		stack_base.clear();
	}

	unsigned int size(){
		return stack_base.size();
	}

	void push_back(T a){
		stack_base.push_back(a);
	}

	T operator [](int diff){
		return stack_base[stack_base.size() + diff - 1];
	}

	void pop_previous_elements(unsigned int count){
		for (size_t i = 0; i < count; i++)
			stack_base.erase(stack_base.begin() + stack_base.size() - 2);
	}
};
template<typename RandomAccessIterator, typename Compare>
void sequence(RandomAccessIterator &run_iterator, RandomAccessIterator last, Compare comp){
	for (; run_iterator != last; ++run_iterator){
		if (!comp(run_iterator[-1], run_iterator[0])){
			continue;
		}
		else
			break;
	}
}
template<typename RandomAccessIterator, typename Compare>
void makerun(RandomAccessIterator& last_pointer, int min_run, RandomAccessIterator &run_iterator, RandomAccessIterator last, Compare comp){
	bool is_decreasing = false;

	last_pointer = run_iterator;
	++run_iterator;
	if (run_iterator != last){
		if (!comp(run_iterator[-1], run_iterator[0])){
			is_decreasing = true;
			++run_iterator;
		}
	}

	if (run_iterator != last && is_decreasing){
		++run_iterator;
	}
	for (; run_iterator != last; ++run_iterator){
		if (!comp(run_iterator[-1], run_iterator[0])){
			continue;
		}
		else
			break;
	}

	sequence(run_iterator, last, comp);

	if (is_decreasing){
		reverse(last_pointer, run_iterator);
	}
	if (!is_decreasing){
		sequence(run_iterator, last, ReverseComparator<decltype(comp)>());
	}
	
	RandomAccessIterator insertionsort_begin = run_iterator - 1;
	while (run_iterator - last_pointer < min_run && run_iterator != last)
		++run_iterator;
	insertion_sort(last_pointer, insertionsort_begin, run_iterator, comp);
}

template <typename RandomAccessIterator, typename Compare>
void my_merge(RandomAccessIterator &begin, RandomAccessIterator &nextbegin, RandomAccessIterator &last, Compare comp, const TimSortParams* const params = &TimSortParamsDefault()){
	vector<typename std::iterator_traits<RandomAccessIterator>::value_type> temp(nextbegin - begin);
	copy(begin, nextbegin, temp.begin());

	vector<typename std::iterator_traits<RandomAccessIterator>::value_type>::iterator temp_array_iterator = temp.begin();
	RandomAccessIterator first_array_iterator = begin,
		second_array_iterator = nextbegin;

	for (; temp_array_iterator != temp.end() && second_array_iterator != last;){
		if (comp(*temp_array_iterator, *second_array_iterator)){
			*first_array_iterator = *temp_array_iterator;
			++first_array_iterator;
			++temp_array_iterator;
		} else {
			*first_array_iterator = *second_array_iterator;
			++first_array_iterator; ++second_array_iterator;
		}
		//addgallop
	}
	
	for (; temp_array_iterator!= temp.end(); ++temp_array_iterator){
		*first_array_iterator = *temp_array_iterator;
		++first_array_iterator;
	}
	temp.clear();
}

template <typename RandomAccessIterator, typename Compare>
void in_merge(RandomAccessIterator &begin, RandomAccessIterator &nextbegin, RandomAccessIterator &last, Compare comp, const TimSortParams* const params = &TimSortParamsDefault()){
	if (nextbegin - begin <= last - nextbegin){
		my_merge(begin, nextbegin, last, comp, params);
	} else {
		typedef std::reverse_iterator<RandomAccessIterator> RIter;
		my_merge(RIter(last), RIter(nextbegin), RIter(begin), ReverseComparator<decltype(comp)>(), params);
	}
}
template<typename RandomAccessIterator, typename Compare>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator sequence, RandomAccessIterator last, Compare comp){
	if (!(sequence < last))
		return;
	for (RandomAccessIterator i = sequence-1; i != last; ++i){
		for (RandomAccessIterator j = i; j != first && comp(*j, *(j - 1)); --j)
			std::iter_swap(j - 1, j);
	}
}
template<typename RandomAccessIterator, typename Compare>
void merge(int count_of_segments, MyStack<Run<RandomAccessIterator>> &stack_of_runs, Compare comp, const TimSortParams* const params = &TimSortParamsDefault(), bool end = false){

	if (count_of_segments == 2 || end){
		in_merge(stack_of_runs[-1].begin, stack_of_runs[0].begin, stack_of_runs[0].begin + stack_of_runs[0].size, comp, params);
		stack_of_runs.push_back(Run<RandomAccessIterator>(stack_of_runs[-1].begin, stack_of_runs[0].size + stack_of_runs[-1].size));
		stack_of_runs.pop_previous_elements(2);
		return;
	}

	
	switch (params->WhatNeedMerge(stack_of_runs[0].size, stack_of_runs[-1].size, stack_of_runs[-2].size)){
	case WM_NO_MERGE:
		return;
	case WM_MERGE_XY:
		in_merge(stack_of_runs[-1].begin, stack_of_runs[0].begin, stack_of_runs[0].begin + stack_of_runs[0].size, comp, params);
		stack_of_runs.push_back(Run<RandomAccessIterator>(stack_of_runs[-1].begin, stack_of_runs[0].size + stack_of_runs[-1].size));
		stack_of_runs.pop_previous_elements(2);
		break;
	case WM_MERGE_YZ:
		Run<RandomAccessIterator> temp;
		in_merge(stack_of_runs[-2].begin, stack_of_runs[-1].begin, stack_of_runs[-1].begin + stack_of_runs[-1].size, comp, params);
		temp = stack_of_runs[0];
		stack_of_runs.push_back(Run<RandomAccessIterator>(stack_of_runs[-2].begin, stack_of_runs[-2].size + stack_of_runs[-1].size));
		stack_of_runs.pop_previous_elements(3);
		stack_of_runs.push_back(temp);
		break;
	}
}

template<typename RandomAccessIterator, typename Compare>
void start_merge(bool end, MyStack<Run<RandomAccessIterator>> &stack_of_runs, Compare comp, const TimSortParams* const params = &TimSortParamsDefault()){
	while (stack_of_runs.size() != 1){
		if (stack_of_runs.size() == 2){
			if (end){
				merge(2, stack_of_runs, comp, params);
				return;
			}
			else {
				if (params->needMerge(stack_of_runs[0].size, stack_of_runs[-1].size)){
					merge(2, stack_of_runs, comp, params);
				}
				else {
					return;
				}
			}
		} else {
			if (end){
				if (params->WhatNeedMerge(stack_of_runs[0].size, stack_of_runs[-1].size, stack_of_runs[-2].size) == WM_NO_MERGE){
					merge(2, stack_of_runs, comp, params, true);
				} else {
					merge(3, stack_of_runs, comp, params);
				}
			} else {
				if (params->WhatNeedMerge(stack_of_runs[0].size, stack_of_runs[-1].size, stack_of_runs[-2].size) == WM_NO_MERGE)
					return;

				merge(3, stack_of_runs, comp, params);
			}
		}
	}
}

template<typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void TimSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp = Compare(), const TimSortParams* const params = &TimSortParamsDefault()){
	int vector_length = last - first,
		min_run = params->minRun(vector_length);
	RandomAccessIterator last_pointer, run_iterator = first;
	MyStack<Run<RandomAccessIterator>> stack_of_runs;

	for (; run_iterator != last;){
		makerun(
			last_pointer,
			min_run,
			run_iterator,
			last,
			comp);
	
		stack_of_runs.push_back(Run<RandomAccessIterator>(last_pointer, run_iterator - last_pointer));
		start_merge(false, stack_of_runs, comp, params);
	}
	
	start_merge(true, stack_of_runs, comp, params);
	//final_merge(stack_of_runs, comp);
}
