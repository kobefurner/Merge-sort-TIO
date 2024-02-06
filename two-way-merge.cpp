#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <chrono>

using std::cout;
using std::cin;
using std::endl;
using std::string;

template <typename T>
class BaseSort {
public:
	BaseSort(const string& sortName, const unsigned int capacity);
	~BaseSort();
	string getName() { return sortName; }
	void loadRandomValues();
	void printValues() const;
	void verifySort() const;

	virtual void runSort() = 0; // Pure virtual function.  
	                            // It makes the class **abstract**.  In other words,
	                            // nothing can instantiate an object of this class.  
protected:
	string sortName;
	T* arr{ nullptr };
	unsigned int capacity{ 0 };
private:

};

template <typename T>
BaseSort<T>::BaseSort(const string& sortName, const unsigned int capacity) {
	this->sortName = sortName;
	this->capacity = capacity;
	this->arr = new T[capacity];
}
template <typename T>
BaseSort<T>::~BaseSort() {
	delete[] arr;
}

template <typename T>
void BaseSort<T>::loadRandomValues() {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<unsigned int> distrib(0, 0xffffffffui32);

	for (unsigned int i = 0; i < capacity; ++i) {
		this->arr[i] = distrib(gen);
	}
}

template <typename T>
void BaseSort<T>::printValues() const {
	if (capacity < 50) {
		for (unsigned int i = 0; i < capacity; i++) {
			cout << arr[i] << " ";
		}
		cout << endl;
	}
}

template <typename T>
void BaseSort<T>::verifySort() const {
	for (unsigned int i = 0; i < capacity - 1; i++) {
		if (arr[i + 1] < arr[i]) {
			cout << "Not sorted at index: " << (i + 1) << endl;
			return;
		}
	}
	cout << "Sorted!" << endl;
}

template <typename T>
class Bubble : public BaseSort <T> {
public:
	Bubble(const unsigned int capacity) : BaseSort<T>("Bubble", capacity) {}
	void runSort();
private:
};

template <typename T>
void Bubble<T>::runSort() {
	for (unsigned int round = 0; round < this->capacity - 1; round++) {
		for (unsigned int i = 0; i < this->capacity - 1 - round; i++) {
			if (this->arr[i + 1] < this->arr[i]) {
				T temp = this->arr[i];
				this->arr[i] = this->arr[i + 1];
				this->arr[i + 1] = temp;
			}
		}
	}
}

template <typename T>
class Selection : public BaseSort<T> {
public:
	Selection(const unsigned int capacity) : BaseSort<T>("Selection", capacity) {};
	void runSort();
private:
};

template <typename T>
void Selection<T>::runSort() {
	// The outer loop's job is to...
	// Each time the outer loop iterators, it works with the leftmost/unsorted index value
	for (unsigned int i = 0; i < this->capacity - 1; i++) {
		// The inner loop's job is to...  
		// Compare the current index to the rest of the unsorted region
		for (unsigned int j = i + 1; j < this->capacity; j++) {
			// We have an i (the index we hope to sort) and a j (a candidate to check)
			if (this->arr[j] < this->arr[i]) {
				T temp = this->arr[i];
				this->arr[i] = this->arr[j];
				this->arr[j] = temp;
			}
		}
	}
}

template <typename T>
class Insertion : public BaseSort<T> {
public:
	Insertion(const unsigned int capacity) : BaseSort<T>("Insertion", capacity) {};
	void runSort();
private:
};

template <typename T>
void Insertion<T>::runSort() {
	// The outer loop's job is to...
	// Obtain the leftmost unsorted value, iterates one index to the right each round.  Starts at 1
	for (unsigned int i = 1; i < this->capacity; i++) {
		// The inner loop's job is to...  
		// Take the leftmost unsorted value, walk to the left (back to index 0), swapping along the way until
		// no swaps are needed, or until we get to index 0.      
		unsigned int j = i;
		while (j > 0 && this->arr[j] < this->arr[j - 1]) {
			T temp = this->arr[j];
			this->arr[j] = this->arr[j - 1];
			this->arr[j - 1] = temp;
			j--;
		}
	}
}

template <typename T>
class Quick : public BaseSort<T> {
public:
	Quick(const unsigned int capacity) : BaseSort<T>("Quick", capacity) {};
	void runSort();
private:
	void runSort(unsigned int first, unsigned int last);
	unsigned int quickSortPartition(unsigned int first, unsigned int last);
};

template <typename T>
void Quick<T>::runSort() {
	runSort(0, this->capacity);
}

template <typename T>
void Quick<T>::runSort(unsigned int first, unsigned int last) {
	if (first < last) {
		// Obtain a pivot, move all values smaller to the left of pivot
		// and all values larger to the right of pivot
		unsigned int pivotLocation = quickSortPartition(first, last);
		runSort(first, pivotLocation);
		runSort(pivotLocation + 1, last);
	}
}

template <typename T>
unsigned int Quick<T>::quickSortPartition(unsigned int first, unsigned int last) {
	T pivot;
	unsigned int index;
	unsigned int smallIndex;
	T temp;
	pivot = this->arr[first];
	smallIndex = first;
	for (index = first + 1; index < last; index++) {
		if (this->arr[index] < pivot) {
			smallIndex++;
			//swap
			temp = this->arr[smallIndex];
			this->arr[smallIndex] = this->arr[index];
			this->arr[index] = temp;
		}
	}
	// swap pivot into its final spot
	temp = this->arr[first];
	this->arr[first] = this->arr[smallIndex];
	this->arr[smallIndex] = temp;

	return smallIndex;
}


template <typename T>
class Heap : public BaseSort<T> {
public:
	Heap(const unsigned int capacity) : BaseSort<T>("Heap", capacity) {};
	void runSort();
private:
	void heapify(unsigned int lastUnsortedIndex, unsigned int parent);
};

template <typename T>
void Heap<T>::heapify(unsigned int lastUnsortedIndex, unsigned int parent) {
	// Find largest among root, left child and right child
	unsigned int largest = parent;
	unsigned int left = 2 * parent + 1;
	unsigned int right = 2 * parent + 2;

	if (left < lastUnsortedIndex && this->arr[left] > this->arr[largest])
		largest = left;

	if (right < lastUnsortedIndex && this->arr[right] > this->arr[largest])
		largest = right;

	// Swap and continue heapifying if root is not largest
	if (largest != parent) {
		T temp = this->arr[parent];
		this->arr[parent] = this->arr[largest];
		this->arr[largest] = temp;
		heapify(lastUnsortedIndex, largest);
	}
}

template <typename T>
void Heap<T>::runSort() {

	// Put tree in max heap (all parents bigger than their children)
	unsigned int  i = this->capacity / 2;
	while (i > 0) {
		i--;
		heapify(this->capacity, i);
	}

	// Swap root to sorted position, re-heap, repeat.
	unsigned int lastUnsortedIndex = this->capacity;
	while (lastUnsortedIndex > 0) {
		lastUnsortedIndex--;
		T temp = this->arr[0];
		this->arr[0] = this->arr[lastUnsortedIndex];
		this->arr[lastUnsortedIndex] = temp;
		heapify(lastUnsortedIndex, 0);
	}
}


template <typename T>
void runMySort(BaseSort<T>&& sortObj) {
	cout << "Running sort: " << sortObj.getName() << endl;
	sortObj.loadRandomValues();
	sortObj.printValues();
	auto t1 = std::chrono::high_resolution_clock::now();
	sortObj.runSort();
	auto t2 = std::chrono::high_resolution_clock::now();

	sortObj.printValues();
	sortObj.verifySort();
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
	std::cout << "Sort completed in " << fp_ms.count() << " milliseconds" << endl;

}


int main() {

	//BaseSort<unsigned int> badIdea("cant", 10);

	runMySort(Bubble<unsigned int>(20000));
	runMySort(Selection<unsigned int>(20000));
	runMySort(Insertion<unsigned int>(20000));
	runMySort(Quick<unsigned int>(20000));
	runMySort(Heap<unsigned int>(20000));
	runMySort(Merge<unsigned int>(20000));

	cin.get();
	return 0;
}