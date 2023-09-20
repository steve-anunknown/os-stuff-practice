import java.util.concurrent.ForkJoinPool;
// compiled with javac MergeSort.java -Xlint:-serial
public class MergeSort
{
    public static void main(String[] args)
    {
        // argument is the number of elements to sort.
        // check that there is one argument
        if (args.length != 1)
        {
            System.out.println("Usage: java MergeSort n");
            System.exit(1);
        }
        int n = Integer.parseInt(args[0]);
        // check if argument is positive
        if (n <= 0)
        {
            System.out.println("Argument must be positive.");
            System.exit(1);
        }
        // create an array of random numbers
        Integer[] array = new Integer[n];
        for (int i = 0; i < array.length; i++)
        {
            array[i] = (int)(Math.random() * n);
        }
        // print the array
        System.out.println("Before sorting:");
        for (int i = 0; i < array.length; i++)
        {
            System.out.print(array[i] + " ");
        }
        // create a ForkJoinPool to run the task
        ForkJoinPool pool = new ForkJoinPool();
        // create the task
        MergeSortAction<Integer> task = new MergeSortAction<Integer>(array, 0, array.length - 1);
        // run the task
        pool.invoke(task);
        pool.shutdown();
        // print the result
        System.out.println("\nAfter sorting:");
        for (int i = 0; i < array.length; i++)
        {
            System.out.print(array[i] + " ");
        }
        System.out.println();
    }
}
