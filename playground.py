#!/usr/bin/env python

import struct

import des
from des import core


def main():
    key = des.DesKey(b'some key')
    from pprint import pprint
    pprint(key._DesKey__encryption_key)
    enc = key.encrypt(b'some msg')
    for b in enc:
        print(b)

    print(enc)
    print(struct.unpack('>Q', enc))


if __name__ == '__main__':
    main()
