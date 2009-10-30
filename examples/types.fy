#*
 * Examples how to convert types
 *#

assert 1 == say 1; # Integer output

assert 1 == say double 1; # Double output

assert 14 == say 1 + string 13; # Implicit type conversion to Integer

assert 2 == say integer 2.8; # Rounds down to the Integer 2

assert say integer double string put say neg 12; # Nonsense but working :)

