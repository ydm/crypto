#!/usr/bin/env python

import struct

import des
from des import core


def main():
    # ecb
    key = des.DesKey(b'password')
    print('ECB')
    enc = key.encrypt(b'Hello, DES world')
    for b in enc:
        print(b)
    # cbc
    print('\nCBC')
    cbc = key.encrypt(b'Hello, DES world', b'initial!')
    for b in cbc:
        print(b)
    dec = key.decrypt(cbc, b'initial!')
    print('CBC decoded:', dec)


if __name__ == '__main__':
    main()
