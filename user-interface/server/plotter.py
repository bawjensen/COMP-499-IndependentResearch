import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

def read_csv_for_plot(filename):
    averages = []
    parameters = {}
    
    with open(filename, 'r') as inFile:
        firstLine = inFile.readline().strip().split(',')
        secondLine = inFile.readline().strip().split(',')
        
        for i, val in enumerate(firstLine):
            parameters[val] = secondLine[i]
            
        
        trash = inFile.readline()
        
        for i in xrange(int(parameters['numGenerations'])):
            ithLine = inFile.readline()
            
            averages.append(ithLine.split(',')[1])
            
#         trash = inFile.readline()
#         trash = inFile.readline()
            
    return (averages, parameters)

def plotAllCsvs():
    linewidth = 0.75
    
    pngName = sys.argv[1]
    csvDirs = sys.argv[2:]

    for csvDir in csvDirs:
        csvPath = '../../runs/saved-runs/' + csvDir + '/output.csv'
        print 'Using:', csvPath

        try:
            df = read_csv_for_plot(csvPath)
            plt.plot(df[0], label=csvDir, lw=linewidth)
            plt.legend(loc=4)
            
        except:
            print 'Issue with file at:', csvPath, '- ignoring'

    print 'Saving at:', pngName
    plt.savefig(pngName)
        
plotAllCsvs()
