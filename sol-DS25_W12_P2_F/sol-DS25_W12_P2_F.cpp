#include<iostream>
#include<string>
using namespace std;

enum class Status{ISITEM,NOITEM,AVAILABLE};

int calKey(string name) {
	int res = 0;
	int p = 1;
	for (int i{ 0 }; i < name.length(); ++i) {
		res += (name[i] - 'a') * p;
		p *= 26;
	}
	return res;
}

struct Entry {
	int key;
	string value;
	Status status;

	Entry():key(-1),status(Status::NOITEM){}
	Entry(string& value):key(calKey(value)),value(value),status(Status::ISITEM){}
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
	HTLinearMap(int N) :n(0), N(N), bucket(new Entry[N]) {}
	~HTLinearMap() { delete[]bucket; }
	int size() { return n; }
	void insert(string value) {
		if (size() >= N)return;
		int key = calKey(value);
		int idx = hash_code(key);
		int probe = 1;
		Entry* first_available = nullptr;
		while (probe <= N) {
			Entry& cur = bucket[idx];
			if (cur.status == Status::NOITEM) {
				if (!first_available)first_available = &cur;
				break;
			}
			if (cur.status == Status::ISITEM && key == cur.key)return;
			if (cur.status == Status::AVAILABLE && !first_available)first_available = &cur;
			idx = hash_code(idx + 1);
			++probe;
		}
		if (!first_available)return;
		*first_available = Entry(value);
		++n;
	}
	bool exist(string value) {
		int key = calKey(value);
		Entry* entry = find_entry(key);
		if (entry == nullptr)return false;
		return true;
	}
};

int main() {
	int N;
	cin >> N;
	HTLinearMap map(500009);
	string corona;
	cin >> corona;
	map.insert(corona);
	while (N--) {
		string n1, n2;
		cin >> n1 >> n2;
		if (map.exist(n1) || map.exist(n2)) {
			if (!map.exist(n1))map.insert(n1);
			if (!map.exist(n2))map.insert(n2);
		}
	}
	cout << map.size() << '\n';
}