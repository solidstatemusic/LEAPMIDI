int main()
{
	char buffer[1024];
	printf("Buffer size: %d\n", sizeof(buffer));

	foo(buffer);

	return 0;
}

void foo(char *buffer)
{
	printf("Buffer size in foo: %d\n", sizeof(*buffer));
}
