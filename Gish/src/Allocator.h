#pragma once

class Allocator {
private:

	void** allocations = nullptr;
	int nAllocations = 0;

public:

	void registerAllocation(void* address) {

		if (this->nAllocations == 0) {
			this->nAllocations = 1;
			this->allocations = (void**)malloc(this->nAllocations * sizeof(void*));
			this->allocations[0] = address;
		}
		else {
			this->nAllocations++;
			void** buffer = (void**)malloc((this->nAllocations * sizeof(void*)) - (1 * sizeof(void*)));
			for (int i = 0; i < this->nAllocations - 1; i++)
				buffer[i] = this->allocations[i];
			this->allocations = (void**)realloc(this->allocations, this->nAllocations * sizeof(void*));
			for (int i = 0; i < this->nAllocations - 1; i++)
				allocations[i] = buffer[i];
			this->allocations[this->nAllocations - 1] = address;
			free(buffer);
		}
	}

	void clearAllAllocations() {
		for (int i = 0; i < this->nAllocations; i++) {
			free(allocations[i]);
		}
		free(this->allocations);
		this->allocations = nullptr;
		this->nAllocations = 0;
	}

}GloabalAllocator;