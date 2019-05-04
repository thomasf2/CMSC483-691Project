#! /usr/bin/python3

from Crypto.Util import number
import pandas as pd
import os
import sys
import subprocess
import time
import argparse

def log(msg):
    print(msg)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-l", "--length", default=64, type=int,
                        help="Specifies the number of bits for the two primes. The final key will be 2 * bits long.")
    parser.add_argument("-p", "--program", default="tdiv", type=str,
                        help="The program to execute the tests on.")
    parser.add_argument("-i", "--iterations", default=5, type=int,
                        help="How many times to run the program.")
    parser.add_argument("-t", "--threads", default=0, type=int,
                        help="How many threads the program under test is gonna potentially use.")
    args = parser.parse_args()

    filename = f'{args.program}.csv'
    
    data = None
    columns=['Bits','Passed','Time(ms)','Time(sec)','Time(mins)','n','p','q']
    
    if os.path.exists(filename):
        data = pd.read_csv(filename)
    else:
        #data = pd.DataFrame([],columns=columns)
        data = pd.DataFrame()


    for i in range(args.iterations):
        log(f'================== Iteration {i + 1} ==================\n')
        
        p = number.getPrime(args.length)
        q = number.getPrime(args.length)
        n = p * q

        msg = (
        f'Calling {args.program} with:\n'
        f'n: {n}\n'
        f'p: {p}\n'
        f'q: {q}\n'
        )
        log(msg)

        cmd = [f'./{args.program}', str(n)]
        if args.threads> 0:
            cmd[2] = str(args.threads)
            
        lines = subprocess.check_output(cmd)\
            .decode()\
            .splitlines()


        output_dict = {
            'n'     : int(lines[0].split(' ')[-1].strip()),
            'p'     : int(lines[1].split(' ')[-1].strip()),
            'q'     : int(lines[2].split(' ')[-1].strip()),
            'ms'    : int(lines[3].split(' ')[-2].strip()),
            'sec'   : int(lines[4].split(' ')[-2].strip()),
            'min'   : int(lines[5].split(' ')[-2].strip()),
        }

        print(output_dict)
        print("n", n, "p", p, "q", q)
        passed =      n == output_dict['n'] \
                 and (p == output_dict['p'] or p == output_dict['q'])\
                 and (q == output_dict['p'] or q == output_dict['q'])


        d = {"Bits": args.length * 2, "Passed": passed, "Time(ms)": output_dict['ms'],\
             "Time(sec)": output_dict['sec'],"Time(mins)": output_dict['min'], \
             "n":output_dict["n"],  "p": output_dict["p"], "q":output_dict["q"]}
        d = pd.DataFrame(d, index =[0])

        data = data.append(d)
        print(data)

    data.to_csv(filename, mode = "w", index=False)
