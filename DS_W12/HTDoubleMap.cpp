#include<iostream>
#include<string>
using namespace std;

enum class Status{NOITEM,ISITEM,AVAILABLE};

struct Entry {
	int key;
	string value;
	Status status;

	Entry():key(-1),status(Status::NOITEM){}
	Entry(int key,string& value):key(key),value(value),status(Status::ISITEM){}
	void erase() { status = Status::AVAILABLE; }
};

class HTDoubleMap {
private:
	int n;
	int N;
	Entry* bucket;
	int divisor;

	int hash_code(int key);
	int compress(int hash);
	int hash2(int hash);
	Entry* find_entry(int key);
public:
	HTDoubleMap(int N, int divisor);
	~HTDoubleMap();
	int size();
	bool empty();
	string find(int key);
	bool insert(int key, string value);
	bool erase(int key);
};

HTDoubleMap::HTDoubleMap(int N,int divisor):n(0),N(N),bucket(new Entry[N]), divisor(divisor) {}

HTDoubleMap::~HTDoubleMap() { delete[] bucket; }

int HTDoubleMap::size() { return n; }

bool HTDoubleMap::empty() { return size() == 0; }

string HTDoubleMap::find(int key) {
	Entry* entry = find_entry(key);
	if (entry == nullptr)return "";
	return entry->value;
}

bool HTDoubleMap::insert(int key, string value) {
	if (size() >= N)return false;
	int hash = hash_code(key);
	int idx = compress(hash);
	int probe = 1;
	Entry* first_available = nullptr;
	while (probe <= N) {
		Entry& cur = bucket[idx];
		if (cur.status == Status::NOITEM) {
			if (!first_available)first_available = &cur;
			break;
		}
		if (cur.status == Status::ISITEM && key == cur.key)return false;
		if (cur.status == Status::AVAILABLE && !first_available)first_available = &cur;

		idx = compress(idx + hash2(hash));
		++probe;
	}
	if (!first_available)return false;
	*first_available = Entry(key, value);
	++n;
	return true;
}

bool HTDoubleMap::erase(int key) {
	Entry* entry = find_entry(key);
	if (entry == nullptr)return false;
	entry->erase();
	--n;
	return true;
}

int HTDoubleMap::hash_code(int key) { return key; }

int HTDoubleMap::compress(int hash) { return hash % N; }

int HTDoubleMap::hash2(int hash) { return divisor - (hash % divisor); }

Entry* HTDoubleMap::find_entry(int key) {
	int hash = hash_code(key);
	int idx = compress(hash);
	int probe = 1;
	while (probe <= N && bucket[idx].status != Status::NOITEM) {
		if (bucket[idx].status == Status::ISITEM && key == bucket[idx].key)return &bucket[idx];

		idx = compress(idx + hash2(hash));
		++probe;
	}
	return nullptr;
}