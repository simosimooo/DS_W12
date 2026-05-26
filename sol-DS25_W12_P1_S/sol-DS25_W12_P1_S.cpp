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
	int pSum;

	int hash_code(int key) { return key % N; }
	int hash2(int key) { return divisor - (key % divisor); }
	Entry* find_entry(int key, bool cnt) {
		int idx = hash_code(key);
		int probe = 1;
		while (probe <= N && bucket[idx].status != Status::NOITEM) {
			if (bucket[idx].status == Status::ISITEM && bucket[idx].key == key) {
				if(cnt)pSum += probe;
				return &bucket[idx];
			}
			idx = hash_code(idx + hash2(key));
			++probe;
		}
		if(cnt)pSum += probe;
		return nullptr;
	}
public:
	HTDoubleMap(int N,int divisor):n(0),N(N),bucket(new Entry[N]),divisor(divisor),pSum(0){}
	~HTDoubleMap() { delete[]bucket; }
	int size() { return n; }
	void insert(int k, string s) {
		int idx = hash_code(k);
		int hash = idx;
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
			if (cur.status == Status::ISITEM && k == cur.key) {
				cout << hash << ' ' << -1 << '\n';
				return;
			}
			if (cur.status == Status::AVAILABLE && !avail) {
				avail = &cur;
				index = idx;
			}
			idx = hash_code(idx + hash2(k));
			++probe;
		}
		if (!avail) { cout << hash << ' ' << -1 << '\n'; return; }
		*avail = Entry(k, s);
		cout << hash << ' ' << index << '\n';
		++n;
	}
	void erase(int k) {
		Entry* entry = find_entry(k,false);
		if (entry == nullptr) { cout << -1 << '\n'; return; }
		cout << entry->value << '\n';
		entry->erase();
		--n;
	}
	void find(int k) {
		Entry* entry = find_entry(k,true);
		if (entry == nullptr) { cout << -1 << '\n'; return; }
		cout << entry->value << '\n';
	}
	void get_pSum() { cout << pSum << '\n'; }
};
int main() {
	int T, N, M;
	cin >> T >> N>>M;
	HTDoubleMap map(N, M);
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