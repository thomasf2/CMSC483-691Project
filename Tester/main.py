from Crypto.Util import number
import os
import getopt
import sys
import subprocess
import time

def log(msg):
    print(msg)
    f.write(msg)

f = None

def print_usage(scriptname):
    print((
        f'Usage: {scriptname} args\n'
        f'Args can be:\n'
        f'[-h | --help]                 Prints this message\n'
        f'[-l | --length] <bits>        Specifies the numbr of bits for the two primes. The final key will be 2 * bits long.\n'
        f'[-p | --program] <program>    The program to execute the tests on.\n'
        f'[-i | --iterations] <its>     How many times to run the program.\n'
        f'[-t | --threads]              How many threads the program under test is gonna potentially use.\n'    
    )
    )

if __name__ == "__main__":
    length = 64
    program = 'tdiv'
    iterations = 5
    threads = 0

    # Parse command line arguments
    unixOptions = "hl:p:i:t:"
    gnuOptions = ["help", "length=", "program=", "iterations=", "threads="]

    try:  
        arguments, values = getopt.getopt(sys.argv[1:], unixOptions, gnuOptions)
    except getopt.error as err:  
        # output error, and return with an error code
        print (str(err))
        sys.exit(2)

    for arg, val in arguments:
        if arg in ("-h", "--help"):
            print_usage(sys.argv[0])
            sys.exit(0)
        elif arg in ("-l", "--length"):
            length = int(val)
        elif arg in ("-p", "--program"):
            program = val
        elif arg in ("-i", "--iterations"):
            iterations = int(val)
        elif arg in ("-t", "--threads"):
            threads = int(val)


    

    timestamp = time.strftime("%Y%m%d-%H%M%S")
    filename = f'logs/{program}/{length * 2}-bits-{timestamp}.txt'

    os.makedirs(os.path.dirname(filename), exist_ok=True)

    f = open(filename, "w")

    

    for i in range(iterations):
        log(f'================== Iteration {i + 1} ==================\n')
        
        p = number.getPrime(length)
        q = number.getPrime(length)
        n = p * q

        msg = (
        f'Calling {program} with:\n'
        f'n: {n}\n'
        f'p: {p}\n'
        f'q: {q}\n'
        )
        log(msg)

        cmd = [f'./{program}', str(n)]
        if threads> 0:
            cmd[2] = str(threads)
            
        output = subprocess.check_output(cmd).decode()
        log(output)
    
    f.close()