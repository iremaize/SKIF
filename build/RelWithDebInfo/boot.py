import os
import shutil
import subprocess

def replace_line(file_name, line_num, text):
    lines = open(file_name, 'r').readlines()
    lines[line_num] = text
    out = open(file_name, 'w')
    out.writelines(lines)
    out.close()
procs=[]
r=[64, 70, 80, 90, 100, 128]
k=0

for i in r:
    shutil.copyfile("run.mac", "run"+str(k)+".mac")
    replace_line("run"+str(k)+".mac", 10, "/customiseDet/deadendThickness "+str(i)+" mm" + "\n")
    replace_line("run" + str(k) + ".mac", 21, "/analysis/setFileName hist" + str(i).replace('.','_') + ".csv" + "\n")
    procs.append(subprocess.Popen('exampleB1.exe run'+str(k)+'.mac'))
    k+=1
    
    
for p in procs:
    p.wait()
'''
k=0
for i in r:

    replace_line('plotHistoSingle.C', 14, 'TFile f("hist' + str(k) + '.root");' + "\n")
    replace_line('plotHistoSingle.C', 47, 'c1->Print("d37_'+str(i).replace('.', '_') + '.pdf");' + "\n")
    res = subprocess.call('root plotHistoSingle.C', shell = True)
    k+=1
'''
for i in range(0, k-1):
    os.remove("run"+str(k)+".mac")

