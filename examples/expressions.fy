#* 
 * Simple expression tests
 *#

# Result 10 
assert 10 == say (8 / 2) + 2 * 3; 

# Result 12 
assert 12 == say 2 * (4 + 2);  

# Result 4 
assert 4 == say 2 * (4 / 2);  

# Result 4 
assert 4 == say 2 * (4 / 2);  

# Result 4 
assert 4 == say 2 * (4 / 2);  

# Result 46 
assert 46 == say "12" + "34";

# Result 1231
assert 1231 == say "1234" - "3";

# Result 24
assert "24" == say "2ab" * "12";

# Result 5.0
assert 5 == say "10 bla" / 2;

