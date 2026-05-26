#include<iostream>
#include<string>
using namespace std;

enum class Status{ISITEM,NOITEM,AVAILABLE};

int calKey(string id) {
	int res = 0;
	int p = 1;
	for (int i{ 0 }; i < id.length(); ++i) {
		res += (id[i] - 'a') * p;
		p *= 26;
	}
	return res;
}

struct Entry {
	string id;
	string pwd;
	Status status;
	bool online;

	Entry():id(""),status(Status::NOITEM){}
	Entry(string& id, string& pwd) :id(id), pwd(pwd), status(Status::ISITEM), online(false) {}
	void erase() { status = Status::AVAILABLE; }
};

class HTDoubleMap {
private:
	int n;
	int N;
	Entry* bucket;
	int divisor;

	int hash_code(int key) {
		return key % N;
	}
	int hash2(int key) {
		return divisor - (key % divisor);
	}
	Entry* find_entry(string id) {
		int key = calKey(id);
		int idx = hash_code(key);
		int probe = 1;
		while (probe <= N && bucket[idx].status != Status::NOITEM) {
			if (bucket[idx].status == Status::ISITEM && bucket[idx].id == id)return &bucket[idx];
			idx = hash_code(idx + hash2(key));
			++probe;
		}
		return nullptr;
	}
public:
	HTDoubleMap(int N,int divisor):n(0),N(N),bucket(new Entry[N]),divisor(divisor){}
	~HTDoubleMap() { delete[]bucket; }
	int size() { return n; }
	void signup(string s, string p) {
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
			if (cur.status == Status::ISITEM && cur.id == s) {
				cout << "Invalid " << cur.pwd << '\n';
				return;
			}
			if (cur.status == Status::AVAILABLE && !avail)avail = &cur;
			idx = hash_code(idx + hash2(key));
			++probe;
		}
		if (!avail)return;
		*avail = Entry(s, p);
		cout << "Submit\n";
		++n;
	}
	void login(string s, string p) {
		Entry* entry = find_entry(s);
		if (entry == nullptr||p!=entry->pwd) { cout << "Invalid\n"; return; }
		if (entry->online) { cout << "Quit\n"; }
		else { entry->online = true; cout << "Submit\n"; }
	}
	void logout(string s) {
		Entry* entry = find_entry(s);
		entry->online = false;
		cout << "Submit\n";
	}
	void change(string s, string p) {
		Entry* entry = find_entry(s);
		entry->pwd = p;
		cout << "Submit\n";
	}
};

int main() {
	int T;
	cin >> T;
	HTDoubleMap	map(500009,200003);
	while (T--) {
		string cmd;
		cin >> cmd;
		if (cmd == "signup") {
			string s, p;
			cin >> s >> p;
			map.signup(s, p);
		}
		else if (cmd == "login") {
			string s, p;
			cin >> s >> p;
			map.login(s, p);
		}
		else if (cmd == "logout") {
			string s;
			cin >> s;
			map.logout(s);
		}
		else if (cmd == "change") {
			string s, p;
			cin >> s >> p;
			map.change(s, p);
		}
	}
}