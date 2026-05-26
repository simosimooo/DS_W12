#include<iostream>
#include<string>
using namespace std;

enum class Status { OCCUPIED, EMPTY, DELETED };

struct Entry {
	int key;
	string value;
	Status status;

	Entry() :key(-1), value(""), status(Status::EMPTY) {}
	Entry(int key, string& value) :key(key), value(value), status(Status::OCCUPIED) {}
	void erase() { status = Status::DELETED; }
};

class HTLinearMap {
private:
	int n;
	int N;
	Entry* bucket;

	int pSum;

	int hash_code(int key) { return key; }
	int compress(int hash) { return hash % N; }
	Entry* find_entry(int key, bool cnt) {
		int hash = hash_code(key);
		int idx = compress(hash);
		int probe = 1;
		while (probe <= N && bucket[idx].status != Status::EMPTY) {
			if (bucket[idx].status == Status::OCCUPIED && key == bucket[idx].key) {
				if (cnt)pSum += probe;
				return &bucket[idx];
			}
			idx = compress(idx - 1 + N);
			++probe;
		}
		if (cnt)pSum += probe;
		return nullptr;
	}
public:
	HTLinearMap(int N) :n(0), N(N), bucket(new Entry[N]), pSum(0) {}
	~HTLinearMap() { delete[]bucket; }
	int size() { return n; }
	void stock(int key, string value) {
		int hash = hash_code(key);
		int idx = compress(hash);
		int probe = 1;
		Entry* avail = nullptr;
		while (probe <= N) {
			Entry& cur = bucket[idx];
			if (cur.status == Status::EMPTY) {
				if (!avail)avail = &cur;
				break;
			}
			if (cur.status == Status::OCCUPIED && key == cur.key)return;
			if (cur.status == Status::DELETED && !avail)avail = &cur;
			idx = compress(idx - 1 + N);
			++probe;
		}
		if (!avail)return;
		*avail = Entry(key, value);
		cout << probe << '\n';
		++n;
	}
	void release(int key) {
		Entry* entry = find_entry(key, false);
		if (entry == nullptr) { cout << "None\n"; return; }
		cout << entry->value << '\n';
		entry->erase();
		--n;
	}
	void check(int key) {
		Entry* entry = find_entry(key, false);
		if (entry == nullptr) { cout << "None\n"; return; }
		cout << entry->value << '\n';
	}
	void space() { cout << N - size() << '\n'; }
	void probe(int key) {
		pSum = 0;
		Entry* entry = find_entry(key, true);
		cout << pSum << '\n';

	}
};

int main() {
	int T;
	cin >> T;
	while (T--) {
		int Q, N;
		cin >> Q >> N;
		HTLinearMap map(N);
		while (Q--) {
			string cmd;
			cin >> cmd;
			if (cmd == "stock") {
				int k;
				string s;
				cin >> k >> s;
				map.stock(k, s);
			}
			else if (cmd == "release") {
				int k;
				cin >> k;
				map.release(k);
			}
			else if (cmd == "check") {
				int k;
				cin >> k;
				map.check(k);
			}
			else if (cmd == "space")map.space();
			else if (cmd == "probe") {
				int k;
				cin >> k;
				map.probe(k);
			}
		}

	}
}