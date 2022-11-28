#include <iostream>
#include <vector>
#include <forward_list>

template <typename S, typename T>
class hashTable {
private:
	std::vector<std::forward_list<std::pair<S, T>>> mData;
	unsigned mLength;
	const float cMax_overflow = 0.75;

	unsigned hashCode(std::string value) {
		unsigned hash = 0;
		for (int i = 0; i < value.length(); i++) {
			hash = 31 * hash + value[i];
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
};

int main()
{
	hashTable<std::string, int> table;
	table.append("abc", 3);
	table.append("abc", 12);
	return 0;
}
