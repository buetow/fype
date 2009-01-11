#*
 * Examples of how to use arrays
 *#

# Create a function bar, returns 0 by default
func bar { say "bar" } 

# Create a multi dimensional array foo
my foo = [bar, 1, 4/2, double "3", ["A", ["BA", "BB"]]];

# Run on each element of foo recursive 'say'
say foo;

# Print the length of the array
assert 5 == say len foo;

# Print the last index of the array
assert 4 == say ind foo;

# Print the 3rd element of the array
say foo[3];

#say "bla";
