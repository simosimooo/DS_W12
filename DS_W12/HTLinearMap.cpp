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

class HTLinearMap {
private:
	int n;
	int N;
	Entry* bucket;

	int hash_code(int key);
	int compress(int hash);
	Entry* find_entry(int key);
public:
	HTLinearMap(int N);
	~HTLinearMap();

	int size();
	bool empty();
	string find(int key);
	bool insert(int key, string value);
	bool erase(int key);
};

HTLinearMap::HTLinearMap(int N):n(0),N(N),bucket(new Entry[N]){}
HTLinearMap::~HTLinearMap() { delete[] bucket; }

int HTLinearMap::size() { return n; }
bool HTLinearMap::empty() { return size() == 0; }
string HTLinearMap::find(int key) {
	Entry* entry = find_entry(key);
	if (entry == nullptr)return "";
	return entry->value;
}
bool HTLinearMap::insert(int key, string value) {
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
		idx = compress(idx + 1);
		++probe;
	}
	if (!first_available)return false;
	*first_available = Entry(key, value);
	++n;
	return true;
}
bool HTLinearMap::erase(int key) {
	Entry* entry = find_entry(key);
	if (entry == nullptr)return false;
	entry->erase();
	--n;
	return true;	
}

int HTLinearMap::hash_code(int key) { return key; }
int HTLinearMap::compress(int hash) { return hash % N; }
Entry* HTLinearMap::find_entry(int key) {
	int hash = hash_code(key);
	int idx = compress(hash);
	int probe = 1;
	while (probe <= N && bucket[idx].status != Status::NOITEM) {
		if (bucket[idx].status==Status::ISITEM&&key == bucket[idx].key)return &bucket[idx];

		idx = compress(idx + 1);
		++probe;
	}
	return nullptr;
}