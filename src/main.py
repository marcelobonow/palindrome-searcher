import os
import subprocess

os.chdir("../bin/Release")
p1 = subprocess.run(
    'PalindromeSearcher.exe data/pi.txt output.txt 12 10000', shell=True)

print("AQUI")
