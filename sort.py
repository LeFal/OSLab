
arr = [5,10,1,4,3,7,6,9,2,8]


# bubble sort
for i in range (len(arr)-1):
	for j in range (len(arr)-1):
		if (arr[j] > arr[j+1]):
			temp = arr[j]
			arr[j] = arr[j+1]
			arr[j+1] = temp

print("bubble sort : %s" %arr)

# insertion sort
for i in range(1, len(arr)):
	j = i -1
	key = arr[i]
	while (arr[j] > arr[i] and j >= 0 ):
		arr[j+1] = arr[j]
		j = j-1
	arr[j+1] = key

print("insertion sort : %s" %arr)

# selection sort
for i in range(len(arr)):
	j = i
	min_index = i
	for j in range(i+1, len(arr)):
		if (arr[j] < arr[min_index]):
			min_index = j
	if (min_index != i):
		temp = arr[min_index]
		arr[min_index] = arr[i]
		arr[i] = temp

print("selection sort : %s" %arr)



# merge sort
def merge_sort(array, left, right):
	if (left != right):
		mid = (right + left)/2
		merge_sort(array[:mid], 0, mid)
		merge_sort(array[mid+1:])








