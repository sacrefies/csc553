/*------------------------------------------------------------------------------
 - File       : UnisexBathroom.java
 - Course     : csc553
 - Date:      : 03/21/2016
 - Author     : Jason Qiao Meng
 - Author ID  : 10652564
 - Description: This file contains ...
 -----------------------------------------------------------------------------*/

package csc553.hw4;


public class UnisexBathroom {

    private static Semaphore door_locks;

    public static void main(String[] args) {
        if (args.length != 2) {
            printUsage();
        }

        int totMen = 0;
        int totWomen = 0;

        try {
            totMen = Integer.parseInt(args[0]);
            totWomen = Integer.parseInt(args[1]);
            door_locks = new Semaphore(totMen, totWomen);
        } catch (NumberFormatException e) {
            printUsage();
        }

        System.out.println("Working\t\tEntering\tIn Bathroom\tLeaving");
        System.out.println("----------------------------------------------------------");

        Thread[] men = new Thread[totMen];
        for (int i = 0; i < totMen; i++) {
            men[i] = new ManThread(i);
            men[i].start();
        }

        Thread[] women = new Thread[totWomen];
        for (int i = 0; i < totWomen; i++) {
            women[i] = new WomanThread(i);
            women[i].start();
        }

        for (int i = 0; i < totMen; i++) {
            try {
                men[i].join();
            } catch (InterruptedException e) {
            }
        }

        for (int i = 0; i < totWomen; i++) {
            try {
                women[i].join();
            } catch (InterruptedException e) {
            }
        }

        System.exit(0);
    }

    private static void printUsage() {
        System.out.println("Usage: java UnisexBathroom <totMen> <totWomen>");
        System.out.println("  <totMen>: Total number of men.");
        System.out.println("  <totWomen>: Total number of women.");
        System.exit(-1);
    }

    private static void randomSleep(int max) {
        try {
            Thread.sleep((int)(Math.random() * max));
        } catch (InterruptedException e) {
        }
    }

    private static class ManThread extends Thread {

        private int id;

        ManThread(int id) {
            this.id = id;
        }

        public void run() {
            doWork();
            try {
                door_locks.acquireForMan();
                //System.out.println("Lock acquired for man " + id);
                useBathroom();
                door_locks.releaseMan();
                //System.out.println("Lock released for man " + id);
            } catch (InterruptedException ie) {
                System.out.printf("Thread " + id + " interrupted: " + ie.getMessage() + "\n");
            }
            doWork();
        }

        private void doWork() {
            System.out.println("Man " + id);
            UnisexBathroom.randomSleep(10000);
        }

        private void useBathroom() {
            //System.out.println("People indoor currently: " + door_locks.getPeopleIndoor());
            System.out.println("\t\tMan " + id);
            UnisexBathroom.randomSleep(100);
            System.out.println("\t\t\t\tMan " + id);
            UnisexBathroom.randomSleep(500);
            System.out.println("\t\t\t\t\t\tMan " + id);
            UnisexBathroom.randomSleep(100);
        }
    }

    private static class WomanThread extends Thread {

        private int id;

        WomanThread(int id) {
            this.id = id;
        }

        public void run() {
            doWork();
            try {
                door_locks.acquireForWoman();
                //System.out.println("Lock acquired for woman " + id);
                useBathroom();
                door_locks.releaseWoman();
                //System.out.println("Lock released for woman " + id);
            } catch (InterruptedException ie) {
                System.out.printf("Thread " + id + " interrupted: " + ie.getMessage() + "\n");
            }
            doWork();
        }

        private void doWork() {
            System.out.println("Woman " + id);
            UnisexBathroom.randomSleep(10000);
        }

        private void useBathroom() {
            //System.out.println("People indoor currently: " + door_locks.getPeopleIndoor());
            System.out.println("\t\tWoman " + id);
            UnisexBathroom.randomSleep(100);
            System.out.println("\t\t\t\tWoman " + id);
            UnisexBathroom.randomSleep(500);
            System.out.println("\t\t\t\t\t\tWoman " + id);
            UnisexBathroom.randomSleep(100);
        }
    }


    private static class Semaphore {

        Semaphore(int men, int women) {
            if (men < 1)
                throw new IllegalArgumentException("Permit bound must be a positive number. men = " + men);
            if (women < 1)
                throw new IllegalArgumentException("Permit bound must be a positive number. women = " + women);
            this.men = men;
            this.women = -women;
            peopleIndoor = 0;
        }


        int getPeopleIndoor() {
            return peopleIndoor;
        }


        synchronized void acquireForMan() throws InterruptedException {
            while (peopleIndoor < 0 || peopleIndoor == men)
                wait();
            peopleIndoor++;
            notifyAll();
        }


        synchronized void acquireForWoman() throws InterruptedException {
            while (peopleIndoor > 0 || peopleIndoor == women)
                wait();
            peopleIndoor--;
            notifyAll();
        }


        synchronized void releaseMan() throws InterruptedException {
            while (peopleIndoor <= 0)
                wait();
            peopleIndoor--;
            notifyAll();
        }


        synchronized void releaseWoman() throws InterruptedException {
            while (peopleIndoor >= 0)
                wait();
            peopleIndoor++;
            notifyAll();
        }

        private int men;
        private int women;
        private int peopleIndoor;
    }
}
