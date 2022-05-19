#pragma once

class Allocator {
private:

	//Array of all allocations
	void** allocations = nullptr;
	//The number of allocations
	int nAllocations = 0;

public:

	//Adds a allocations to the array of allocations
	void registerAllocation(void* address) {
		//If there has not been an allocation yet
		if (!this->nAllocations) {
			this->nAllocations++;
			//Allocate one void*, to add store the allocation to
			this->allocations = (void**)malloc(this->nAllocations * sizeof(void*));
			//Store the allocation
			this->allocations[0] = address;
		} //There has already been an allocation
		else {
			this->nAllocations++;
			//Increase the size of the array holding the allocations
			this->allocations = (void**)realloc(this->allocations, this->nAllocations * sizeof(void*));
			//Store the new allocation
			this->allocations[this->nAllocations - 1] = address;
		}
	}
	//Frees all allocations and clears the array
	void clearAllAllocations() {
		//Frees every allocation in the array
		for (int i = 0; i < this->nAllocations; i++) {
			free(allocations[i]);
		}
		//Deletes the array
		free(this->allocations);
		//Resets the data
		this->allocations = nullptr;
		this->nAllocations = 0;
	}

}GlobalAllocator; //Global allocater, cleared after every code execution
