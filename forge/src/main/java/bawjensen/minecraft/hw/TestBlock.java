package bawjensen.minecraft.hw;

import bawjensen.minecraft.hw.ann.platform.PlatformCreator;
import cpw.mods.fml.common.Mod.EventHandler;
import cpw.mods.fml.common.event.FMLPreInitializationEvent;
import net.minecraft.block.Block;
import net.minecraft.block.material.Material;
import net.minecraft.creativetab.CreativeTabs;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.world.World;

public class TestBlock extends Block {
    private int j;

    public TestBlock (Material material) {
		super(material);
		
		setHarvestLevel("shovel", 2);
		setBlockName("My Beautiful Block");
        setHardness(3.0f);
        setStepSound(soundTypePiston);
		setCreativeTab(CreativeTabs.tabBlock);
	}
	
	@EventHandler
	public void preInit(FMLPreInitializationEvent event) {
			
	}

    public boolean onBlockActivated(World world, int blockX, int blockY, int blockZ, EntityPlayer player, int par6, float par7, float par8, float par9)
    {
        if (world.isRemote)
        {
            return true;
        }
        else
        {
            PlatformCreator.createIn(world);

            return true;
        }
    }


}