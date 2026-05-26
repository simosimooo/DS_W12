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

	int hash_code(int key) { return key; }
	int compress(int hash) { return hash % N; }

	Entry* find_entry(int key) {
		int hash = hash_code(key);
		int idx = compress(hash);
		int probe = 1;
		while (probe <= N && bucket[idx].status != Status::NOITEM) {
			if (bucket[idx].status == Status::ISITEM && key == bucket[idx].key)return &bucket[idx];

			idx = compress(idx + 1);
			++probe;
		}
		return nullptr;
	}
	int pSum;
public:
	HTLinearMap(int N):n(0),N(N),bucket(new Entry[N]),pSum(0){}
	~HTLinearMap() { delete[]bucket; }
	int size() { return n; }
	void insert(int key, string value) {
		int hash = hash_code(key);
		int idx = compress(hash);
		int probe = 1;
		Entry* first_available = nullptr;
		int h = idx;
		int index = idx;
		if (size() >= N) {
			cout << h << ' ' << -1 << '\n'; return;
		}
		while (probe <= N) {
			Entry& cur = bucket[idx];
			if (cur.status == Status::NOITEM) {
				if (!first_available) {
					first_available = &cur;
					index = idx;
				}
				break;
			}
			if (cur.status == Status::ISITEM && key==cur.key) {
				cout << h << ' ' << -1 << '\n';
				pSum += probe;
				return;
			}
			if (cur.status == Status::AVAILABLE && !first_available) {
				first_available = &cur;
				index = idx;
			}
			idx = compress(idx + 1);
			++probe;
		}
		if (!first_available) { cout << h << ' ' << -1 << '\n'; return; }
		*first_available = Entry(key, value);
		cout << h << ' ' << index << '\n';
		pSum += probe;
		++n;
	}
	void erase(int key) {
		Entry* entry = find_entry(key);
		if (entry == nullptr) { cout << -1 << '\n'; return; }
		cout << entry->value << '\n';
		entry->erase();
		--n;
	}
	void find(int key) {
		Entry* entry = find_entry(key);
		if (entry == nullptr) { cout << -1 << '\n'; return; }
		cout << entry->value << '\n';
	}
	void get_pSum() { cout << pSum << '\n'; }
};

int main() {
	int T, N;
	cin >> T >> N;
	HTLinearMap	map(N);
	while (T--) {
		string cmd;
		cin >> cmd;
		if (cmd == "size")cout << map.size() << '\n';
		else if (cmd == "insert") {
			int k;
			string s;
			cin >> k >> s;
			map.insert(k, s);
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
	}
	map.get_pSum();
}