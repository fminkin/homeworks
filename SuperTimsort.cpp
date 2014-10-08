#include<iostream>
#include<vector> 
#include<stack>
#include<ctime>
#include<algorithm>
#include<string>
using namespace std;
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
	vector<T> stack_base;
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
		return stack_base[stack_base.size() +diff - 1];
	}
	void pop(){
		stack_base.erase(stack_base.begin() + stack_base.size() - 1);
		
	}
	void pop_for_xy(){
		for (size_t i = 0; i < 2; i++)
			stack_base.erase(stack_base.begin() + stack_base.size() - 2);
	}
	void pop_index(int index){
		stack_base.erase(stack_base.begin() + stack_base.size() - 1 + index);
	}
};
template<typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void makerun(RandomAccessIterator& last_pointer, int min_run, RandomAccessIterator &run_iterator, RandomAccessIterator last, Compare comp = Compare()){
	bool is_decreasing = false;

	last_pointer = run_iterator;
	++run_iterator;
	if (run_iterator != last){
		if (!comp(run_iterator[-1], run_iterator[0]))
		{
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
	if (is_decreasing){
		reverse(last_pointer, run_iterator);
	}
	for (; run_iterator != last && !is_decreasing; ++run_iterator){
		if (!comp(run_iterator[0], run_iterator[-1])){
			continue;
		}
		else
			break;
	}


	RandomAccessIterator insertionsort_begin = run_iterator;
	while (run_iterator - last_pointer < min_run && run_iterator != last)
		++run_iterator;
	insertion_sort(last_pointer, insertionsort_begin, run_iterator, comp);
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
void insertion_sort(RandomAccessIterator first, RandomAccessIterator sequence, RandomAccessIterator last, Compare comp = Compare()){
	if (!(first < last))
		return;
	for (RandomAccessIterator i = first; i != last; ++i)
	for (RandomAccessIterator j = i; j != first && comp(*j, *(j - 1)); --j)
		std::	iter_swap(j - 1, j);
}
template<typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void merge(int count_of_segments, MyStack<Run<RandomAccessIterator>> &stack_of_runs, Compare comp = Compare(), const TimSortParams* const params = &TimSortParamsDefault(), bool end = false){

	if (count_of_segments == 2 || end){
		in_merge(stack_of_runs[-1].begin, stack_of_runs[0].begin, stack_of_runs[0].begin + stack_of_runs[0].size, comp, params);
		stack_of_runs.push_back(Run<RandomAccessIterator>(stack_of_runs[-1].begin, stack_of_runs[0].size + stack_of_runs[-1].size));
		stack_of_runs.pop_index(-1);
		stack_of_runs.pop_index(-1);
		return;
	}
	

	switch (params->WhatNeedMerge(stack_of_runs[0].size, stack_of_runs[-1].size, stack_of_runs[-2].size)){
	case WM_NO_MERGE:
		return;
	case WM_MERGE_XY:
		in_merge(stack_of_runs[-1].begin, stack_of_runs[0].begin, stack_of_runs[0].begin + stack_of_runs[0].size, comp, params);
		stack_of_runs.push_back(Run<RandomAccessIterator>(stack_of_runs[-1].begin, stack_of_runs[0].size + stack_of_runs[-1].size));
		stack_of_runs.pop_index(-1);
		stack_of_runs.pop_index(-1);
		break;
	case WM_MERGE_YZ:
		Run<RandomAccessIterator> temp;	
		in_merge(stack_of_runs[-2].begin, stack_of_runs[-1].begin, stack_of_runs[-1].begin+ stack_of_runs[-1].size, comp, params);
		temp = stack_of_runs[0];
		stack_of_runs.push_back(Run<RandomAccessIterator>(stack_of_runs[-2].begin, stack_of_runs[-2].size + stack_of_runs[-1].size));
		stack_of_runs.pop_index(-1);
		stack_of_runs.pop_index(-1);
		stack_of_runs.pop_index(-1);
		stack_of_runs.push_back(temp);
		break;
	}

}

template<typename RandomAccessIterator, typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void start_merge(bool end, MyStack<Run<RandomAccessIterator>> &stack_of_runs, Compare comp = Compare(), const TimSortParams* const params = &TimSortParamsDefault()){
	unsigned int size;

	while (stack_of_runs.size() != 1){
		if (stack_of_runs.size() == 2)
		{
			if (end){
				merge(2, stack_of_runs, comp, params);
				return;
			}
			else {
				if (params->needMerge(stack_of_runs[0].size, stack_of_runs[-1].size)){
					merge(2, stack_of_runs, comp, params);
				}
				else
					return;
			}
		}
		else{
			if (end){
				if (params->WhatNeedMerge(stack_of_runs[0].size, stack_of_runs[-1].size, stack_of_runs[-2].size) == WM_NO_MERGE)
					merge(2, stack_of_runs, comp, params, true);
				else
					merge(3, stack_of_runs, comp, params);
			}
			else {
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
void getstring(string &s){
	string gs("qwertyuiopasdfghjklzxcvbnm");
	for (size_t i = 0; i < s.size(); ++i)
		s[i] = gs[rand() % 26];
}
/*
void gettest(vector<int>&a){
const int n = a.size() / 500;
for (vector<int>::iterator i = a.begin, j = i+n; j!= a.end; ++i)

}*/
int main(){
	//freopen("txt.txt", "r", stdin);
	//freopen("txt1.txt", "w", stdout);
	srand(123123123);
	const int N = 1000000, M = 1000;

	vector<int> a(N), b(N);
	for (int i = 0; i < N; i++){
		a[i] = rand();
		b[i] = a[i];
	}
	
	MyStack<Run<vector<int>::iterator>> f;
	Run<vector<int>::iterator> ff; 
	f.push_back(ff);
	cout << f.size() << endl;
	f.pop();
	cout << f.size() << endl;
	f.push_back(Run<vector<int>::iterator>(a.begin(), 3));
	cout << f[0].size << endl;

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
	
	system("PAUSE");
	return 0;

}
