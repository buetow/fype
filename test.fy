#*
 * Examples of how to use arrays
 *#

# Create a function bar, returns 0 by default
func bar { say "bar" } 

# Create a multi dimensional array foo
my foo = [bar, 1, 4/2, double "3", ["A", ["BA", "BB"]]];

# Run on each element of foo recursive 'say'
say foo;
