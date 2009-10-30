#*
 * Examples of how to define variables
 *#

# Defines the variables 
my foo = 1 + 1;
my bar = 4 - 1, baz = 100 + 1, bay;

# bay has been initialized with the default value of 0
say bay;

# Prints out "5\n"
assert 5 == (say foo + bar);

# Pritns out "51101\n"
assert 51 == (put baz - 50);
assert 101 == (say baz);

# Change the value of the variable to 99 and print it out
assert 99 == (baz = 99);
say baz;
