enum class TAG_RESOURCE
{
//Character
    Character_F,
    DeathDropBox,

//Item
    //Ammunition
    Ammu_5_56mm,
    Ammu_7_62mm,

    //Attachment
    ACOG,
    Aimpoint2X,
    RedDot,

    //Consumable
    AdrenalineSyringe,
    Bandage,
    EnergyDrink,
    FirstAidKit,
    JerryCan,
    MedKit,
    Painkiller,

    //Equipment
    Armor_Lv1,
    Back_Lv1,
    Head_Lv1,
//Map
    //Building
    AbandonedTownHall,
    Artifact,
    Church,
    Museum,
    OldWoodenShed_1,
    OldWoodenShed_2,
    OldWoodenShed_3,
    PoliceStation,
    WareHouse_A,
    WareHouse_B,

    //Exterior
        //-AmmoBox
    AmmoBox_1,
    AmmoBox_2,

        //- BrokenVehicle
    BrokenBus,
    BrokenCar,
    BrokenMeshTruck,
    BrokenPoliceCar,
    BrokenTractorGunnyBag,
    BrokenUaz3151,

    CityStreetSign,

        //-Container
    ContainerBox_A,
    ContainerBox_B,
    ContainerSmall_1,
    ContainerSmall_2,

        //- HayBale
    HeyBale_1,
    HeyBale_2,

    Lighthouse,

        //- MetalBarrel
    MetalBarrel_Blue,
    MetalBarrel_Gray,
    MetalBarrel_Green,
    MetalBarrel_Red,

        //- MetalFence
    MetalFence_A,
    MetalFence_E,
    MetalFence_F,
    MetalFence_Long,

    MetalShelf,
    
        //- PicketFence
    PicketFence_Long_B,
    PicketFence_Short_A,
    PicketFence_Short_B,
    PicketFence_Short_C,
    PicketFence_Short_Long_A,

    PlasticBarrel,

        //- Powerline
    Powerline_1,
    Powerline_2,

        //- RadioTower
    RadioTower_1,
    RadioTower_2,

        //- SandBag
    SandBag_1,
    SandBag_2,

        //- Silo
    Silo_A,
    Silo_B,

    TableSet,

    Tower,

    //SkySphere
    SkySphere,

    //Vegetation
        //- Bush
    DeadGrass,
    Dogwood,

        //- Grass
    Grass_1,
    Grass_2,
    Grass_3,

        // - Rock
    Desert_Cover_Rock_Combine_1,
    Desert_Cover_Rock_Combine_2,
    Desert_Mashup_1,
    Desert_Mashup_2,
    Desert_Mashup_3,
    Rock_1,
    Rock_2,

        // - Tree
    AlaskaCedar,
    AmericanElem,
    BlueSpruce,
    LondonPlane,
    ScotsPine, 

//Weapon
        //- AR
    QBZ,
        // - SR
    Kar98k,

//    
    COUNT
};
const char* ComboObjectList[] = 
{
    "Character_F",
    "DeathDropBox",
    "Ammu_5_56mm",
    "Ammu_7_62mm",
    "ACOG",
    "Aimpoint2X",
    "RedDot",
    "AdrenalineSyringe",
    "Bandage",
    "EnergyDrink",
    "FirstAidKit",
    "JerryCan",
    "MedKit",
    "Painkiller",
    "Armor_Lv1",
    "Back_Lv1",
    "Head_Lv1",
    "AbandonedTownHall",
    "Artifact",
    "Church",
    "Museum",
    "OldWoodenShed_1",
    "OldWoodenShed_2",
    "OldWoodenShed_3",
    "PoliceStation",
    "WareHouse_A",
    "WareHouse_B",
    "AmmoBox_1",
    "AmmoBox_2",
    "BrokenBus",
    "BrokenCar",
    "BrokenMeshTruck",
    "BrokenPoliceCar",
    "BrokenTractorGunnyBag",
    "BrokenUaz3151",
    "CityStreetSign",
    "ContainerBox_A",
    "ContainerBox_B",
    "ContainerSmall_1",
    "ContainerSmall_2",
    "HeyBale_1",
    "HeyBale_2",
    "Lighthouse",
    "MetalBarrel_Blue",
    "MetalBarrel_Gray",
    "MetalBarrel_Green",
    "MetalBarrel_Red",
    "MetalFence_A",
    "MetalFence_E",
    "MetalFence_F",
    "MetalFence_Long",
    "MetalShelf",
    "PicketFence_Long_B",
    "PicketFence_Short_A",
    "PicketFence_Short_B",
    "PicketFence_Short_C",
    "PicketFence_Short_Long_A",
    "PlasticBarrel",
    "Powerline_1",
    "Powerline_2",
    "RadioTower_1",
    "RadioTower_2",
    "SandBag_1",
    "SandBag_2",
    "Silo_A",
    "Silo_B",
    "TableSet",
    "Tower",
    "SkySphere",
    "DeadGrass",
    "Dogwood",
    "Grass_1",
    "Grass_2",
    "Grass_3",
    "Desert_Cover_Rock_Combine_1",
    "Desert_Cover_Rock_Combine_2",
    "Desert_Mashup_1",
    "Desert_Mashup_2",
    "Desert_Mashup_3",
    "Rock_1",
    "Rock_2",
    "AlaskaCedar",
    "AmericanElem",
    "BlueSpruce",
    "LondonPlane",
    "ScotsPine",
    "QBZ",
    "Kar98k"
};