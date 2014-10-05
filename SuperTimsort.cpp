#include<iostream>
#include<vector> 
#include<stack>
#include<ctime>
#include<algorithm>
using namespace std;
enum EWhatMerge{
	WM_NO_MERGE,
	WM_MERGE_XY,
	WM_MERGE_YZ
};

int p = 0;
class TimSortParams{
public:
	virtual unsigned int minRun(int len) const = 0;
	virtual bool needMerge(unsigned int len_x, unsigned int len_y) const = 0;
	virtual EWhatMerge WhatNeedMerge(unsigned int len_x, unsigned int len_y, unsigned int len_z) const = 0;
	virtual unsigned int getGallop() const = 0;

};/*
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
		if (len_x <= len_y)
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
*/
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
		if (len_x <= len_y)
			return true;
		else
			return false;
	}
	EWhatMerge WhatNeedMerge(unsigned int len_x, unsigned int len_y, unsigned int len_z)const {
		if (len_x <= len_z + len_y || len_y <= len_z)
		{
			if (len_z < len_x)
				return WM_MERGE_YZ;
			else
				return WM_MERGE_XY;
		}
		else
			return WM_NO_MERGE;
	}

	unsigned int getGallop() const {
		return 7;
	}
};

template<typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void makerun(vector<RandomAccessIterator> &pointers_at_runs, int min_run, int &count_of_runs, RandomAccessIterator &run_iterator, RandomAccessIterator last, Compare comp = Compare()){
	bool done = false;
	if (run_iterator + 1 != last){
		pointers_at_runs.push_back(run_iterator);
		++count_of_runs;
		++run_iterator;
		while (run_iterator != last){   //Decreasing sequence
			if (!comp(*run_iterator, *(run_iterator + 1))){
				++run_iterator;
				done = true;
			}
			else if (*(run_iterator) == *(run_iterator + 1))
				++run_iterator;
			else
			{
				//cout << "decr ended : " << *run_iterator << " " << *(run_iterator + 1) << endl;
				reverse(pointers_at_runs[count_of_runs - 1], run_iterator);
				++ run_iterator;
				break;
			}
		}
		while (run_iterator != last && !done){   //Increasing sequence
			if (comp(*run_iterator, *(run_iterator + 1)) || *(run_iterator) == *(run_iterator+1)){
				
				++run_iterator;
			}
			else {	
				++run_iterator;
				//cout << "incr ended : "<<*run_iterator << " " << *(run_iterator + 1) << endl;
				break;
			}
		}
	}
	else {
		++run_iterator;
		pointers_at_runs.push_back(run_iterator);
		return;
	}
	//Filling the rest of Run
	while ((run_iterator - pointers_at_runs[count_of_runs - 1]) < min_run && run_iterator != last)
		++run_iterator;
	return;
}

template <typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void in_merge(RandomAccessIterator begin, RandomAccessIterator nextbegin, RandomAccessIterator last, Compare comp = Compare(), const TimSortParams* const params = &TimSortParamsDefault()){
	vector<typename std::iterator_traits<RandomAccessIterator>::value_type> temp;
	unsigned int i = 0, f, count_of_comparisons[2] = { 0, 0 }, power = 1, degree, gallop_constant = params->getGallop();
	RandomAccessIterator first_array_iterator = begin,
		temp_array_iterator = temp.begin(),
		second_array_iterator = nextbegin,
		start;

	temp.resize(nextbegin - begin);
	copy(begin, nextbegin, temp.begin());

	first_array_iterator = begin;
	temp_array_iterator = temp.begin();
	first_array_iterator = begin;

	for (; temp_array_iterator != temp.end() && second_array_iterator != last;){
		if (count_of_comparisons[0] == gallop_constant){
			power = 1;
			degree = 0;
			start = temp_array_iterator;
			while (temp_array_iterator != temp.end()){
				if (comp(*temp_array_iterator, *second_array_iterator)){
					if (!degree)
						degree = 1;
					degree *= 2;
					f = temp_array_iterator - temp.begin();
					if (f + degree < temp.size()){
						temp_array_iterator += degree;
					}
					else {
						if (start != temp_array_iterator)
							copy(start, temp_array_iterator, first_array_iterator);

						first_array_iterator += temp_array_iterator - start;
						count_of_comparisons[0] = 0;
						break;
					}
				}
				else {
					temp_array_iterator -= degree;
					if (start != temp_array_iterator)
						copy(start, temp_array_iterator, first_array_iterator);

					first_array_iterator += temp_array_iterator - start;
					count_of_comparisons[0] = 0;
					break;
				}
			}
		}
		if (count_of_comparisons[1] == gallop_constant){
			power = 1;
			degree = 0;
			start = second_array_iterator;
			while (second_array_iterator != last){
				if (!comp(*temp_array_iterator, *second_array_iterator)){
					if (!degree)
						degree = 1;
					degree *= 2;
					f = second_array_iterator - begin;
					if (f + degree < last - nextbegin){
						second_array_iterator += degree;
					}
					else {
						if (start != second_array_iterator)
							copy(start, second_array_iterator, first_array_iterator);

						first_array_iterator += second_array_iterator - start;
						count_of_comparisons[1] = 0;
						break;
					}
				}
				else {
					second_array_iterator -= degree;
					if (start != second_array_iterator)
						copy(start, second_array_iterator, first_array_iterator);

					first_array_iterator += second_array_iterator - start;
					count_of_comparisons[1] = 0;
					break;
				}
			}
		}

		if (comp(*temp_array_iterator, *second_array_iterator)){
			*first_array_iterator = *temp_array_iterator;
			++first_array_iterator;
			++temp_array_iterator;
			count_of_comparisons[0] ++;
			count_of_comparisons[1] = 0;
		}
		else {
			*first_array_iterator = *second_array_iterator;
			++first_array_iterator; ++second_array_iterator;

			++count_of_comparisons[1];
			count_of_comparisons[0] = 0;
		}
	}

	for (; temp_array_iterator != temp.end(); temp_array_iterator++){
		*first_array_iterator = *temp_array_iterator;
		++first_array_iterator;
	}


	for (; second_array_iterator != last; second_array_iterator++){
		*first_array_iterator = *second_array_iterator;
		++first_array_iterator;
	}
	temp.clear();
}

template<typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp = Compare()){
	if (!(first < last))
		return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
	for (RandomAccessIterator j = i; j != first && comp(*j, *(j - 1)); --j)
		std::iter_swap(j - 1, j);
}
template<typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void merge(int count_of_segments, stack<pair<RandomAccessIterator, int> > &stack_of_runs, Compare comp = Compare(), const TimSortParams* const params = &TimSortParamsDefault(), bool end = false){

	pair<RandomAccessIterator, int> x[3];
	for (int i = 0; i < count_of_segments; i++)
	{
		x[i] = stack_of_runs.top();
		stack_of_runs.pop();
	}
	if (count_of_segments == 2){
		if (params->needMerge(x[0].second, x[1].second) || end){
			in_merge(x[1].first, x[0].first, x[0].first + x[0].second, comp, params);
			stack_of_runs.push(make_pair(x[1].first, x[0].second + x[1].second));
			return;
		}
		else
			return;
	}
	if (end){
		switch (params->WhatNeedMerge(x[0].second, x[1].second, x[2].second)){
		case WM_NO_MERGE:
			stack_of_runs.push(x[2]);
			in_merge(x[1].first, x[0].first, x[0].first + x[0].second, comp, params);
			stack_of_runs.push(make_pair(x[1].first, x[0].second + x[1].second));
			break;
		case WM_MERGE_XY:
			stack_of_runs.push(x[2]);
			in_merge(x[1].first, x[0].first, x[0].first + x[0].second, comp, params);
			stack_of_runs.push(make_pair(x[1].first, x[0].second + x[1].second));
			break;
		case WM_MERGE_YZ:
			in_merge(x[2].first, x[1].first, x[1].first + x[1].second, comp, params);
			stack_of_runs.push(make_pair(x[2].first, x[2].second + x[1].second));
			stack_of_runs.push(x[0]);
			break;
		}
	}
	else
	{
		switch (params->WhatNeedMerge(x[0].second, x[1].second, x[2].second)){
		case WM_NO_MERGE:
			return;
		case WM_MERGE_XY:
			stack_of_runs.push(x[2]);
			in_merge(x[1].first, x[0].first, x[0].first + x[0].second, comp, params);
			stack_of_runs.push(make_pair(x[1].first, x[0].second + x[1].second));
			break;
		case WM_MERGE_YZ:
			in_merge(x[2].first, x[1].first, x[1].first + x[1].second, comp, params);
			stack_of_runs.push(make_pair(x[2].first, x[2].second + x[1].second));
			stack_of_runs.push(x[0]);
			break;
		}
	}
}

template<typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void start_merge( bool end, stack<pair<RandomAccessIterator, int> > &stack_of_runs, Compare comp = Compare(), const TimSortParams* const params = &TimSortParamsDefault()){
	while (stack_of_runs.size() != 1){
		if (stack_of_runs.size() == 2)
		{
			if (end)
				merge(2, stack_of_runs, comp, params, true);
			else
				return;
		}
		else{
			if (end){
				merge(3, stack_of_runs, comp, params, true);
			}
			else{
				merge(3, stack_of_runs, comp, params);
			}
		}
	} 
	
}

template<typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void TimSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp = Compare(), const TimSortParams* const params = &TimSortParamsDefault()){
	int vector_length = last - first,
		min_run = params->minRun(vector_length),
		count_of_runs = 0,
		c = 0;

	vector<RandomAccessIterator> pointers_at_runs;
	RandomAccessIterator run_iterator = first;
	stack< pair<RandomAccessIterator, int> > stack_of_runs;

	if (min_run == vector_length){
		insertion_sort(first, last, comp);
		return;
	}
	for (; run_iterator != last;){
		makerun(
			pointers_at_runs,
			min_run,
			count_of_runs,
			run_iterator,
			last,
			comp);
	//	cout << run_iterator - pointers_at_runs[count_of_runs - 1] << endl;
		insertion_sort(pointers_at_runs[count_of_runs - 1], run_iterator, comp);
		stack_of_runs.push(make_pair(pointers_at_runs[count_of_runs - 1], run_iterator - pointers_at_runs[count_of_runs - 1]));
		
	//	start_merge( false, stack_of_runs, comp, params);	
	}
	
	start_merge( true,stack_of_runs, comp, params);
	
}

template <typename RandomAccessIterator>
void sorted(RandomAccessIterator first, RandomAccessIterator last) {
	for (RandomAccessIterator i = first; i != last - 1; ++i)
	if (*i > *(i + 1)){
		cout << "Not sorted" << endl;
		return;
	}
	cout << "Sorted" << endl;
	return;
}
int main(){
	freopen("txt.txt", "w", stdout);
	freopen("txt1.txt", "r", stdin);
	srand(123123123);
	const int N = 100000;
	vector<int> a(N), b(N);
	for (int i = 0; i < N; i++){
		a[i] = rand();
		b[i] = a[i];
	}

	clock_t time;
	time = clock();
	TimSort(a.begin(), a.end());
	time = clock() - time;
	std::cout.precision(10);
	cout << "Timsort worked for: " << (double)time / CLOCKS_PER_SEC << endl;
	time = clock();
	sort(b.begin(), b.end());
	time = clock() - time;
	std::cout.precision(10);
	cout << "Std::sort worked for: " << (double)time / CLOCKS_PER_SEC << endl;
	sorted(a.begin(), a.end());

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
