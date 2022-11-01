import os
import socket
import threading
import time
import pyfiglet
import json

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)



def convert_to_base(n, base):
    digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    if n < base:
        return digits[n]
    else:
        return convert_to_base(n // base, base) + digits[n % base]


def main():
    ipp = lambda ip: [int(i) for i in ip.split(".")]
    c36 = lambda n: convert_to_base(n, 36)
    ip_c = os.popen("curl -s ifconfig.me").read()
if __name__ == "__main__":
    main()