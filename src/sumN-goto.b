declblock{
	int i, sum;
}

codeblock{
	i = 0;

L1:	i = i + 1;
	goto L1 if i < 100;


	sum = 0;
	i = 0;
L2:	sum = sum + i;
	i = i + 1;
	goto L2 if i < 100;

	print "Sum value: ", sum;
}
