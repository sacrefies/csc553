// ------------------------------------------------------------------------------------
//       Filename: Accumulator.java
//       Revision: $Id$
//    Description: This file includes a program which uses multithreading to sum up a large
//                 range of numbers.
//        Created: 02/21/2016 01:20:29 PM
//       Compiler: GCC
//         Author: Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng
// ------------------------------------------------------------------------------------

// package csc553.hw3;

public class Accumulator implements Runnable {
    private int start;
    private int end;
    private int tid;
    private Summation.SharedSum sum;

    public Accumulator(final int id, final int start, final int end, Summation.SharedSum sum) {
        this.start = start;
        this.end = end;
        tid = id;
        this.sum = sum;
    }

    /**
     * If this thread was constructed using a separate <code>Runnable</code> run object, then that <code>Runnable</code>
     * object's <code>run</code> method is called; otherwise, this method does nothing and returns.
     * <p/>
     * Subclasses of <code>Thread</code> should override this method.
     *
     * @see #start()
     * @see #stop()
     * @see {@link #Thread(ThreadGroup, Runnable, String)}
     */
    @Override
    public void run() {
        // debug
        System.out.printf("thread_id: %d, start: %d, end: %d\n", tid, start, end);
        for (int i = start; i <= end; i++)
            sum.setSum(sum.getSum() + i);
    }
}
