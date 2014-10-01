package bawjensen.minecraft.ann.platform;

import bawjensen.minecraft.ann.sacrifice.SacrificialVillager;
import net.minecraft.block.Block;
import net.minecraft.entity.Entity;
import net.minecraft.entity.monster.EntityZombie;
import net.minecraft.potion.PotionEffect;
import net.minecraft.util.AxisAlignedBB;
import net.minecraft.world.World;

import java.util.List;

/**
 * Created by bryanjensen on 9/30/14.
 */
public class PlatformCreator {
    public static void createIn(World world) {
        int platformBlockId = 89;
        Block platformBlock = Block.getBlockById(platformBlockId);

        int numZombies = 20;
        float inwardsOffset = 1.5f;

        int platformSizeX = 15;
        int platformSizeZ = platformSizeX;

        int platformOffsetX = platformSizeX / 2;
        int platformOffsetZ = platformSizeZ / 2;

        int x = 0 - platformOffsetX;
        int y = 200;
        int z = 0 - platformOffsetZ;

        // Clear the area
        for (int i = x-30; i < platformOffsetX+30; i++) {
            for (int j = y; j < y + 20; j++) {
                for (int k = z-30; k < platformOffsetZ+30; k++) {
                    world.setBlockToAir(i, j, k);
                }
            }
        }
        List<Entity> entities = world.getEntitiesWithinAABB(Entity.class, AxisAlignedBB.getBoundingBox(x, y, z, platformOffsetX, y + 5, platformOffsetZ));
        for (Entity entity : entities) {
            if (entity != null)
                entity.setDead();
        }

        int blockRenderFlag = 7;

        // Set the area up
        for (int i = x; i <= platformOffsetX; i++) {
            for (int k = z; k < platformOffsetZ; k++) {
                world.setBlock(i, y, k, platformBlock, 0, blockRenderFlag);
            }
        }

        int wallHeight = 2;
        for (int i = x; i <= platformOffsetX; i++) {
            for (int j = y; j <= y+wallHeight; j++) {
                world.setBlock(i, j, z,               platformBlock, 0, blockRenderFlag);
                world.setBlock(i, j, platformOffsetZ, platformBlock, 0, blockRenderFlag);
            }
        }
        for (int k = z; k < platformOffsetZ; k++) {
            for (int j = y; j <= y+wallHeight; j++) {
                world.setBlock(x,               j, k, platformBlock, 0, blockRenderFlag);
                world.setBlock(platformOffsetX, j, k, platformBlock, 0, blockRenderFlag);
            }
        }

        int perimeterSize = platformSizeX * 2 + platformSizeZ * 2;
        float blocksPerZombie = perimeterSize / (float)numZombies;

        for (float i = x+inwardsOffset; i < platformOffsetX-inwardsOffset; i += blocksPerZombie) {
            EntityZombie zombie1 = new EntityZombie(world);
            EntityZombie zombie2 = new EntityZombie(world);

            zombie1.setPositionAndUpdate( i, y+1, z+inwardsOffset);
            zombie2.setPositionAndUpdate(-i, y+1, platformOffsetZ-inwardsOffset);

            zombie1.addPotionEffect(new PotionEffect(12, Integer.MAX_VALUE, 0, true));
            zombie2.addPotionEffect(new PotionEffect(12, Integer.MAX_VALUE, 0, true));

            world.spawnEntityInWorld(zombie1);
            world.spawnEntityInWorld(zombie2);
        }

        for (float k = z+inwardsOffset; k < platformOffsetZ-inwardsOffset; k += blocksPerZombie) {
            EntityZombie zombie1 = new EntityZombie(world);
            EntityZombie zombie2 = new EntityZombie(world);

            zombie1.setPositionAndUpdate(x+inwardsOffset,               y+1,  k);
            zombie2.setPositionAndUpdate(platformOffsetX-inwardsOffset, y+1, -k);

            zombie1.addPotionEffect(new PotionEffect(12, Integer.MAX_VALUE, 0, true));
            zombie2.addPotionEffect(new PotionEffect(12, Integer.MAX_VALUE, 0, true));

            world.spawnEntityInWorld(zombie1);
            world.spawnEntityInWorld(zombie2);
        }

        SacrificialVillager villager = new SacrificialVillager(world);
        villager.setPositionAndUpdate(0, y+1, 0);
        world.spawnEntityInWorld(villager);

//        villager.onDeath();


//            player.setPositionAndUpdate(0, y+1, 0);
    }
}
