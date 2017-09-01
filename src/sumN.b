declblock{
	int data[100];
	int i, sum;
}

codeblock{
	for i = 1, 100 {
		data[i] = i; 
		if i > 2 {
			read r;
		}
		else {
			x=2;
		}
	}
	

	sum = 0;
	for i = 1, 100 {
		sum = sum + data[i];
 	}

	print "Sum value: ", sum;
}

