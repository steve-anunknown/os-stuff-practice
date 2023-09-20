import java.util.Arrays;
import java.util.concurrent.RecursiveAction;
public class QuickSortAction<T extends Comparable<T>> extends RecursiveAction
{
    private int threshold = 100;
    private T[] array;
    private int start;
    private int end;

    public QuickSortAction(T[] array, int start, int end)
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
            int pivot = partition(array, start, end);
            invokeAll(new QuickSortAction<T>(array, start, pivot - 1),
                      new QuickSortAction<T>(array, pivot + 1, end));
        }
        else
        {
            // if you want something done right, do it yourself
            Arrays.sort(array, start, end + 1);
        }
    }
    protected int partition(T[] array, int start, int end)
    {
        // pick the first element as the pivot
        T pivot = array[start];
        int low = start + 1;
        int high = end;
        while (high > low)
        {
            // search forward from left
            while (low <= high && array[low].compareTo(pivot) <= 0)
            {
                low++;
            }
            // search backward from right
            while (low <= high && array[high].compareTo(pivot) > 0)
            {
                high--;
            }
            // swap two elements in the list
            if (high > low)
            {
                T temp = array[high];
                array[high] = array[low];
                array[low] = temp;
            }
        }
        while (high > start && array[high].compareTo(pivot) >= 0)
        {
            high--;
        }
        // swap pivot with list[high]
        if (pivot.compareTo(array[high]) > 0)
        {
            array[start] = array[high];
            array[high] = pivot;
            return high;
        }
        else
        {
            return start;
        }
    }
}