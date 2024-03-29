import re
import os
import itertools
my_dict=dict()
my_dict['A']=['A', '@', '4']
my_dict['a']=['a', '@', '4']
my_dict['B']=['B', '8']
my_dict['E']=['E', '3']
my_dict['e']=['e', '3']
my_dict['I']=['I', '1']
my_dict['i']=['i', '1']
my_dict['L']=['L', '1']
my_dict['l']=['l', '1']
my_dict['O']=['o', '0']
my_dict['o']=['o', '0']
my_dict['S']=['S', '5']
my_dict['s']=['s', '5']
my_dict['T']=['T', '7']
my_dict['t']=['t', '7']

with open('words', 'r') as inputFile:
    with open('outputwords', 'a+') as outputFile:
        words = inputFile.readlines()
        for word in words:
            keys = re.findall(r'\{([^}]+)\}', word)
            for values in itertools.product(*[my_dict[key] for key in keys]):
                output = word.format(**dict(zip(keys, values)))
                outputFile.write(output)
