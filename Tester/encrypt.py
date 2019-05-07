#! /usr/bin/python3
'''Encrypts and decrypts a string using a given program

This is only meant for testing purposes of small, unsecure
primes for educational purposes and therefore should not
be used by anyone. The program generates all RSA values,
then lets the specified program factor the RSA composite,
then compares the values returned to the originals and
the original message to what was decrypted using the
returned values.

It is worth noting that a decrypted message may appear
exactly like the original on the terminal, but will fail
a boolean comparison on occation. This was taken into
consideration for about 2 seconds then dismissed as
unimportant as it does not affect our project or
demonstation.

Usage:
    This is meant to be run on the commandline, it will
    work by itself with default values, but the program
    must be specified

    $ ./encrypt.py -p tdiv

    For more information on what attributes can be changed,
    run the program with the -h or --help options

    $ ./encrypt.py --help

    If the -m or --message option is used, make sure the
    string is in quotes

    $ ./encrypt.py --program=tdiv --message "This is not my sandwich."

    An equal sign can also be used for the options

    $ ./encrypt.py --program=tdiv --message="This is not my sandwich."
'''

import os
import argparse
import subprocess
from Crypto.Util.number import getPrime, GCD, size, inverse

def listToStr(lst):
    '''Combines all elements of a list to a single string

    This does not do type checking on the elements in the
    list, dont do anything stupid.
    
    Parameters:
        lst (list(str)): a list of string elements

    Returns:
        str: The concatination of all the items in lst as a single string
    '''
    return ''.join(item for item in lst)

def strToList(string, size):
    '''Breaks up a string into a list of substrings

    Each substing will be of the size specified,
    If the characters in string are not evenly divisible by
    the value of size, the last element of the list will
    be smaller than the rest.
    
    Parameters:
        string (str): a string
        size (int): the size of each substring

    Returns:
        list(str): list of substrings
    '''
    return [string[i:i+size] for i in range(0, len(string), size)]

def pqend(numBits):
    '''Generates RSA values for p, q, e, n, and d
    
    Parameters:
        numBits (int): number of bits for n

    Returns:
        ints: p, q, e, n, d
    '''
    E = [3, 5, 17, 257, 65537]
    i = 0
    while True:
        p = getPrime(int(numBits / 2))
        q = getPrime(int(numBits / 2) + 1)
        n = p * q
        if size(n) == numBits:
            while GCD(E[i], n) != 1:
                i += 1
            e = E[i]
            d = inverse(e, (p-1) * (q-1))
            if d != 1:
                break

    return p, q, e, n, d

def encrypt(msg, e, n=None, p=None, q=None):
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

    Parameters:
        msg(str): ascii sting containing a message

    Returns:
        int: encrypted message as a number

    Raises:
        ValueError: if n is not given and p or q are not given

    '''
    if n is None:
        if p is None or q is None:
            raise ValueError("p and q are needed if n is not given")
        n = p * q

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

def decrypt(msg, d=None, n=None, p=None, q=None):
    '''Decrypts a number msg using values d and n

    If d is not given, it will be calculated based on
    the values of p and q
    
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

    Parameters:
        msg (int): The encrypted messag
        d (int): For decryption, can be generated
                 from p and q
        n (int): The RSA composite number. Can be
                 generated from p and q
        p (int): One prime factor of the RSA composite n
        q (int): One prime factor of the RSA composite n

    Returns:
        str: The decrypted message

    Raises:
        ValueError: if d is not given and p or q are not given
        ValueError: if n is not given and p or q are not given
    '''
    if n is None:
        if p is None or q is None:
            raise ValueError("p and q are needed if n is not given")
        n = p * q
        
    if d is None:
        if p is None or q is None:
            raise ValueError("p and q are needed if d is not given")
        E = [3, 5, 17, 257, 65537]
        i = 0
        while GCD(E[i], n) != 1:
            i += 1
        d = inverse(E[i], (p-1) * (q-1))

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
    minThreads = 1
    maxThreads = 8 # Jhon's computer can't do more than 8 threads
    
    # Argument parser stuff
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", "--bits", type=int, default=minBits,
                        help=f"The number of bits in n. Range {minBits}..{maxBits}")
    parser.add_argument("-m", "--message", type=str,
                        default="You will never know my secret!",
                        help="The message to be encrypted")
    parser.add_argument("-t", "--threads", type=int, default=1,
                        help="The number of threads the program will use")
    parser.add_argument("-p", "--program", type=str, required=True,
                        help="The program that will factor a number")
    args = parser.parse_args()
    if not os.path.exists(args.program):
        parser.error(f"argument -p/--program: {args.program} not found")
    if not args.bits in range(minBits, maxBits+1):
        parser.error(f"argument -b/--bits: must be in range {minBits} to {maxBits}")
    if not args.message:
        parser.error("argument -m/--message: cannot be empty")
    if not args.threads in range(minThreads, maxThreads+1):
        parser.error(f"argument -t/--threads: must be in range {minThreads} to {maxThreads}")

    # Encryption Stuff
    print(f"Message: {args.message}\n"
          f"Encrypting message with {args.bits} bits")
    p, q, e, n, d = pqend(args.bits)
    encrypted = encrypt(args.message, e, n)
    print(f"\nEncrypted Message:\n{str(encrypted)}\n")
    
    # Calling Process
    print(f"Factoring public key {n} with {args.program} using {args.threads} threads\n")
    cmd = [f'./{args.program}', str(n), str(args.threads)]
    output = subprocess.check_output(cmd)\
        .decode()
    
    # Parsing output
    lines = output\
        .splitlines()
    lines = [line for line in lines if not line.isspace() and line]

    output_dict = {
        'n'      : int(lines[0].split(' ')[-1].strip()),
        'p'      : int(lines[1].split(' ')[-1].strip()),
        'q'      : int(lines[2].split(' ')[-1].strip()),
        'ms'     : int(lines[3].split(' ')[-2].strip()),
        'sec'    : int(lines[4].split(' ')[-2].strip()),
        'min'    : int(lines[5].split(' ')[-2].strip()),
    }

    # Results
    passed = (n == output_dict['n']) \
             and (p == output_dict['p'] or p == output_dict['q'])\
             and (q == output_dict['p'] or q == output_dict['q'])
    if passed:
        print(f"Successfully factored {n} into {p} and {q}")
    else:
        print(f"Failed\n"
              f"Expected p: {p}\n"
              f"Calculated: {output_dict['p']}\n"
              f"Expected q: {q}\n"
              f"Calculated: {output_dict['q']}\n")
    print(f"Process finished in {output_dict['ms']}ms\n\n")

    # Decryption
    print(f"Decrypting using values {output_dict['p']} and {output_dict['q']}\n")
    decrypted = decrypt(encrypted, p=output_dict['p'], q=output_dict['q'])
    print(f"Original : {args.message}\n"
          f"Decrypted: {decrypted}")
