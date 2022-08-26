import subprocess

p1 = subprocess.run('PalindromeSearcher.exe data/pi.txt 21 1000', shell=True)
print(p1.returncode)
