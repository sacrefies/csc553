#! /usr/bin/env python
# -*- coding: ascii -*-

__rev__ = """$Id$"""
__created__ = """04/08/2016 01:20:29 PM"""
__author__ = """Jason Meng (jm)"""
__email__ = """jm652564@sju.edu"""
__doc__ = """This module simulates disk schedulers' algorithms.
Implemented: SSTF, SCAN
"""


def sstf_comp(i, j, x):
    former = abs(i - x)
    latter = abs(j - x)
    if former < latter:
        return -1
    elif former > latter:
        return 1
    else:
        return 0


def scan_rearrange_req(head_pos, requests, dir):
    requests.sort()
    start = 0
    for r in requests:
        if (r >= head_pos):
            start = requests.index(r)
            break
    req = requests[0:start]
    req.sort(reverse=True)
    return (requests[start:len(requests)] + req) if dir else (req + requests[start:len(requests)])


def sstf_seq(head_pos, requests):
    print "SSTF - init HEAD pos: %s" % head_pos
    print "Requests: ", requests
    print "Service Seq: ",
    req = list(requests)
    while len(req) > 0:
        req.sort(cmp=lambda i, j: sstf_comp(i, j, head_pos))
        head_pos = req.pop(0)
        print head_pos,
    print "\n"


def scan_seq(head_pos, requests, dir):
    '''dir = true means moving to the end,
    otherwise means head is moving to 0
    '''
    print "SCAN - init HEAD pos: %s" % head_pos
    print "Direction - %s" % ("To end" if dir else "To beginning")
    print "Requests: ", requests
    new_req = scan_rearrange_req(head_pos, list(requests), dir)
    print "Service Seq: ", new_req


if __name__ == '__main__':
    requests = [2069, 1212, 2296, 2800, 544, 1618, 356, 1523, 4965, 3681]
    head_at = 2150

    sstf_seq(head_at, requests)
    scan_seq(head_at, requests, True)
    scan_seq(head_at, requests, False)
