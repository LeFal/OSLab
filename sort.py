
arr = [5,10,1,4,3,7,6,9,2,8]

for i in range (len(arr)-1):
	for j in range (len(arr)-1):
		if (arr[j] > arr[j+1]):
			temp = arr[j]
			arr[j] = arr[j+1]
			arr[j+1] = temp

print("bubble sort : %s" %arr)