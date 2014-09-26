#include <vector>
#include<iostream>
#include<ctime>
#include<stack>
#include<algorithm>
#include <fstream>
#define N 10000
using namespace std;
inline int GetMinrun(int n){
	int r = 0;
	while (n >= 64) {
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}

template <typename RandomAccessIterator, typename Compare>
void in_merge(RandomAccessIterator begin,RandomAccessIterator nextbegin, RandomAccessIterator last, Compare comp){

	
		vector<typename std::iterator_traits<RandomAccessIterator>::value_type> Temp;
		Temp.resize(nextbegin - begin);
		int i = 0;
		RandomAccessIterator FirstArrayIterator = begin,
			TempArrayIterator = Temp.begin(),
			SecondArrayIterator = nextbegin,
			Start;
		copy(begin, nextbegin, Temp.begin());
		FirstArrayIterator = begin;
		int CountOfComparisons[2] = { 0, 0 };
		
		int j = 1, degree;
		TempArrayIterator = Temp.begin();
		FirstArrayIterator = begin;
		for (; TempArrayIterator != Temp.end() && SecondArrayIterator != last; )
		{
		//	cout << "i`m here\n";
			if (CountOfComparisons[0] == 7)
			{
				Start = TempArrayIterator;
				++TempArrayIterator;
			
				while (TempArrayIterator != Temp.begin())
				{
					if (comp(*TempArrayIterator, *SecondArrayIterator))
					{
						
						degree = (int)pow(2, j);
						cout << degree << endl;
						if (TempArrayIterator + degree - Temp.begin()< Temp.end() - Temp.begin()){

							TempArrayIterator += degree;
							j++;
						}
						else
						{
							copy(Start, TempArrayIterator - degree, FirstArrayIterator);
							CountOfComparisons[0] = 0;
							break;
						}
					}
					else
					{
						copy(Start, TempArrayIterator - degree, FirstArrayIterator);
						CountOfComparisons[0] = 0;
						break;
					}

				}
			}/*
			if (CountOfComparisons[1] == 7)
			{
				Start = TempArrayIterator;
				++TempArrayIterator;

				while (SecondArrayIterator != last)
				{
					if (!comp(*TempArrayIterator, *SecondArrayIterator))
					{

						degree = (int)pow(2, j);
						if (TempArrayIterator + degree < Temp.end() - Temp.begin()){

							TempArrayIterator += degree;
							j++;
						}
					}
					else
					{
						copy(Start, TempArrayIterator - degree, FirstArrayIterator);
					}

				}
			}*/
			if (comp(*TempArrayIterator, *SecondArrayIterator))
			{
				*FirstArrayIterator = *TempArrayIterator;
				++FirstArrayIterator;
				++TempArrayIterator;
			
				CountOfComparisons[0] ++;
				CountOfComparisons[1] = 0;
			}
			else
			{
				*FirstArrayIterator = *SecondArrayIterator;
				++FirstArrayIterator; ++SecondArrayIterator;
				
				CountOfComparisons[1]++;
				CountOfComparisons[0] = 0;
			}
			//cout << "Bu\n";

		}
		
		for (; TempArrayIterator != Temp.end(); TempArrayIterator++)
		{
			*FirstArrayIterator = *TempArrayIterator;
			FirstArrayIterator++;
		}
		

		for (; SecondArrayIterator != last; SecondArrayIterator++)
		{
			*FirstArrayIterator = *SecondArrayIterator;
			FirstArrayIterator++;
		}
		

}


template <typename RandomAccessIterator, typename Compare>
void mergeend(stack<pair<RandomAccessIterator, int> > &StackOfRuns, Compare comp){
	pair<RandomAccessIterator, int> x, y;
	x = StackOfRuns.top();
	StackOfRuns.pop();

	y = StackOfRuns.top();
	StackOfRuns.pop();
	in_merge(y.first, x.first, x.first + x.second, comp);
	//inplace_merge(y.first, x.first, x.first + x.second);
}

template <typename RandomAccessIterator, typename Compare>
void merge(stack<pair<RandomAccessIterator, int> > &StackOfRuns, Compare comp){
	pair<RandomAccessIterator, int> x, y, z;
	x = StackOfRuns.top();
	StackOfRuns.pop();

	y = StackOfRuns.top();
	StackOfRuns.pop();
	
	z = StackOfRuns.top();
	StackOfRuns.pop();

	if (z.second > y.second + x.second && y.second > z.second)
		return;
	
	if (z.second <= x.second + y.second){
		if (x.second < z.second)
		{
			StackOfRuns.push(z);
			in_merge(y.first, x.first, x.first + x.second, comp);
			//inplace_merge(y.first, x.first, x.first + x.second);
			StackOfRuns.push(make_pair(y.first, x.second + y.second));
		}
		else{

			in_merge(z.first, y.first, y.first + y.second, comp);
			//inplace_merge(z.first, y.first, y.first + y.second);
			StackOfRuns.push(make_pair(z.first, z.second + y.second));
			StackOfRuns.push(x);
		}
	}
	else if (y.second <= x.second)
	{
		StackOfRuns.push(z);
		in_merge(y.first, x.first, x.first + x.second, comp);
		//inplace_merge(z.first, y.first, y.first + y.second);
		StackOfRuns.push(make_pair(y.first, x.second + y.second));
	}
	
	
}


template <typename RandomAccessIterator, typename Compare>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	if (!(first < last))
		return;

	for (RandomAccessIterator i = first + 1; i != last; ++i)
	for (RandomAccessIterator j = i; j != first && comp(*j, *(j-1)); --j)
		std::iter_swap(j - 1, j);
}
template <typename RandomAccessIterator, typename Compare>
void makerun(vector<RandomAccessIterator> &PointersAtRuns, int MinRun, int &CountOfRuns, RandomAccessIterator &RunIterator, RandomAccessIterator last, Compare comp){
	bool done = false;
	if (RunIterator + 1 != last){
		PointersAtRuns.push_back(RunIterator);
		CountOfRuns++;
		RunIterator++;

		while (RunIterator != last){   //Decreasing sequence
			if (!comp(*RunIterator, *(RunIterator + 1)))
				RunIterator++;
			else {
				done = true;
				reverse(PointersAtRuns[CountOfRuns - 1], RunIterator);
				break;
			}
		}
		while (RunIterator != last && !done){   //Increasing sequence
			if (!comp(*RunIterator, *(RunIterator + 1)))
				RunIterator++;
			else {
				if (RunIterator - PointersAtRuns[CountOfRuns - 1] >= MinRun)
				reverse(PointersAtRuns[CountOfRuns - 1], RunIterator);
				break;
			}
		}
	} else {
		RunIterator++;
		PointersAtRuns.push_back(RunIterator);
		return;
	}

	//Filling the rest of Run
	while ((RunIterator - PointersAtRuns[CountOfRuns - 1]) < MinRun && RunIterator != last)
		RunIterator++;
	return;
}

bool IntCompare(int &a, int &b){
	return a <= b;
}
template <typename RandomAccessIterator, typename Compare>
void TimSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	int VectorLength = last - first,
		MinRun = GetMinrun(VectorLength),
		CountOfRuns = 0,
		c = 0;
	
	if (MinRun == VectorLength){
		insertion_sort(first, last, comp);
		return;
	}
	
	vector<RandomAccessIterator> PointersAtRuns;
	

	RandomAccessIterator RunIterator = first;
	stack<pair<RandomAccessIterator, int> > StackOfRuns;
	
	for (; RunIterator != last;)
	{
		makerun(
			PointersAtRuns,
			MinRun,
			CountOfRuns,
			RunIterator,
			last,
			comp);

		insertion_sort(PointersAtRuns[CountOfRuns - 1], RunIterator, comp);
		StackOfRuns.push(make_pair(PointersAtRuns[CountOfRuns - 1], RunIterator - PointersAtRuns[CountOfRuns - 1]));
		/*
		if (StackOfRuns.size() > 2){
			merge(StackOfRuns,comp);
		}
		*/
	}
	while (StackOfRuns.size() > 2)
		merge(StackOfRuns, comp);
	
	mergeend(StackOfRuns, comp);
	
}

int main()
{
	//freopen("txt.txt", "w", stdout);
	//freopen("txt1.txt", "r", stdin);
	vector<int> a, b;
	srand(111);
	vector<int> ::iterator it;
	clock_t time;
	a.clear();
	b.clear();
	a.resize(N);
	b.resize(N);
	for (int i = 0; i < N; i++)
	{
		a[i] = rand();
		b[i] = a[i];
	}
	
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
	
	for (int i = 0; i < N; i++)
	{
		if (a[i] == b[i])
			continue;
		else
			cout << "BAD\n";
	}
	
	getchar();
	return 0;
}

