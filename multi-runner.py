import subprocess
import sys
from multiprocessing import Pool

def main():
    numSubProcesses = len(sys.argv) - 1 # Subtract the exe

    pool = Pool(processes=numSubProcesses)

    for i in xrange(numSubProcesses):
        # print 'Starting', sys.argv[i+1], 'async'
        pool.apply_async(subprocess.call, [['bin/2048f', '-t', sys.argv[i+1]]])

    pool.close()
    pool.join()

    print 'Done'


if __name__ == '__main__':
    main()