Radix 64 Encoding in C
======================
Radix 64 or base 64 is a binary-to-text encoding system that is designed to allow binary data to be represented in ASCII string format. This is an educational project accepts a string from a user and Radix 64 encodes it.

What is Radix 64 Encoding?
--------------------------
Radix 64 encoding allows binary data stored in octets (i.e. bytes) to be expressed as printable characters.

Radix-64 characters require the binary input to be split into blocks of 6. These numbers (which all range from 0 - 63) are then mapped onto a character set of printable characters.

The Radix 64 characterset includes:
* A-Z
* a-z
* 0-9
* The additional characters '+' and '/'

This amounts to a total of 64 symbols. To encode binary data into Radix 64, data is parsed in 6-bit blocks (i.e. such that each block has a maximum value of 64), and the number represented by each 6-bit block is used to look up a Radix 64 character.

Number of Output Bytes
----------------------
Radix 64 encoding results in 33% more bytes - every 3 input bytes is converted to 4 output.

Given `n` input bytes, the output will be:

`4⌈n/3⌉`

The ceiling brackets `⌈x⌉` mean round the number to the upper integer. In words,, this expression would be: "n divided by 3, rounded up to the next whole number and then multiplied by 4".

Example: PGP Messages
---------------------
A good example is the [ascii-armor][3] output from PGP:

~~~
jA0ECgMCl9ViayBOZkZg0kkBDqU+4ofR+bJDXd+cpfAQCk30pFcK4QmtFXYivhqy
N8WrBUN8ala9bJ8ON2+COaB1ls+Pr9ohpiWSQLlC6t6/fQLSsHFLCJq5
=GH0r
~~~

The above example is the ciphertext (excluding cleartext message headers) that results from ASCII armored symmetric encryption of the string "Hello World!" (additional metadata is added by PGP - including a salt and the cipher algorithm designator).

Note that the encrypted message consists of Radix 64 characters exclusively, with the '=' sign used as padding - so in the message above, data after the '=' is not part of the message. You wouldn't see this padding if the number of the plaintext bytes was an exact multiple of three (and therefore did not require padding).

By definition, each byte of the original ciphertext was a number from 0-255. Some of these numbers do not map to printable ASCII characters. Encoding the message as Radix 64 allows the message to be printed and/or sent as an email or text message - modes of communication which do not allow the transfer of binary data.

The Radix 64 Alphabet
---------------------
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
                +-----------+-----------+-----------+-----------+-----------+
char:           | 'd'       | 'a'       | 'v'       | 'i'       | 'd'       |
                +-----------+-----------+-----------+-----------+-----------+
binary:         | 011001 00 | 0110 0001 | 01 110110 | 011010 01 | 0110 0100 |
                |                                                           |
6 bit groups:   | 011001 | 000110 | 000101 | 110110 | 011010 | 010110 | 0100  | 00  |
Decimal output: | 25     | 6      | 5      | 54     | 26     | 22     | 16    |     |
                +--------+--------+--------+--------+--------+--------+-------+-----+
Radix-64 chars  | Z      | G      | F      | 2      | a      | W      | Q     | =   |
                +--------+--------+--------+--------+--------+--------+-------+-----+
~~~

Radix 64 Encoding in C
----------------------
The encoding takes place in the function `void base64Encode(unsigned char *inputBuffer, unsigned char *b64Buffer, size_t inputLength)`, in `base64.c`.

The function receives:
* The input buffer `inputBuffer` - which is a pointer to a character array (a c-style string) that contains the user input
* The output buffer `b64Buffer` - a pointer to a dynamically allocated section of memory, sufficient to contain the Radix 64 encoded output

For the first character, the function:
* Takes the first 6 bits, looks up the value against the Radix 64 encoding and assigns the correct ASCII character to `b64Buffer[0]`
* A temporary byte is then created wich stores the data from the unused bits (i.e. the least significant 2 bits from the first char is stored as the most significant 2 bits in `tmpByte`)
* A `size_t divider` variable is set - this will determine the number of bits to take from the next char - these will be combined with the bits in `tmpByte` to form the next Radix 64 6-bit group


### Bitwise Operations
Bitwise operators allow for bit-by-bit manipulation of data stored in individual bytes.

In our case, we make use of bit-masking to collect bits from specific positions within a byte:

~~~c
// base64.c
// Handling the first char
// ----------------------
// tmpByte will carry unused bits forward
unsigned char tmpByte = 0;

// We need last 2 bits, so mask against 00000011
tmpByte = inputBuffer[0] & (0xFF >> 6);

// Mask is 11111100 - note usefulness of the complement operator!
unsigned char mask = ~(0xFF >> 6);

// After getting the first 6 bits, shift them
unsigned char lookupVal = (inputBuffer[0] & mask) >> 2;

// Get the b64 character
b64Buffer[0] = encodingTable[lookupVal];

// First run: set the divider for next char
size_t divider = 4;
~~~

Originally I gathered the first 6 bits like this:
~~~c
size_t lookupVal = (inputBuffer[0] >>= 2) & (0xFF >> 2);
~~~

...which bit shifts the original char (i.e. `inputBuffer[0]`) two places to the right before masking against `00111111`. This does collect the correct data for the Radix 64 lookup index - but it has the unwanted side-effect of mutating the original char - which meant that the function signature could not declare `inputBuffer` as `const`.

Next, the function loops through the characters. During each iteration:

* The correct bits are used to determine the next radix 64 character
* Excess bits are stored in `tmpByte` to be carried forward if necessary
* The `divider` is set such that the correct number of bits are collected when advancing

~~~c
// Loop to the penultimate character (assuming last char is a newline)
// The last 2 bits of tmpByte is the last 2 bits of the first char
// -------------------------------------------------------------------------
size_t i = 1;
size_t bufIndex = 1;
while (i < inputLength) {
    // Shift bits from previous iteration to the correct position
    tmpByte <<= divider;

    // Mask representing the first `divider` bits of a byte
    unsigned char mask = ~(0xFF >> divider);

    // Collect the first `divider` bits of the current char
    unsigned char mostSigBits = inputBuffer[i] & mask;

    // Shift to correct position
    mostSigBits >>= (8 - divider);

    // Add combined bytes to the return buffer
    b64Buffer[bufIndex] = encodingTable[tmpByte ^ mostSigBits];
    bufIndex++;

    // If the divider == 2, the rest of this byte shoudl form the next radix 64 char.
    // Set the last 6 bits of this char as a new radix 64 character
    // ---------------------------------------------------------------------
    if (divider == 2) {
        b64Buffer[bufIndex] = encodingTable[(inputBuffer[i] & (0xFF >> divider))];
        bufIndex++;
        // In this case, there is no data to carry forward
        tmpByte = 0;
    } else {
        // Carry forward the unused bits from the current character
        // Apply a logical & against a mask of the last bits
        tmpByte = inputBuffer[i] & (0xFF >> divider);
    }

    // Set divider for the next char
    // ---------------------------------------------------------------------
    divider = (divider - 2) ? divider - 2 : 6;
    i++;
}
// Add the remaining data from the tmpByte
b64Buffer[bufIndex] = encodingTable[tmpByte <<= divider];
~~~

The divider is set `divider = (divider - 2) ? divider - 2 : 6;`:
* If the current divider is 2, set divider to 6 (because no data is carried forward from the current character)
* If the divider is not 2, set the divider to divider - 2:
  - Current divider is 4, 4 bits carried forward, new divider is 2

Notes on Decoding
-----------------
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
* [Very good video explanation][3] on Base 64: mobilefish

[1]: https://en.wikipedia.org/wiki/Base64
[2]: https://tools.ietf.org/html/rfc1421
[3]: https://en.wikipedia.org/wiki/Binary-to-text_encoding#ASCII_armor
[4]: https://www.youtube.com/watch?v=vw7BhqsjYFk
