#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int start, int end)
{
    int n = end - start + 1; // Calculate the size of the array

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        int parent = i;
        while (parent < n / 2) {
            int leftChild = 2 * parent + 1;
            int rightChild = 2 * parent + 2;
            int maxChild = leftChild;

            // Determine the maximum child
            if (rightChild < n && arr[start + rightChild] > arr[start + leftChild]) {
                maxChild = rightChild;
            }

            // Swap if the parent is smaller than the maximum child
            if (arr[start + parent] < arr[start + maxChild]) {
                int temp = arr[start + parent];
                arr[start + parent] = arr[start + maxChild];
                arr[start + maxChild] = temp;

                // Update parent index to continue heapifying
                parent = maxChild;
            } else {
                break; // Heap property is satisfied
            }
        }
    }

    // Extract elements from the heap one by one
    for (int i = n - 1; i > 0; i--) {
        // Swap the root (maximum element) with the last element
        int temp = arr[start];
        arr[start] = arr[start + i];
        arr[start + i] = temp;

        // Restore heap property for the remaining heap
        int parent = 0;
        while (parent < i / 2) {
            int leftChild = 2 * parent + 1;
            int rightChild = 2 * parent + 2;
            int maxChild = leftChild;

            // Determine the maximum child
            if (rightChild < i && arr[start + rightChild] > arr[start + leftChild]) {
                maxChild = rightChild;
            }

            // Swap if the parent is smaller than the maximum child
            if (arr[start + parent] < arr[start + maxChild]) {
                int temp = arr[start + parent];
                arr[start + parent] = arr[start + maxChild];
                arr[start + maxChild] = temp;

                // Update parent index to continue heapifying
                parent = maxChild;
            } else {
                break; // Heap property is satisfied
            }
        }
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) {
        int mid = l + (r - l) / 2;

        mergeSort(pData, l, mid);
        mergeSort(pData, mid + 1, r);

        int leftSize = mid - l + 1;
        int rightSize = r - mid;

        // Allocate memory for temporary left and right arrays
        int *leftArray = (int *)Alloc(sizeof(int) * leftSize);
        int *rightArray = (int *)Alloc(sizeof(int) * rightSize);

        // Copy data to temporary arrays
        for (int i = 0; i < leftSize; i++)
            leftArray[i] = pData[l + i];
        for (int j = 0; j < rightSize; j++)
            rightArray[j] = pData[mid + 1 + j];

        // Merge the temporary arrays back into pData[l..r]
        int i = 0, j = 0, k = l;
        while (i < leftSize && j < rightSize) {
            if (leftArray[i] <= rightArray[j]) {
                pData[k] = leftArray[i];
                i++;
            } else {
                pData[k] = rightArray[j];
                j++;
            }
            k++;
        }

        // Copy the remaining elements of leftArray, if any
        while (i < leftSize) {
            pData[k] = leftArray[i];
            i++;
            k++;
        }

        // Copy the remaining elements of rightArray, if any
        while (j < rightSize) {
            pData[k] = rightArray[j];
            j++;
            k++;
        }

        // Deallocate temporary arrays
        DeAlloc(leftArray);
        DeAlloc(rightArray);
    }
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	 for (int i = 1; i < n; i++) {
        int key = pData[i];
        int j = i - 1;
        while (j >= 0 && pData[j] > key) {
            pData[j + 1] = pData[j];
            j = j - 1;
        }
        pData[j + 1] = key;
    }
	
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	 int swapped;
    do {
        swapped = 0;
        for (int i = 0; i < n - 1; i++) {
            if (pData[i] > pData[i + 1]) {
                // Swap pData[i] and pData[i + 1]
                int temp = pData[i];
                pData[i] = pData[i + 1];
                pData[i + 1] = temp;
                swapped = 1;
            }
        }
    } while (swapped);
	
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (pData[j] < pData[min_idx])
                min_idx = j;
        }
        // Swap pData[i] and pData[min_idx]
        int temp = pData[i];
        pData[i] = pData[min_idx];
        pData[min_idx] = temp;
    }
	
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		 for (int i = 0; i < dataSz; ++i) {
            fscanf(inFile, "%d", &((*ppData)[i]));
        }
        fclose(inFile);
    }
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
