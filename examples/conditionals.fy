#* 
 * Simple conditional tests
 *#

# "0010\n"
assert 0 == (put 1 < 1); 
assert 0 == (put 1 < 0); 
assert 1 == (put 0 < 1); 
assert 0 == (say 0 < 0);

# "0100\n"
assert 0 == (put 1 > 1); 
assert 1 == (put 1 > 0); 
assert 0 == (put 0 > 1); 
assert 0 == (say 0 > 0);

# "1001\n"
assert 1 == (put 1 == 1); 
assert 0 == (put 1 == 0); 
assert 0 == (put 0 == 1);
assert 1 == (say 0 == 0);
 
# "0110\n"
assert 0 == (put 1 != 1);
assert 1 == (put 1 != 0); 
assert 1 == (put 0 != 1); 
assert 0 == (say 0 != 0);

# "1011\n"
assert 1 == (put 1 <= 1); 
assert 0 == (put 1 <= 0); 
assert 1 == (put 0 <= 1); 
assert 1 == (say 0 <= 0);

## "1101\n"
assert 1 == (put 1 >= 1); 
assert 1 == (put 1 >= 0); 
assert 0 == (put 0 >= 1); 
assert 1 == (say 0 >= 0);


