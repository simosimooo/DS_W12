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
			if (bucket[idx].status == Status::ISITEM && key == bucket[idx].key)return &bucket[idx];
			idx = hash_code(idx + 1);
			++probe;
		}
		return nullptr;
	}
public:
	HTLinearMap(int N):n(0),N(N),bucket(new Entry[N]){}
	~HTLinearMap() { delete[]bucket; }
	int size() { return n; }
	void put(int key, string value) {
		if (size() >= N)return;
		int idx = hash_code(key);
		int index = idx;
		int probe = 1;
		Entry* avail = nullptr;
		while (probe <= N) {
			Entry& cur = bucket[idx];
			if (cur.status == Status::NOITEM) {
				if (!avail) {
					avail = &cur;
					index = idx;
				}
				break;
			}
			if (cur.status == Status::ISITEM && key == cur.key)return;
			if (cur.status == Status::AVAILABLE && !avail) {
				avail = &cur;
				index = idx;
				break;
			}
			idx = hash_code(idx + 1);
			++probe;
		}
		if (!avail)return;
		*avail = Entry(key, value);
		cout << probe + index << '\n';
		++n;
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
	void vacant() { cout << N - size() << '\n'; }
};

int main() {
	int T, N;
	cin >> T >> N;
	HTLinearMap map(N);
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
		else if (cmd == "vacant") {
			map.vacant();
		}
	}
}