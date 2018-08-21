Radix 64 Encoding in C
======================
Radix 64 or base 64 is a binary-to-text encoding system that is designed to allow binary data to be represented in ASCII string format.

Radix-64 characters require the binary input to be split into blocks of 6. These numbers (which all range from 0 - 63) are then mapped onto a character set of printable characters.

Generally the Radix 64 characterset includes:
* A-Z
* a-z
* 0-9
* The additional characters '+' and '/'

This amounts to a total of 64 symbols. To encode binary data into Radix 64, data is parsed in 6-bit blocks (i.e. such that each block has a maximum value of 64), and the number represented by each 6-bit block is used to look up a Radix 64 character.

A good example is the [ascii-armor][3] output from PGP:

~~~
jA0ECgMCl9ViayBOZkZg0kkBDqU+4ofR+bJDXd+cpfAQCk30pFcK4QmtFXYivhqy
N8WrBUN8ala9bJ8ON2+COaB1ls+Pr9ohpiWSQLlC6t6/fQLSsHFLCJq5
=GH0r
~~~

The above example is the ciphertext (excluding cleartext message headers) that results from ASCII armored symmetric encryption of the string "Hello World!" (additional metadata is added by PGP - including a salt and the cipher algorithm designator).

Note that the encrypted message consists of Radix 64 characters exclusively, with the '=' sign used as padding - so in the message above, data after the '=' is not part of the message. You wouldn't see this padding if the number of the plaintext bytes was an exact multiple of three (and therefore did not require padding).

By definition, each byte of the original ciphertext was a number from 0-255. Some of these numbers do not map to printable ASCII characters. Encoding the message as Radix 64 allows the message to be printed and/or sent as an email or text message - modes of communication which do not allow the transfer of binary data.

Radix 64 Alphabet
-----------------
~~~
Index R64 value  Index R64 value  Index R64 value  Index R64 value
0     A          17    R          34    i          51   z
1     B          18    S          35    j          52   0
2     C          19    T          36    k          53   1
3     D          20    U          37    l          54   2
4     E          21    V          38    m          55   3
5     F          22    W          39    n          56   4
6     G          23    X          40    o          57   5
7     H          24    Y          41    p          58   6
8     I          25    Z          42    q          59   7
9     J          26    a          43    r          60   8
10    K          27    b          44    s          61   9
11    L          28    c          45    t          62   +
12    M          29    d          46    u          63   /
13    N          30    e          47    v
14    O          31    f          48    w         (pad) =
15    P          32    g          49    x
16    Q          33    h          50    y
~~~

Example Radix 64 Encoding
-------------------------
Radix 64 encoding can be difficult to get your head around. Before building a programme to do the encoding, it can help to step through the process manually.

This example will encode the string 'david' into Radix 64.

* The string is represented by 5 chars.
* The total size in bits (assuming that one char == 1 byte) is given by 8 x 5 = 40 bits.
* Dividing the total number of bits by 6 leaves remainder 4 -  therefore 7 sextuplets will be needed to represent the string in Radix 64
* The last sextuplet will have 2 bits of padding. When this is the case, '=' padding character is added.

~~~
char:           | 'd' | 'a' | 'v' | 'i' | 'd' |
binary:         | 011001 00  | 0110 0001  | 01 110110  | 011010 01  | 0110 0100  |
6 bit groups:   | 011001 | 000110 | 000101 | 110110 | 011010 | 010110 | 0100   | 00 |
Decimal output: | 25     | 6      | 5      | 54     | 26     | 22     | 16     | padding |
Radix-64 chars  | Z      | G      | F      | 2      | a      | W      | Q      | = |
~~~

Another way to look at the problem. The string 'david' is represented by these bits:
`0110010001100001011101100110100101100100`

Split into groups of 6:

Size = 8 * 5 = 40
40/6 = 6 remainder 4 therefore 7 sextuplets (and therefore radix 64 characters) are needed.

By convention,

Decoding
--------
To decode radix 64 encoded text, typically four characters are converted to three bytes. If the string contains a single padding character (i.e. '=') the last four characters (including the padding character) will decode to only two bytes, while '==' indicates that the four characters will decode to only a single byte. From [RFC 1521][2]:

>Since all base64 input is an integral number of octets, only the following cases can arise:
>1. the final quantum of encoding input is an integral multiple of 24 bits; here, the final unit of encoded output will be an integral multiple of 4 characters with no '=' padding
>2. the final quantum of encoding input is exactly 8 bits; here, the final unit of encoded output will be two characters followed by two '=' padding characters
>3. the final quantum of encoding input is exactly 16 bits; here, the final unit of encoded output will be three characters followed by one '=' padding character."

Manual example:
~~~
radix 64 chars  | T | Q | = | =  |
TQ== goes to a single byte
radix 64 int:   | 19  | 16  |
binary:         | 010011 | 010000 |
First 8 bits:   | 01001101 | last 4 bits are "padding" zeroes
Decimal:        | 77 |
ASCII:          | M  |
~~~


## Resources
* [Wiki article][1]
* [RFC 1421][2]: Privacy Enhancement for Internet Electronic Mail: Part I: Message Encryption and Authentication Procedures

[1]: https://en.wikipedia.org/wiki/Base64
[2]: https://tools.ietf.org/html/rfc1421
[3]: https://en.wikipedia.org/wiki/Binary-to-text_encoding#ASCII_armor
