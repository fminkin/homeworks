#include<iostream>
#include<vector>
#include<algorithm>

long long factorial(long long n)
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}
#define UI unsigned int
UI random(){
	return r(generator);
}

void do_vector_without_reps(vector<UI> &v, UI n, uniform_int_distribution<UI> Function){
	for (vector<UI>::iterator i = v.begin(); i != v.end(); i++)
		*i = Function(generator);
	v.erase(unique(v.begin(),v.end()), v.end());
	while (v.size() !=  n){
		for (vector<UI>::iterator i = v.begin() + v.size(); i != v.end(); i++)
			*i = Function(generator);
		v.erase(unique(v.begin(), v.end()), v.end());
	}
}
class Test{
private:
	std::vector<UI> keys;
	vector<UI>base;
	UI n;
	PerfectHashTable t;
public:
	Test(){};
	
	void init(PerfectHashTable b, UI size){
		n = size;
		t = b;
		cout << "Testing Inits:\n";
		cout << endl;
		test_with_reps();
		test_without_reps();
		ispossible();
		has_insert_erase();
	}
	
	void ispossible(){
		set<UI> true_keys;
		keys.resize(n);
		do_vector_without_reps(keys, n, r);
		for (size_t i = 0; i < n; i++)
			true_keys.insert(keys[i]);
		cout << "Testing true keys:\n";
		t.init(keys);
		for (set<UI>::iterator i = true_keys.begin(); i != true_keys.end(); ++i){
			if (!t.isPossibleKey(*i)){
				cout << "Incorrect\n";
				return;
			}
		}

		cout << "Correct\n";
		vector<UI> false_keys(n);
		cout << "Testing false keys:\n";
		generate_n(false_keys.begin(), n, random);
		for (size_t i = 0; i < n; ++i){
			if (true_keys.find(false_keys[i]) == true_keys.end() && t.isPossibleKey(false_keys[i])){
				cout << "Incorrect\n";
				return;
			}
		}
		cout << "Correct\n";
		t.clear();
	}
	void has_insert_erase(){

	}
	void do_all_permutations(bool rep){
		int count_of_ok_iter = 0, count_of_permutations = 0;
		sort(keys.begin(), keys.end());
		do{
			t.init(keys);
			if (myex.happen)
				count_of_ok_iter++;
			t.clear();
			count_of_permutations++;
		} while (next_permutation(keys.begin(), keys.end()));
		if (rep){
			if (count_of_ok_iter == count_of_permutations)
				cout << "Correct\n";
			else
				cout << "Incorrect\n";
		}
		else{
			if (t.check())
				cout << "Correct\n";
			else
				cout << "Incorrect\n";
		}
		t.clear();
	}
	void do_random_shuffle(bool rep){
		random_shuffle(keys.begin(), keys.end());
		t.init(keys);
		if (rep){
			if (myex.happen)
				cout << "Correct\n";
			else
				cout << "Incorrect\n";
		}
		else{
			if (t.check())
				cout << "Correct\n";
			else
				cout << "Incorrect\n";
		}
		t.clear();
	}
	void choose_operation(bool rep = true){
		if (n < 11){
			cout << "Testing all permutations:\n";
			do_all_permutations(rep);
		}
		else{
			cout << "Testing random shuffle:\n";
			do_random_shuffle(rep);
		}
		t.clear();
	}
	void do_test_with_shift(UI shift){
		generate_n(keys.begin(), n - shift, random);
		for (size_t i = 1; i <= shift; ++i){
			keys[n - i] = keys[i - 1];
		}
		choose_operation();
	}
	void test_with_reps(){
		int form;
		keys.resize(n);
		base.resize(n);
		base[0] = r(generator);
		cout << "Testing vector full oh h0\n";
		for (size_t i = 0; i < n; ++i){
			keys[i] = base[0];
		}
		t.init(keys);
		if (myex.happen)
			cout << "Correct\n";
		else
			cout << "Incorrect\n";
		t.clear();
		
		cout << endl;
		keys.clear();
		keys.resize(n);
		base[0] = r(generator);
		base[1] = r(generator);

		cout << "Testing vector: h0, h1, ...:\n";
		for (size_t i = 0; i < n; ++i){
			keys[i] = base[i % 2];
		}
		choose_operation();
		cout << endl;
		cout << "Testing vector: h0, h1, ... hn-2, h0:\n";
		do_test_with_shift(1);
		cout << endl;
		cout << "Testing vector: h0, h1, ... hn-3, h0, h1:\n";
		do_test_with_shift(2);
		cout << endl;	
	}
	void test_without_reps(){
		cout << "Testing vector without reps:\n";
		do_vector_without_reps(keys, n, r);
		choose_operation(false);
	}
};
