import matplotlib
matplotlib.use('Agg')

import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

import sys
import json

# sns.set(rc={"figure.figsize": (6, 6)})
# np.random.seed(sum(map(ord, "palettes")))

def read_data_for_plot(filename, dataKey):
    jsonFile = open(filename, 'r')
    data = json.load(jsonFile)
    jsonFile.close()

    return [gen[dataKey] for gen in data['generations']]

def plotAllData():
    linewidth = 0.75
    
    pngName     = sys.argv[1]
    runSuperDir = sys.argv[2]
    plotMode    = sys.argv[3]
    runDirs     = sys.argv[4:]

    print 'plotMode:', plotMode

    print 'Setting', len(runDirs), 'different colors'

    for csvDir in runDirs:
        csvPath = runSuperDir + csvDir + '/output.json'
        print 'Using:', csvPath

        try:
            print 'Before reading the file'
            df = read_data_for_plot(csvPath, plotMode)
            print 'After reading the file, df:', df
            plt.plot(df, label=csvDir, lw=linewidth)
            plt.legend(loc=4)
            
        except:
            print 'Issue with file at:', csvPath, '- ignoring'

    print 'Saving at:', pngName
    plt.savefig(pngName)
        
plotAllData()
