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

class HTDoubleMap {
private:
	int n;
	int N;
	Entry* bucket;
	int divisor;

	int hash(int key) { return key % N; }
	int hash2(int hash) { return divisor - (hash % divisor); }
	Entry* find_entry(int key) {
		int idx = hash(key);
		int hash_code = idx;
		int probe = 1;
		while (probe <= N && bucket[idx].status != Status::NOITEM) {
			if (bucket[idx].status == Status::ISITEM && bucket[idx].key == key)return &bucket[idx];
			idx = (hash_code + probe * hash2(key)) % N;
			++probe;
		}
		return nullptr;
	}
public:
	HTDoubleMap(int N,int divisor):n(0),N(N),bucket(new Entry[N]),divisor(divisor){}
	~HTDoubleMap() { delete[]bucket; }
	int size() { return n; }
	void put(int key, string value) {
		int idx = hash(key);
		int h = idx;
		int insertIdx = idx;
		int probe = 1;
		Entry* first_available = nullptr;
		while (probe <= N) {
			Entry& cur = bucket[idx];
			if (cur.status == Status::NOITEM) {
				if (!first_available) {
					first_available = &cur;
					insertIdx = idx;
				}
				break;
			}
			if (cur.status == Status::ISITEM && key == cur.key) {
				cout << h << ' ' << -1 << '\n';
				return;
			}
			if (cur.status == Status::AVAILABLE && !first_available) {
				first_available = &cur;
				insertIdx = idx;
			}
			idx = (h + probe * hash2(key)) % N;
			++probe;
		}
		if (!first_available) { cout << h << ' ' << -1 << '\n'; return; }
		*first_available = Entry(key, value);
		++n;
		cout << h << ' ' << insertIdx << '\n';
	}
	void erase(int key) {
		Entry* entry = find_entry(key);
		if (entry == nullptr) { cout << "None\n"; return; }
		cout << entry->value << '\n';
		entry->erase();
		--n;
	}
	void find(int key) {
		Entry* entry = find_entry(key);
		if (entry == nullptr) { cout << "None\n"; return; }
		cout << entry->value << '\n';
	}
	void vacant() {
		cout << N - size() << '\n';
	}
};

int main() {
	int T, N, M;
	cin >> T >> N >> M;
	HTDoubleMap map(N, M);
	while (T--) {
		string cmd;
		cin >> cmd;
		if (cmd == "put") {
			int k;
			string s;
			cin >> k >> s;
			map.put(k, s);
		}
		else if (cmd == "erase") {
			int k;
			cin >> k;
			map.erase(k);
		}
		else if (cmd == "find") {
			int k;
			cin >> k;
			map.find(k);
		}
		else if (cmd == "vacant")map.vacant();
	}
}