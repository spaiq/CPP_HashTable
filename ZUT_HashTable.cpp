#include <iostream>
#include <vector>
#include <forward_list>
#include <sstream>

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

	void put(std::string key, T value) {
		float cur_overflow = (float) mLength / mData.size();
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
		mLength++;

		return;
	}

	std::pair<std::string, T>* get(std::string key) {
		unsigned index = hashCode(key) % mData.size();
		for (auto& pair : mData[index]) {
			if (pair.first == key) {
				return &pair;
			}
		}
		return nullptr;
	}

	bool deleteElement(std::string key) {
		std::pair<std::string, T>* element = get(key);
		if (element) {
			unsigned index = hashCode(key) % mData.size();
			mData[index].remove(*element);
			mLength--;

			return true;
		}
		return false;
	}

	void clear() {
		mData.clear();
		mData.resize(4);
		mLength = 0;
	}

	std::string toString() {
		if (mLength == 0) return "Tree is empty";

		std::ostringstream stream;
		unsigned cnt = 0;
		bool exists = false;
		stream << "size: " << mLength
			<< "\nmax size: " << mData.size() << "\n";
		
		if (mLength > 10) {
			unsigned j = 0;
			for (const auto& list : mData) {
				if (j == 10) return stream.str();
				exists = false;
				for (const auto& [k, v] : list) {
					if (exists == false) stream << "[" << cnt << "]";
					exists = true;
					stream << "[" << k << ", " << v << "] ";
				}
				if (exists) {
					stream << std::endl;
					j++;
				}
				cnt++;
			}
		}
		for (const auto& list : mData) {
			exists = false;
			for (const auto& [k, v] : list) {
				if (exists == false) stream << "[" << cnt << "]";
				exists = true;
				stream << "[" << k << ", " << v << "] ";
			}
			if (exists) stream << std::endl;
			cnt++;
		}

		return stream.str();
	}

	std::string getStats() {
		int minDist = -1;
		unsigned maxDist = 0, nullLists = 0, totalSize = 0, size;
		std::ostringstream stream;

		for (const auto& list : mData) {
			size = distance(list.begin(), list.end());
			if (size < minDist && size > 0) {
				minDist = size;
			}
			if (size > maxDist) {
				maxDist = size;
			}
			if (size == 0) {
				nullLists++;
			}
			totalSize += size;
		}
		unsigned notNull = mData.size() - nullLists;
		double avgSize = (double) totalSize/mData.size();
		stream << "list minimum size: " << minDist
			<< "\nlist maximum size: " << maxDist
			<< "\nnon-null lists: " << notNull
			<< "\nlist avg size: " << avgSize;

		return stream.str();
	}

};
std::string randomKey(const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return tmp_s;
}

int main()
{
	/*hashTable<int> table;
	table.put("z", 1);
	table.put("y", 2);
	table.put("x", 3);
	table.put("w", 4);
	table.put("v", 5);
	table.put("u", 6);
	table.put("t", 7);
	table.put("s", 8);
	table.put("r", 9);
	table.put("q", 10);
	table.put("p", 11);
	table.put("o", 12);
	auto test = table.get("w");
	table.deleteElement("w");
	std::cout << table.toString();
	table.clear();*/
	srand(time(NULL));
	double mstimediff;
	const int MAX_ORDER = 7; // maksymalny rzad wielkosci dodawanych danych
	hashTable<int>* ht = new hashTable <int>(); // w tym przykladzie tablica mieszajaca par < string ,int > jako < klucz, wartosc >
		for (int o = 1; o <= MAX_ORDER; o++)
		{
			const int n = pow(10, o); // rozmiar danych
			// dodawanie do tablicy mieszajacej
			clock_t t1 = clock();
			for (int i = 0; i < n; i++)
				ht -> put(randomKey(6), i); // klucze losowe 6 - znakowe , a jako wartosci indeks petli
			clock_t t2 = clock();
			mstimediff = 1000 * (t2 - t1) / (double)CLOCKS_PER_SEC;
			std::cout << ht->toString()
				<< "\nPomiar 1 (dodawanie), rzedu " << o
				<< "\nCzas calkowity: " << mstimediff
				<< "ms\nCzas zamortyzowany: " << mstimediff / n << "ms\n\n"; // wypis na ekran aktualnej postaci tablicy mieszajacej ( skrotowej ) oraz pomiarow czasowych
				// wyszukiwanie w tablicy mieszajacej
				const int m = pow(10, 4);
			int hits = 0;
			t1 = clock();
			for (int i = 0; i < m; i++)
			{
				auto entry = ht -> get(randomKey(6)); // wyszukiwanie wg losowego klucza
				if (entry != NULL)
					hits++;
			}
			t2 = clock();
			mstimediff = 1000 * (t2 - t1) / (double)CLOCKS_PER_SEC;
			std::cout << "\nPomiar 2 (wyszukiwanie), rzedu " << o
				<< "\nCzas calkowity: " << mstimediff
				<< "ms\nCzas zamortyzowany: " << mstimediff / n << "ms"
				<< "\nTrafienia: " << hits << "\n\n"
				<< "Statystyki:\n" << ht->getStats()
				<< "\n\n"; // wypis na ekran pomiarow czasowych i liczby trafien
			ht->clear(); // czyszczenie tablicy mieszajacej
		}
	delete ht;
	return 0;
}
