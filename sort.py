
arr = [5,10,1,4,3,7,6,9,2,8]


# bubble sort
for i in range (len(arr)-1):
	for j in range (len(arr)-1):
		if (arr[j] > arr[j+1]):
			temp = arr[j]
			arr[j] = arr[j+1]
			arr[j+1] = temp

print("bubble sort : %s" %arr)


for i in range(1, len(arr)):
	j = i -1
	key = arr[i]
	while (arr[j] > arr[i] and j >= 0 ):
		arr[j+1] = arr[j]
		j = j-1
	arr[j+1] = key

print("insertion sort : %s" %arr)