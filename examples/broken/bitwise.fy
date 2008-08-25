#*
 * Examples of how to use bitwise operators
 *#

# Prints "01\n"
assert 0 == (put 1 and 0); 
assert 1 == (say 1 and 1); 

# Prints "01\n"
assert 0 == (put 0 or 0);
assert 1 == (say 0 or 1);

# Prints "01\n"
assert 0 == (put 1 xor 1);
assert 1 == (say 1 xor 0);

# Prints "82\n"
assert 8 == (put 2 :< 2);
assert 2 == (say 8 :> 2);

# A bit more complex, prints "9\n"
assert 9 == (say 1 :< 5 :> 5 or 2 and 5 xor 8);

# Same result, but with parenthesis:
assert 9 == (say ((((1 :< 5) :> 5) or 2) and 5) xor 8);

# Different parenthesis, different result: "1\n"
assert 1 == (say 1 :< 5 :> 5 or 2 and (5 xor 8));

# Prints "-1" 
assert (neg 1) == (say neg not 0);

