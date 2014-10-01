package bawjensen.minecraft.ann.sacrifice;

import bawjensen.minecraft.ann.LifeTracker;
import net.minecraft.entity.passive.EntityVillager;
import net.minecraft.util.DamageSource;
import net.minecraft.util.EntityDamageSource;
import net.minecraft.world.World;

/**
 * Created by bryanjensen on 9/30/14.
 */
public class SacrificialVillager extends EntityVillager {
    public SacrificialVillager(World world) {
        super(world);
    }

    @Override
    public void onDeath(DamageSource dSource) {
        super.onDeath(dSource);

        if (dSource instanceof EntityDamageSource) // Only track deaths by zombie
            LifeTracker.getInstance().endTimer(System.currentTimeMillis());

        System.out.print("Aged: ");
        System.out.print(this.getAge());
        System.out.println(" when dead.");
    }
}
