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
import sys
from Crypto.Util.number import getPrime, GCD, size, inverse

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
    decryptChunkSize = len(bin(n)[2:])
    chunkSize = int(decryptChunkSize / 8)

    # Unreadable code, don't even try nerds
    ## Read steps 2 to 5 in the docstring for English explanations
    trans = [(lambda chunk: ''.join(hex(ord(c))[2:] for c in chunk)),
             (lambda chunk: chunk + '0'*((chunkSize*2)-len(chunk))),
             (lambda chunk: bin(pow(int(chunk, 16), e, n))[2:]),
             (lambda chunk: ('0'*(decryptChunkSize-len(chunk))) + chunk)]

    # Step 1: Break message up into chunks
    msgChunks = [msg[i:i+chunkSize] for i in range(0, len(msg), chunkSize)]
    # Step 2: ???????
    for func in trans:
        msgChunks = list(map(func, msgChunks))
    # Step 6: Merge chunks into a single string and convert to integer
    return int(''.join(chunk for chunk in msgChunks), 2)

def decrypt(num, d, n):
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
    binN = bin(n)[2:]
    binNum = bin(num)[2:]
    decryptChunkSize = len(binN)

    # Unreadable code, don't even try nerds
    ## Read steps 3 and 4 in the docstring for English explanations
    trans = [(lambda chunk: hex(pow(int(chunk, 2), d, n))[2:]),
             (lambda chunk: "".join(chr(int(chunk[i:i+2], 16)) for i in range(0, len(chunk), 2)))]

    # Step 1: Pad the encrypted message with 0's
    binNum = ('0'*(decryptChunkSize - (len(binNum) % decryptChunkSize))) + binNum
    # Step 2: Break the encrypted message up into chunks
    encryptedChunks = [binNum[i:i+decryptChunkSize] for i in range(0, len(binNum), decryptChunkSize)]
    # Step 3: ???????
    for func in trans:
        encryptedChunks = list(map(func, encryptedChunks))
    # Step 5: Merge decrpted chunks into string and remove null padding
    return ''.join(chunk for chunk in encryptedChunks).rstrip(chr(0))

if __name__ == "__main__":
    # Globals
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

    print(args.message)
    encrypted = encrypt(args.message, e, n)
    print(encrypted)
    decrypted = decrypt(encrypted, d, n)
    print(decrypted)
