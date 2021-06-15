#pragma once

#include <vector>
#include <cassert>

template<class _type, class _container = std::vector<_type>, class _pred = std::less<_type>>
class priority_queue {
private:
	_container heap{};
	_pred comparator{};

	int leftChild(int index) const noexcept {
		return 2 * index + 1;
	}

	int rightChild(int index) const noexcept {
		return 2 * (index + 1);		// Same As 2 * index + 2 or leftChild(index) + 1
	}

	int parent(int index) const noexcept {
		return (index - 1) / 2;
	}
public:
	bool empty() const noexcept {
		return heap.empty();
	}

	int size() const noexcept {
		return heap.size();
	}

	// The noexcept check kare compile - time if an expression does not throw the exception
	const _type& top() const noexcept {		// Calling front On Empty Container Is Undefined Behaviour
		return heap.front();
	}

	void push(const _type& val) {
		heap.push_back(val);
		heapifyUp(heap.size() - 1);
	}

	void push(_type&& val) {
		heap.push_back(std::move(val));
		heapifyUp(heap.size() - 1);
	}

	void pop() {
		if (!heap.empty()) {
			std::swap(heap[0], heap[heap.size() - 1]);
			heap.pop_back();
			heapifyDown(0);
		}
	}

	void heapifyDown(int index) {
		int largeElemInd = index;
		int leftChildInd = leftChild(index), rightChildInd = rightChild(index);

		if (leftChildInd < heap.size()) {
			if (comparator(heap[leftChildInd], heap[largeElemInd])) {
				largeElemInd = leftChildInd;
			}
		}
		if (rightChildInd < heap.size()) {
			if (comparator(heap[rightChildInd], heap[largeElemInd])) {
				largeElemInd = rightChildInd;
			}
		}

		if (index != largeElemInd) {
			std::swap(heap[largeElemInd], heap[index]);
			heapifyDown(largeElemInd);
		}
	}

	void heapifyUp(int index) {
		int parent_ind = parent(index);
		if (index <= 0) {
			return;
		}

		if (comparator(heap[index], heap[parent_ind])) {
			std::swap(heap[index], heap[parent_ind]);
			heapifyUp(parent_ind);
		}
	}
};