import java.util.Arrays;
import java.util.concurrent.RecursiveAction;

public class MergeSortAction<T extends Comparable<T>> extends RecursiveAction
{
    private int threshold = 100;
    private T[] array;
    private int start;
    private int end;

    public MergeSortAction(T[] array, int start, int end)
    {
        this.array = array;
        this.start = start;
        this.end = end;
    }
    protected void compute()
    {
        if (end - start >= threshold)
        {
            // divide and conquer
            MergeSortAction<T> left = new MergeSortAction<T>(array, start, (start + end) / 2);
            MergeSortAction<T> right = new MergeSortAction<T>(array, (start + end) / 2, end);
            invokeAll(left, right);
            // merge the two sorted halves
            merge(array, start, (start + end)/2,  end);
        }
        else
        {
            // if you want something done right, do it yourself
            Arrays.sort(array, start, end + 1);
        }
    }
    protected void merge(T[] array, int start, int mid, int end)
    {
        // conveniently taken from
        // https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/RecursiveAction.html
        T[] buffer = Arrays.copyOfRange(array, start, mid);
        for (int i = 0, j = start, k = mid; i < buffer.length; j++)
        {
            array[j] = (k == end || buffer[i].compareTo(array[k]) < 0) ? buffer[i++] : array[k++];
        }
    }
}