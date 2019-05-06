#! /usr/bin/python3
'''Encrypts and decrypts ascii strings

This is only meant for testing purposes of small, unsecure
primes for educational purposes and therefore should not
be used by anyone.

Usage:
    This is meant to be run on the commandline, it will
    work by itself with default values

    >>> ./encrypt.py

    For more information on what attributes can be changed,
    run the program with the -h or --help options

    >>> ./encrypt.py --help

    If the -m or --message option is used, make sure the
    string is in quotes

    >>> ./encrypt.py --message "This is not my sandwhich."

    An equal sign can also be used for the options

    >>> ./encrypt.py --message="This is not my sandwhich."
'''

import argparse
from Crypto.Util.number import getPrime, GCD, size, inverse

# Helper functions defined here
listToStr = lambda lst: ''.join(item for item in lst)
listToStr.__doc__ = """Combinds all string elements of a list to a single string"""

strToList = lambda string, size: [string[i:i+size] for i in range(0, len(string), size)]
strToList.__doc__ = """Breaks up a string into a list of substrings of size size"""

def encrypt(msg, e, n):
    '''Encrypts a string msg with values e and n in 6 steps

    Steps:
    1) Breaks the sting into chunks that can be encrypted for given n.
    2) Each chunk is translated into its ascii value as a hex string.
    3) The last chunk is padded with null values if it is smaller
       than the other chunks.
    4) Each chunk is encrypted and stored as a binary value.
    5) Each chunk is given leading 0's so each has bits equal
       to the number of bits in n.
    6) The chunks are joined together and converted to a number.
    '''
    chunkSize = int(size(n) / 8)

    # Step 1: Break message up into chunks
    msgChunks = strToList(msg, chunkSize)
    # Steps 2 to 5: Read docstring
    for func in [(lambda chunk: ''.join(hex(ord(c))[2:] for c in chunk)),
                 (lambda chunk: chunk + '0'*((chunkSize*2)-len(chunk))),
                 (lambda chunk: bin(pow(int(chunk, 16), e, n))[2:]),
                 (lambda chunk: chunk.zfill(size(n)))]:
        msgChunks = list(map(func, msgChunks))
    # Step 6: Merge chunks into a single string and convert to integer
    return int(listToStr(msgChunks), 2)

def decrypt(msg, d, n):
    '''Decrypts a number num using values d and n
    
    Steps:
    1) Converts the number to a binary sting and pads the
    front with 0's so it can be evenly divided into chunks
    of bits equal to the number of bits in n
    2) Divides it into said chunks
    3) Converts each chunk to an int, decrypts it, then
    stores its hex value
    4) Converts each chunk to its appropriate ascii value
    5) Merges all chunks and strips null padding if it
    was used during encryption
    '''
    chunkSize = size(n)

    # Step 1
    binMsg = ('0'*(chunkSize - (size(msg) % chunkSize))) + bin(msg)[2:]
    # Step 2
    encryptedChunks = strToList(binMsg, chunkSize)
    # Steps 3 to 4
    for func in [(lambda chunk: hex(pow(int(chunk, 2), d, n))[2:]),
                 (lambda chunk: "".join(chr(int(chunk[i:i+2], 16)) for i in range(0, len(chunk), 2)))]:
        encryptedChunks = list(map(func, encryptedChunks))
    # Step 5
    return listToStr(encryptedChunks).rstrip(chr(0))

if __name__ == "__main__":
    minBits = 15 # n can't be less than 32768
    maxBits = 128 # n can't be greater than 680564733841876926926749214863536422911
    
    # Argument parser stuff
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", "--bits", type=int, default=minBits,
                        help=f"The number of bits in n. Range {minBits}..{maxBits}")
    parser.add_argument("-m", "--message", type=str,
                        default="You will never know my secret!",
                        help="The message to be encrypted")
    args = parser.parse_args()
    if not args.bits in range(minBits, maxBits+1):
        parser.error(f"argument -b/--bits: must be in range {minBits} to {maxBits}")
    if not args.message:
        parser.error("argument -m/--message: cannot be empty")

    # Encryption Stuff
    E = [3, 5, 17, 257, 65537]
    i = 0
    while True:
        p = getPrime(int(args.bits / 2))
        q = getPrime(int(args.bits / 2) + 1)
        n = p * q
        if len(bin(n)[2:]) == args.bits:
            while GCD(E[i], n) != 1:
                i += 1
            e = E[i]
            d = inverse(e, (p-1) * (q-1))
            if d != 1:
                break

    # Prints the encrypted message
#    print(args.message)
    encrypted = encrypt(args.message, e, n)
    print(encrypted)
#    decrypted = decrypt(encrypted, d, n)
#    print(decrypted)
