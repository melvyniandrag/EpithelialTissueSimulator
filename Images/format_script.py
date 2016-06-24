import os, os.path

# simple version for working with CWD
n = len([name for name in os.listdir('.') if (os.path.isfile(name) and name.endswith('.off'))])
fout = open('script.txt', 'w')
for i in range(n):
    s = str(10*i)
    fout.write('(load NagaiHonda'+ s.zfill(5) +'.off geometry)\n')
    fout.write('(snapshot c0 ' + s.zfill(5) + '.rgb)\n')
    fout.write('(delete NagaiHonda' + s.zfill(5) + '.off)\n')
fout.write('(quit)')
fout.close()
