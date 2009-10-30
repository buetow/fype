#*
 * Examples of how to use procedures
 *#

proc foo {
	say 1 + a * 3 + b;
	my c = 6;
}

my a = 2, b = 4;

foo; # Run the procedure. Print out "11\n"
say c; # Print out "6\n";

proc bar {
	say "I am bar";

	undef baz;

	proc baz {
		say "I am baz";
	}
}

# Here bar would produce an error because the proc is not yet defined!
# bar; 

bar; # Here the procedure bar will define the procedure baz!
baz; # Now the procedure baz is defined!
bar; # Here the procedure bar will redefine baz again!
