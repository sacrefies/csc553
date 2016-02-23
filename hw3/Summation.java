// ------------------------------------------------------------------------------------
//       Filename: Summation.java
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


public class Summation {


    public static void main(String[] args) {
        String usages = "\n[Usages]\n"
                + "$ java hw3.Summation <lower_bound> <upper_bound> \n"
                + "E.g.: $ java hw3.Summation 2 5000\n"
                + "\n[Note]\n"
                + " - If a decimal number is given, say 1.5, the number will be converted to 1.\n";

        // user did not specify a number
        if (args.length < 2) {
            System.out.printf(usages);
            return;
        }

        if (!isnumber(args[0])) {
            System.out.printf("Illegal input: %s%nPlease specify a lower bound.%n", args[0]);
            System.out.printf(usages);
            return;
        }
        if (!isnumber(args[1])) {
            System.out.printf("Illegal input: %s%nPlease specify an upper bound.%n", args[1]);
            System.out.printf(usages);
            return;
        }

        int upper_bound = Integer.parseInt(args[1]);
        int lower_bound = Integer.parseInt(args[0]);

        if (upper_bound <= lower_bound) {
            System.out.printf("Illegal input: %s %s%nPlease specify a valid range.%n", args[0], args[1]);
            System.out.printf(usages);
            return;
        }
        // debug
        System.out.printf("user input: lower=%d upper=%d%n", lower_bound, upper_bound);
        // prime searching begins
        SharedSum sum = new SharedSum();
        // create the thread addresses on heap.
        for (int tid = 0; tid < PTHREAD_COUNT; ++tid) {
            int[] params = slice(PTHREAD_COUNT, tid, lower_bound, upper_bound);
            if (params[0] == Integer.MIN_VALUE && params[1] == Integer.MIN_VALUE) {
                System.out.printf("Error: Failed to slice the range. for thread %d%n", tid);
                continue;
            }
            Accumulator acc = new Accumulator(tid, params[0], params[1], sum);
            Thread t = new Thread(acc);
            //new Thread(acc).start();
            t.start();
            try {
                t.join();
            } catch (InterruptedException e) {
                System.out.printf("thread %d is interrupted%n", tid);
            }
        }

        System.out.printf("The summary of [%d..%d]: %d%n", lower_bound, upper_bound, sum.getSum());
    }

    static class SharedSum {
        private long sum;

        SharedSum() {
        }

        void setSum(final long value) {
            sum = value;
        }

        long getSum() {
            return sum;
        }
    }

    /**
     * Determine whether a string is a number. E.g.: "5", "+5", "-5", "3.5", ".5", "5." are legal number forms.
     *
     * @param str A string to be examined.
     * @return Returns true if the given string is a legal number; returns false if otherwise.
     */
    private static boolean isnumber(final String str) {
        if (str == null || str.trim().equals(""))
            return false;

        int size = str.trim().length();
        int count_dot = 0, count_sign = 0, count_digit = 0;

        for (int i = 0; i < size; i++) {
            if (str.charAt(i) < '0' || str.charAt(i) > '9') {
                if (str.charAt(i) != '+' && str.charAt(i) != '.' && str.charAt(i) != '-')
                    return false;
                if (str.charAt(i) == '+' || str.charAt(i) == '-') {
                    if ((i > 0 && i < size - 1) || count_sign > 0)
                        return false;
                    count_sign++;
                }
                if (str.charAt(i) == '.') {
                    if (count_dot > 0)
                        return false;
                    count_dot++;
                }
            } else
                count_digit++;
        }

        return (count_digit >= 1);
    }


    /**
     * Get a slice of a range which is represented by a lower bound and a upper bound.
     *
     * @param slices      How many slices shall be split up?
     * @param slice_index The index of the desired slice from the number range. Starts with 0.
     * @param lower       The lower bound of a range.
     * @param upper       The upper bound of a range.
     * @return An array of {@code int} in size of 2 which are the slice's lower bound and upper bound.
     */
    private static int[] slice(final int slices, final int slice_index, final int lower, final int upper) {
        int total_size = Math.abs(upper) - Math.abs(lower) + 1;
        int size_per_piece = total_size / slices;
        int start = Integer.MIN_VALUE;
        int end = Integer.MIN_VALUE;

        if (slice_index < slices) {
            start = lower + slice_index * size_per_piece;
            end = (slice_index == slices - 1) ? upper : start + size_per_piece - 1;
        }
        return new int[]{start, end};
    }

    private static final int PTHREAD_COUNT = 4;
}
