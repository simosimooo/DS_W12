#include<iostream>
#include<string>
using namespace std;

enum class Status{ISITEM,NOITEM,AVAILABLE};

struct Entry {
	int key;
	string value;
	Status status;

	Entry():key(-1),status(Status::NOITEM){}
	Entry(int key,string& value):key(key),value(value),status(Status::ISITEM){}
	void erase() { status = Status::AVAILABLE; }
};

class HTLinearMap {
private:
	int n;
	int N;
	Entry* bucket;

	int hash_code(int key) { return key % N; }
	Entry* find_entry(int key) {
		int idx = hash_code(key);
		int probe = 1;
		while (probe <= N && bucket[idx].status != Status::NOITEM) {
			if (bucket[idx].status == Status::ISITEM && bucket[idx].key == key)return &bucket[idx];
			idx = hash_code(idx + 1);
			++probe;
		}
		return nullptr;
	}
public:
	HTLinearMap(int N):n(0),N(N),bucket(new Entry[N]){}
	~HTLinearMap() { delete[]bucket; }
	int size() { return n; }
	void add(int x, string s) {
		if (size() >= N)return;
		int idx = hash_code(x);
		int probe = 1;
		Entry* first_available = nullptr;
		while (probe <= N) {
			Entry& cur = bucket[idx];
			if (cur.status == Status::NOITEM) {
				if (!first_available)first_available = &cur;
				break;
			}
			if (cur.status == Status::ISITEM && x == cur.key)return;
			if (cur.status == Status::AVAILABLE && !first_available)first_available = &cur;
			idx = hash_code(idx + 1);
			++probe;
		}
		if (!first_available)return;
		*first_available = Entry(x, s);
		++n;
	}
	void Delete(int x) {
		Entry* entry = find_entry(x);
		if (entry == nullptr)return;
		entry->erase();
		--n;
	}
	void name(int x) {
		Entry* entry = find_entry(x);
		cout << entry->value << '\n';
	}
	void number(int s) {

	}
};