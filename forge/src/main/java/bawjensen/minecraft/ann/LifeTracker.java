package bawjensen.minecraft.ann;

/**
 * Created by bryanjensen on 9/30/14.
 */
public class LifeTracker {
    private long startTime;
    private boolean tracking;

    private static LifeTracker instance;

    public static LifeTracker getInstance() {
        if (instance == null)
            instance = new LifeTracker();
        return instance;
    }

    public void startTimer(long sTime) {
        this.startTime = sTime;
        this.tracking = true;
        System.out.print("Starting timer at: ");
        System.out.println(sTime);
    }

    public void endTimer(long eTime) {
        this.tracking = false;
        System.out.print("Timer lasted: ");
        System.out.print((float) (eTime - this.startTime) / 1000);
        System.out.println(" sec");
    }
}
