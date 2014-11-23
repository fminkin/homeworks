#include<vector>
#include<list>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include <algorithm>
#include<random>
#include<exception>
using namespace std;
#define int128 unsigned long long
#define UI unsigned int
#define p 4294967311
std::default_random_engine generator;
std::uniform_int_distribution<UI> r(0, 4294967295);

class myexception{
public:
	UI happen;
	myexception(){ happen = 0; }
	void clear(){happen = 0; }
	void happend(){
		happen++;
	}
} myex;

class HashFunc{
private:
	UI a, b, n;
public:
	HashFunc(){};
	HashFunc(UI a, UI b, UI n) :a(a), b(b), n(n){};
	void init(UI n_a, UI n_b, UI n_n){
		a = n_a;
		b = n_b;
		n = n_n;
	}
	~HashFunc(){
		a = b = n = 0;
	}
	UI operator()(UI key){
		return (((a * key * 1ll + b * 1ll) % p) % n);
	}
};
class Hash_Element{
private:
	vector<pair<UI, bool>> element;//вектор из элементов, на самом деле там 1 элемент, вектор - для проверки
public:
	Hash_Element(){
	}
	Hash_Element(UI a){
		element.push_back(make_pair(a, false));
	}
	~Hash_Element(){
		element.clear();
	}
	void init(UI key){
		element.push_back(make_pair(key, false));
	}
	bool exist(){
		return element[0].second;
	}
	bool is_possible_key(UI key){
		return (key == element[0].first);
	}
	void erase(){
		element[0].second = false;
	}
	void insert(){
		element[0].first = true;
	}
	void check_repetitions(){
		for (size_t i = 0; i < element.size(); ++i){
			for (size_t j = i + 1; j < element.size(); j++){
				try{
					if (element[i] == element[j])
						throw myex;
				}
				catch (myexception& e){
					myex.happend();
					return;
				}
			}
		}
	}
	bool ok(){ // возвращает true, если в векторе 1 элемент и false иначе
		if (element.size() == 1 || !element.size())
			return true;
		else{
			check_repetitions();
			return false;
		}
	}
	bool check(){
		if (element.size() == 1 || !element.size())
			return true;
		else
			return false;
	}
};
class InternalHashTable{
private:
	vector<Hash_Element> InHash;
	HashFunc h_func;
public:
	InternalHashTable(){
		InHash.clear();
	}
	~InternalHashTable(){
		InHash.clear();
	}
	bool has_element(UI key){//вторые a, b коээф
		return InHash[h_func(key)].exist();
	}
	void insert_element(UI key){
		InHash[h_func(key)].insert();
	}
	void erase_element(UI key){
		InHash[h_func(key)].erase();
	}
	bool is_possible_key_element(UI key){
		return InHash[h_func(key)].is_possible_key(key);
	}
	bool is_bad(){
		for (size_t i = 0; i < InHash.size(); ++i){
			if (!InHash[i].ok() || myex.happen){
				return true;
			}
		}
		return false;
	}
	bool try_init(vector<UI> &keys, UI a, UI b){ //пытается инициализировать и возвращает true, если в каждом векторе 1 элемент и false в противном случае
		InHash.clear();
		InHash.resize(keys.size()*keys.size());
		h_func.init(a, b, keys.size() * keys.size());
		for (size_t i = 0; i < keys.size(); ++i){
			InHash[h_func(keys[i])].init(keys[i]);
		}
		if (!is_bad()){
			h_func.init(a, b, keys.size() * keys.size());
			return true;
		}
		else
			return false;
	}
	bool check(){
		for (size_t i = 0; i < InHash.size(); ++i)
		if (!InHash[i].check())
			return false;
		return true;
	}
};
class HashTable{
	UI actual_size, size;
	vector<InternalHashTable> hash;
	vector<vector<UI>> try_init_temp;
	HashFunc main_hash_func;
public:
	UI getconst(){
		return 3;
	}
	bool is_bad(){
		UI sum = 0;
		for (size_t i = 0; i < try_init_temp.size(); ++i){
			for (size_t j = 0; j < try_init_temp[i].size(); j++){
					try{	
						if (try_init_temp[i][j] == try_init_temp[i][(j + 1)%try_init_temp[i].size()])
							throw myex;
					}
					catch (myexception& e){
						myex.happend();
						return false;
					}
				}
			sum += try_init_temp[i].size() * try_init_temp[i].size();
		}
		if (sum < getconst()*size)
			return false;
		else
			return true;
	}

	bool try_init(vector<UI> &keys, UI a, UI b){
		try_init_temp.clear();
		size = keys.size();
		main_hash_func.init(a, b, size);
		try_init_temp.resize(size);
		for (size_t i = 0; i < size; ++i){
			try_init_temp[main_hash_func(keys[i])].push_back(keys[i]);
		}
		if (is_bad()){
			return false;
		}
		else{
			main_hash_func.init(a, b, size);
			return true;	
		}
	}
	void init(vector<UI> &keys){
		hash.resize(try_init_temp.size());
		for (size_t i = 0; i < try_init_temp.size(); ++i){
			UI a, b;
			a = 1 + r(generator);
			b = r(generator);
			while (!hash[i].try_init(try_init_temp[i], a, b) && !myex.happen){
				a = 1 + r(generator);
				b = r(generator);
			}
			if (myex.happen)
				return;
			actual_size = 0;
		}
	}
	void hash_table_erase(UI key){
		actual_size--;
		hash[main_hash_func(key)].erase_element(key);
	}
	void hash_table_insert(UI key){
		actual_size++;
		hash[main_hash_func(key)].insert_element(key);
	}
	bool hash_table_has(UI key){
		return hash[main_hash_func(key)].has_element(key);
	}
	bool is_possible_key(UI key){
		return hash[main_hash_func(key)].is_possible_key_element(key);
	}
	void clear(){
		actual_size = size = 0;
		try_init_temp.clear();
		hash.clear();
	}
	UI getsize(){
		return actual_size;
	}
	bool check(){
		for (size_t i = 0; i < hash.size(); ++i)
		if (!hash[i].check())
			return false;
		return true;
	}
};
class PerfectHashTable{
private:
	HashTable main;
public:
	PerfectHashTable(){
	};
	void init(vector<UI> &keys){
		UI a, b;	
		a = 1 + r(generator);
		b = r(generator) % (p);	
		while (!main.try_init(keys, a, b) && !myex.happen){
			a = 1 + r(generator);
			b = r(generator);
		}
		if (myex.happen)
			return;
		main.init(keys);
	}
	bool isPossibleKey(UI key){
		return main.is_possible_key(key);
	}
	bool has(UI key){
		return main.hash_table_has(key);
	}
	void insert(UI key){
		main.hash_table_insert(key); 
	}
	void erase(UI key){
		main.hash_table_erase(key);
	}
	UI size(){
		return main.getsize();
	}
	void clear(){
		main.clear();
		myex.clear();
	}		
	bool check(){
		return main.check();
	}
};
