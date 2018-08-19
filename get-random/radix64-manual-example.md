david is:
01100100 01100001 01110110 01101001 01100100

Split into groups of 6.

Size = sizeof(char) * 5 = 40
40/6 = 6 remainder 4 therefore 7 sextuplets are needed - the last sextuplet will have 2 bits of padding:

011001 000110 000101 110110 011010 010110 0100   | 00
25     6      5      54     26     22     16     | padding
Z      G      F      2      a      W      Q      | =

* Determine the padding for the last char
* Char 1:
    Use a bitmask of 3 to determine status of last 2 bits

Add zeros to least significant end until the total number of bits is a multiple of 6.
If zeroes are added, last encoded char is '='
Mask against 111111 using bitwise and (&)
Shift right >>= 6 each iteration
Note that we're getting encoded chars in reverse order.

ZGF2aWQ=

01100001
To get last 4 bits:
01100001 &
00001111
--------
00000001

97 & 15 = 1


---


david is:
0110010001100001011101100110100101100100

Split into groups of 6.

Size = 8 * 5 = 40
40/6 = 6 r 4 therefore 7 sextuplets are needed.

011001 000110 000101 110110 011010 010110 0100   | 00

25     6      5      54     26     22     16     | padding

Z      G      F      2      a      W      Q      | =


Add zeros to least significant end until the total number of bits is a multiple of 6.

If zeroes are added, last encoded char is '='

Mask against 111111 using bitwise and (&)

Shift right >>= 6 each iteration

Note that we're getting encoded chars in reverse order.




ZGF2aWQ=
