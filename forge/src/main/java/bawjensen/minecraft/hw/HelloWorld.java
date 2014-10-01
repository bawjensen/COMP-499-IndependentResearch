package bawjensen.minecraft.hw;

import bawjensen.minecraft.ann.sacrifice.SacrificialVillager;
import cpw.mods.fml.common.Mod;
import cpw.mods.fml.common.Mod.EventHandler;
import cpw.mods.fml.common.Mod.Instance;
import cpw.mods.fml.common.SidedProxy;
import cpw.mods.fml.common.event.FMLInitializationEvent;
import cpw.mods.fml.common.event.FMLPostInitializationEvent;
import cpw.mods.fml.common.event.FMLPreInitializationEvent;
import cpw.mods.fml.common.registry.EntityRegistry;
import cpw.mods.fml.common.registry.GameRegistry;
import net.minecraft.block.Block;
import net.minecraft.block.material.Material;

//import cpw.mods.fml.common.Mod.PreInit;    // used in 1.5.2
//import cpw.mods.fml.common.Mod.Init;       // used in 1.5.2
//import cpw.mods.fml.common.Mod.PostInit;   // used in 1.5.2
//import cpw.mods.fml.common.network.NetworkMod; // not used in 1.7

@Mod(modid="HelloWorldModID", name="HelloWorld", version="0.0.0")
//@NetworkMod(clientSideRequired=true) // not used in 1.7
public class HelloWorld {
	
	public final static Block testBlock = new TestBlock(Material.rock);

    // The instance of your mod that Forge uses.
    @Instance(value = "HelloWorldModID")
    public static HelloWorld instance;

    // Says where the client and server 'proxy' code is loaded.
    @SidedProxy(clientSide="bawjensen.minecraft.hw.client.ClientProxy", serverSide="bawjensen.minecraft.hw.CommonProxy")
    public static CommonProxy proxy;

    @EventHandler // used in 1.6.2
    //@PreInit    // used in 1.5.2
    public void preInit(FMLPreInitializationEvent event) {
            // Stub Method
    }

    @EventHandler // used in 1.6.2
    //@Init       // used in 1.5.2
    public void load(FMLInitializationEvent event) {
        proxy.registerRenderers();

        GameRegistry.registerBlock(testBlock, "testBlock");
        EntityRegistry.registerModEntity(SacrificialVillager.class, "sacVill", 0, HelloWorld.instance, 80, 3, true);
    }

    @EventHandler // used in 1.6.2
    //@PostInit   // used in 1.5.2
    public void postInit(FMLPostInitializationEvent event) {
            // Stub Method
    }
}