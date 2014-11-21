  #include<vector>
#include<list>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include <algorithm>
#include<random>
using namespace std;
#define int128 unsigned long long
#define UI unsigned int
#define p 4294967311
// a1  = a/2^32 
// a2 = a%2^32
UI gethash(UI a, UI b, UI key, UI n){
	return (((a * key*1ll+ b*1ll) % p) % n);
}
std::default_random_engine generator;
std::uniform_int_distribution<UI> r(0, 4294967295);
class Hash_Element{
private:
	vector<pair<UI, bool>> element;//вектор из элементов, на самом деле там 1 элемент, вектор - для проверки
public:
	Hash_Element(){	
	}
	Hash_Element(UI a){
		element.push_back(make_pair(a, false));
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
	bool ok(){ // возвращает true, если в векторе 1 элемент и false иначе
		if (element.size() == 1 || !element.size())
			return true;
		else
			return false;
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
	UI a, b;
public:
	InternalHashTable(){
		InHash.clear();
	}
	bool has_element(UI key){//вторые a, b коээф
		UI h = gethash(a, b, key, InHash.size());
		return InHash[h].exist();
	}
	void insert_element(UI key){
		UI h = gethash(a, b, key, InHash.size());
		InHash[h].insert();
	}
	void erase_element(UI key){
		UI h = gethash(a, b, key, InHash.size());
		InHash[h].erase();
	}
	bool is_possible_key_element(UI key){
		UI h = gethash(a, b, key, InHash.size());
		return InHash[h].is_possible_key(key);
	}
	bool is_bad(){
		for (size_t i = 0; i < InHash.size(); ++i){
			if (!InHash[i].ok()){
				return true;
			}
		}
		return false;
	}

	bool try_init(vector<UI> &keys, UI a_s, UI b_s){ //пытается инициализировать и возвращает true, если в каждом векторе 1 элемент и false в противном случае
		InHash.clear();
		InHash.resize(keys.size()*keys.size());
		a = a_s;
		b = b_s;
		UI h;
		for (size_t i = 0; i < keys.size()*keys.size(); ++i){	
			h = gethash(a, b, keys[i], keys.size()*keys.size());
			InHash[i].init(keys[i]);
		}
		if (!is_bad()){
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
	UI main_a, main_b; //главные a,b
	vector<InternalHashTable> hash;
	vector<vector<UI>> try_init_temp;

public:
	UI getconst(){
		return 3;
	}
	bool is_bad(UI n){
		UI sum = 0;
		for (size_t i = 0; i < try_init_temp.size(); ++i){

			for (int j = 0; j < try_init_temp[i].size(); j++){
				if (j + 1 < try_init_temp[i].size()){
					try{
						if (try_init_temp[i][j] == try_init_temp[i][j + 1])
							throw true;
					}//породить от ексепшн
					catch (bool e){
						cout << "SAME KEYS\n";
						exit(0);
					}
				}
			}
			sum += try_init_temp[i].size() * try_init_temp[i].size();
		}
		if (sum < getconst()*n)
			return false;
		else
			return true;
	}

	bool try_init(vector<UI> &keys, UI a_s, UI b_s){
		try_init_temp.clear();
		main_a = a_s;
		main_b = b_s;
		try_init_temp.resize(keys.size());
		UI h, n = keys.size();
		for (size_t i = 0; i < n; ++i){
			h = gethash(main_a, main_b, keys[i], n);
			try_init_temp[h].push_back(keys[i]);
		}
		if (is_bad(n))
			return false;
		else
			return true;
	}
	void init(vector<UI> &keys){
		hash.resize(try_init_temp.size());
		for (size_t i = 0; i < try_init_temp.size(); ++i){
			UI a, b;
			a = 1 + r(generator);
			b = r(generator);
			
			while (!hash[i].try_init(try_init_temp[i], a, b)){
				a = 1 + r(generator);
				b = r(generator);
			}
		}
	}
	void hash_table_erase(UI key, UI n){
		UI h = gethash(main_a, main_b, key, n);
		hash[h].erase_element(key);
	}
	void hash_table_insert(UI key, UI n){
		UI h = gethash(main_a, main_b, key, n);
		hash[h].insert_element(key);
	}
	bool hash_table_has(UI key, UI n){
		UI h = gethash(main_a, main_b, key, n);
		return hash[h].has_element(key);
	}
	bool is_possible_key(UI key, UI n){
		UI h = gethash(main_a, main_b, key, n);
		return hash[h].is_possible_key_element(key);
	}
	bool check(){
		for (size_t i = 0; i < hash.size(); ++i)
		if (!hash[i].check())
			return false;
		return true;
	}
	void clear(){
		main_a = 0;
		main_b = 0; 
		try_init_temp.clear();
		hash.clear();
	}
};

class PerfectHashTable{
private:
	HashTable main;
	UI n; 
public:
	PerfectHashTable(){};

	void init(vector<UI> &keys){
		srand(time(NULL));
		n = keys.size();
		UI a, b;
		
		a = 1 + r(generator);
		b = r(generator) % (p);
		while (!main.try_init(keys, a, b)){
			a = 1 + r(generator);
			b = r(generator);
		}
		main.init(keys);
	}
	bool isPossibleKey(UI key){
		return main.is_possible_key(key, n);
	}
	bool has(UI key){
		return main.hash_table_has(key, n);
	}
	void insert(UI key){
		main.hash_table_insert(key, n);
	}
	void erase(UI key){
		main.hash_table_erase(key, n);
	}
	unsigned int size(){

	}
	void clear(){
		n = 0;
		main.clear();
	}
	bool check(){
		return main.check();
	}
};
