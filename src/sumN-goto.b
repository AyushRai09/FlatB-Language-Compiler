declblock{
	int i, sum;
}

codeblock{
	i = 0;

L1:	i = i + 1;
	goto L1 if i < 100;
	sum=sum+i;

}
