#include <iostream>
#include <vector>
#include <forward_list>

template <typename T>
class hashTable {
private:
	std::vector<std::forward_list<std::pair<std::string, T>>> mData;
	unsigned mLength;
	const float cMax_overflow = 0.75;

	unsigned hashCode(std::string str) {
		unsigned hash = 0;
		for (int i = 0; i < str.length(); i++) {
			hash = 31 * hash + str[i];
		}
		return hash;
	}

	void rehash() {
		auto oldData = std::move(mData);
		mData.resize(oldData.size() * 2);
		for(int i = 0; i < oldData.size(); i++)
			for (auto& [k, v] : oldData[i]) {
				unsigned index = hashCode(k) % mData.size();
				mData[index].emplace_front(k, v);
			}

		return;
	}

public:
	hashTable() {
		mLength = 0;
		mData.resize(4);
	}

	~hashTable() {
		mLength = 0;
		mData.clear();
	}

	void append(std::string key, T value) {
		float cur_overflow = mLength / mData.size();
		if (cur_overflow >= cMax_overflow) {
			rehash();
		}
		unsigned index = hashCode(key) % mData.size();
		for (auto& [k, v] : mData[index]) {
			if (k == key) {
				v = value;
				return;
			}
		}
		mData[index].emplace_front(key, value);

		return;
	}

	std::pair<std::string, T>* findElement(std::string key) {
		unsigned index = hashCode(key) % mData.size();
		for (auto& pair : mData[index]) {
			if (pair.first == key) {
				return &pair;
			}
		}
		return nullptr;
	}

	bool deleteElement(std::string key) {
		std::pair<std::string, T>* element = findElement(key);
		if (element) {
			unsigned index = hashCode(key) % mData.size();
			mData[index].remove(*element);
			return true;
		}
		return false;
	}



};

int main()
{
	hashTable<int> table;
	table.append("z", 1);
	table.append("y", 2);
	table.append("x", 3);
	table.append("w", 4);
	table.append("v", 5);
	table.append("u", 6);
	auto test = table.findElement("w");
	table.deleteElement("w");
	return 0;
}
