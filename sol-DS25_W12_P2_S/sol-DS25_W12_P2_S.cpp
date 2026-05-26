#include<iostream>
#include<string>
using namespace std;

enum class Status{ISITEM,NOITEM,AVAILABLE};

int calKey(string s) {
	int res = 0;
	int p = 1;
	for (int i{ 0 }; i < s.length(); ++i) {
		res += (s[i] - 'a') * p;
		p *= 26;
	}
	return res;
}
struct Entry {
	int key;
	string value;
	Status status;
	
	Entry():key(-1),status(Status::NOITEM){}
	Entry(string& value) :value(value), status(Status::ISITEM) {
		key=calKey(value);
	}
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
	~HTLinearMap() { delete[] bucket; }
	int size() { return n; }
	void insert(string s) {
		if (size() >= N)return;
		int key = calKey(s);
		int idx = hash_code(key);
		int probe = 1;
		Entry* avail = nullptr;
		while (probe <= N) {
			Entry& cur = bucket[idx];
			if (cur.status == Status::NOITEM) {
				if (!avail)avail = &cur;
				break;
			}
			if (cur.status == Status::ISITEM && key == cur.key)return;
			if (cur.status == Status::AVAILABLE && !avail)avail = &cur;
			idx = hash_code(idx + 1);
			++probe;
		}
		if (!avail)return;
		*avail = Entry(s);
		cout << s << '\n';
		++n;
	}
	bool exist(string s) {
		int key = calKey(s);
		Entry* entry = find_entry(key);
		if (entry == nullptr)return false;
		return true;
	}
};

int main() {
	int N;
	cin >> N;
	string first_corona;
	cin >> first_corona;
	HTLinearMap map(500009);
	map.insert(first_corona);
	while (N--) {
		string n1, n2;
		cin >> n1 >> n2;
		if (map.exist(n1) || map.exist(n2)) {
			if (!map.exist(n1))map.insert(n1);
			if (!map.exist(n2))map.insert(n2);
		}
	}
}