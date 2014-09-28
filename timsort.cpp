#include <vector>
#include<iostream>
#include<ctime>
#include<stack>
#include<algorithm>
#include <fstream>
#define N 100000
using namespace std;
inline int GetMinrun(int n);

template <typename RandomAccessIterator>
void sorted(RandomAccessIterator first, RandomAccessIterator last);

template <typename RandomAccessIterator, typename Compare>
void in_merge(RandomAccessIterator begin, RandomAccessIterator nextbegin, RandomAccessIterator last, Compare comp);

template <typename RandomAccessIterator, typename Compare>
void mergeend(stack<pair<RandomAccessIterator, int> > &stack_of_runs, Compare comp);

template <typename RandomAccessIterator, typename Compare>
void merge(stack<pair<RandomAccessIterator, int> > &stack_of_runs, Compare comp);

template <typename RandomAccessIterator, typename Compare>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template <typename RandomAccessIterator, typename Compare>
void makerun(vector<RandomAccessIterator> &pointers_at_runs, int min_run, int &count_of_runs, RandomAccessIterator &run_iterator, RandomAccessIterator last, Compare comp);

bool IntCompare(int &a, int &b);

template <typename RandomAccessIterator, typename Compare>
void TimSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

int main(){
	srand(111);
	vector<int> a, b;
	
	a.resize(N);
	b.resize(N);
	for (int i = 0; i < N; i++)
	{
		a[i] = rand();
		b[i] = a[i];
	}
	clock_t time;
	time = clock();
	sort(b.begin(), b.end());
	time = clock() - time;
	double time_seconds = (double)time / CLOCKS_PER_SEC;

	cout << "Std::sort working time`s " << time_seconds << endl;

	time = clock();
	TimSort(a.begin(), a.end(), IntCompare);
	time = clock() - time;
	time_seconds = (double)time / CLOCKS_PER_SEC;
	cout << "TimSort working time`s " << time_seconds << endl;
	
	sorted(a.begin(), a.end());
		
	return 0;
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

template <typename RandomAccessIterator, typename Compare>
void in_merge(RandomAccessIterator begin, RandomAccessIterator nextbegin, RandomAccessIterator last, Compare comp){
	vector<typename std::iterator_traits<RandomAccessIterator>::value_type> temp;
	int i = 0, f, count_of_comparisons[2] = { 0, 0 }, power = 1, degree;
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
		if (count_of_comparisons[0] == 7){
			power = 1;
			degree = 0;
			start = temp_array_iterator;
			while (temp_array_iterator != temp.end()){
				if (comp(*temp_array_iterator, *second_array_iterator)){
					if(!degree)
					degree = 1;
					degree*=2;
					f = temp_array_iterator - temp.begin();
					if (f + degree < temp.size()){
						temp_array_iterator += degree;
					} else {
						if (start != temp_array_iterator)
							copy(start, temp_array_iterator, first_array_iterator);

						first_array_iterator += temp_array_iterator - start;
						count_of_comparisons[0] = 0;
						break;
					}
				} else {
					temp_array_iterator -= degree;
					if (start != temp_array_iterator)
						copy(start, temp_array_iterator, first_array_iterator);

					first_array_iterator += temp_array_iterator - start;
					count_of_comparisons[0] = 0;
					break;
				}
			}
		}
		if (count_of_comparisons[1] == 7){
			power = 1;
			degree = 0;
			start = second_array_iterator;
			while (second_array_iterator != last){
				if (!comp(*temp_array_iterator, *second_array_iterator)){
					if(!degree)
						degree = 1;
					degree*=2;
					f = second_array_iterator - begin;
					if (f + degree < last - nextbegin){
						second_array_iterator += degree;
					} else {
						if (start != second_array_iterator)
							copy(start, second_array_iterator, first_array_iterator);

						first_array_iterator += second_array_iterator - start;
						count_of_comparisons[1] = 0;
						break;
					}
				} else {
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
		} else {
			*first_array_iterator = *second_array_iterator;
			++first_array_iterator; ++second_array_iterator;

			count_of_comparisons[1]++;
			count_of_comparisons[0] = 0;
		}	
	}

	for (; temp_array_iterator != temp.end(); temp_array_iterator++){
		*first_array_iterator = *temp_array_iterator;
		first_array_iterator++;
	}


	for (; second_array_iterator != last; second_array_iterator++){
		*first_array_iterator = *second_array_iterator;
		first_array_iterator++;
	}
	temp.clear();
}

template <typename RandomAccessIterator, typename Compare>
void mergeend(stack<pair<RandomAccessIterator, int> > &stack_of_runs, Compare comp){
	pair<RandomAccessIterator, int> x, y;
	x = stack_of_runs.top();
	stack_of_runs.pop();
	y = stack_of_runs.top();
	stack_of_runs.pop();
	in_merge(y.first, x.first, x.first + x.second, comp);
	//inplace_merge(y.first, x.first, x.first + x.second);
}

template <typename RandomAccessIterator, typename Compare>
void merge(stack<pair<RandomAccessIterator, int> > &stack_of_runs, Compare comp){
	pair<RandomAccessIterator, int> x, y, z;
	x = stack_of_runs.top();
	stack_of_runs.pop();
	y = stack_of_runs.top();
	stack_of_runs.pop();
	z = stack_of_runs.top();
	stack_of_runs.pop();

	if (z.second > y.second + x.second && y.second > z.second)
		return;
	if (z.second <= x.second + y.second){
		if (x.second < z.second){
			stack_of_runs.push(z);
			in_merge(y.first, x.first, x.first + x.second, comp);
			//inplace_merge(y.first, x.first, x.first + x.second);
			stack_of_runs.push(make_pair(y.first, x.second + y.second));
		} else {
			in_merge(z.first, y.first, y.first + y.second, comp);
			//inplace_merge(z.first, y.first, y.first + y.second);
			stack_of_runs.push(make_pair(z.first, z.second + y.second));
			stack_of_runs.push(x);
		}
	} else if (y.second <= x.second){
		stack_of_runs.push(z);
		in_merge(y.first, x.first, x.first + x.second, comp);
		//inplace_merge(z.first, y.first, y.first + y.second);
		stack_of_runs.push(make_pair(y.first, x.second + y.second));
	}
}

template <typename RandomAccessIterator, typename Compare>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
	if (!(first < last))
		return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
	for (RandomAccessIterator j = i; j != first && comp(*j, *(j - 1)); --j)
		std::iter_swap(j - 1, j);
}

template <typename RandomAccessIterator, typename Compare>
void makerun(vector<RandomAccessIterator> &pointers_at_runs, int min_run, int &count_of_runs, RandomAccessIterator &run_iterator, RandomAccessIterator last, Compare comp){
	bool done = false;
	if (run_iterator + 1 != last){
		pointers_at_runs.push_back(run_iterator);
		count_of_runs++;
		run_iterator++;
		while (run_iterator != last){   //Decreasing sequence
			if (!comp(*run_iterator, *(run_iterator + 1))){
				run_iterator++;
			} else {
				done = true;
				reverse(pointers_at_runs[count_of_runs - 1], run_iterator);
				break;
			}
		}
		while (run_iterator != last && !done){   //Increasing sequence
			if (!comp(*run_iterator, *(run_iterator + 1))){
				run_iterator++;
			} else {
				if (run_iterator - pointers_at_runs[count_of_runs - 1] >= min_run)
					reverse(pointers_at_runs[count_of_runs - 1], run_iterator);
				break;
			}
		}
	} else {
		run_iterator++;
		pointers_at_runs.push_back(run_iterator);
		return;
	}
	//Filling the rest of Run
	while ((run_iterator - pointers_at_runs[count_of_runs - 1]) < min_run && run_iterator != last)
		run_iterator++;
	return;
}

template <typename RandomAccessIterator, typename Compare>
void TimSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
	int vector_length = last - first,
		min_run = GetMinrun(vector_length),
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
		insertion_sort(pointers_at_runs[count_of_runs - 1], run_iterator, comp);
		stack_of_runs.push(make_pair(pointers_at_runs[count_of_runs - 1], run_iterator - pointers_at_runs[count_of_runs - 1]));
		//if (stack_of_runs.size() > 2){
		//merge(stack_of_runs,comp);
		//}
	}

	while (stack_of_runs.size() > 2)
		merge(stack_of_runs, comp);
	mergeend(stack_of_runs, comp);
}

inline int GetMinrun(int n){
	int r = 0;
	while (n >= 64) {
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}

bool IntCompare(int &a, int &b){
	return a <= b;
}


