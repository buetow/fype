#*
 * Examples of how to use scopeing
 *#

my foo = 1;

{
	# Prints out 1
	assert 1 == (put defined foo);

	{
		my bar = 2;

		# Prints out 1
		assert 1 == (put defined bar);
	}

	# Prints out 0
	assert 0 == (put defined bar);

	my baz = 3;
}

# Prints out 0
assert 0 == (say defined bar);


