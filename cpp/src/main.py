import os
import subprocess
import urllib.request
from pydrive.drive import GoogleDrive


def reporthook(count, block_size, total_size):
    if int(count * block_size * 100 / total_size) == 100:
        print('Download completed!')


def Main():
    CLIENT_SECRET_FILE = 'client_secret_GoogleCloudDemo.json'
    API_NAME = "drive"
    API_VErSION = 'vr'
    SCOPES = ['http://www.googleapis.com/auth/drive']

    service = Create_Service(CLIENT_SECRET_FILE, API_NAME, API_VERSION, SCOPES)

    urllib.request.urlretrieve(
        "https://drive.google.com/uc?export=download&id=1PAHIhsYYKN222Ylehk0Avuq4KCnK_MWn", "pi.ycd", reporthook)
    # os.chdir("../bin/Release")
    # p1 = subprocess.run(
    #     'PalindromeSearcher.exe data/pi.txt output.txt 12 10000', shell=True)


Main()
