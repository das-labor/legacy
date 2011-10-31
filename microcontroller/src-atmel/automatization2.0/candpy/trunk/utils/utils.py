#!/usr/bin/env python


def invertDict(data, uniqvalues=True):
    """
    a function to invert a dict struct. since the keys must be uniq we do
    also assume the values are uniq. If this is not the case we should use
    lists at this point.

    Example:
    a={'alex':0x00,'klaus':0x01}
    b=invertDict(a)
    b will be {0x00:'alex',0x01:'klaus'}
    """
    b = {}
    for k in data.keys():
        b[data[k]] = k

    return b


def hexdump(data):
    """ Return the content of the given (byte-) array as a nice hex-string """
    ret = ""
    for c in data:
        ret += "%02x " % c
    return ret


if __name__ == '__main__':
    a = {'alex': 0x00, 'klaus': 0x01}
    print invertDict(a)
