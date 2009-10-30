#*
 * Simple builtin function tests
 *#

# Print "-20\n"
assert (neg 20) == (say neg 20);

# Print "30\n"
assert 30 == (say 10 - neg 20);

# Print "-30\n"
assert (neg 30) == (say neg neg neg 10 - neg 20);

# Print "Hello\n"
put "Hello";
ln;

# Exit with exit code 0
exit 10 + 10 - 5 - 15;

