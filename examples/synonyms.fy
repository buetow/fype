#*
 * Examples of how to use synonyms
 *#

# Create a variable foo, and bar is a synonym for foo
my foo = "foo";
my bar = \foo;

# Reset the value of foo
foo = "bar";

# The synonym variable should now also set to "bar"
assert "bar" == say bar;

# Create a new procedure baz
proc baz { 
	say "I am baz";
}

# Make a synonym baz, and undefine baz
my bay = \baz;
undef baz;

# bay still has a reference of the original procedure baz
bay; # this prints aut "I am baz" 

assert 0 == defined baz;
assert 1 == defined bay;

# This removes the procedure from memory
undef bay; 

