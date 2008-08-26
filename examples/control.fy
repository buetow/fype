#*
 * Examples of how to use control statements
 *#

if 1 { 
	say "if 1";
} 

ifnot 0 == 1 { 
	say "ifnot 0 == 1";
} 

# Calculate 10!

my n = 10, fac = 0;

while n > 1 {
	ifnot fac {
		fac = 1;
	}
	say fac = (fac * n);
	decr n;
}

# Count up to 10

n = 0;

until n == 10 {
	say incr n;
}

