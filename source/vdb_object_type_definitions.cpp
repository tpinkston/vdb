// ============================================================================
// VDB (VDIS Debugger)
// Tony Pinkston (git@github.com:tpinkston/vdb.git)
//
// VDB is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// VDB is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
// details (http://www.gnu.org/licenses).
// ============================================================================

// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#include "vdb_object_type.h"

void vdb::object_type_data::add_all(void)
{
    add(0,5,0,0,"Tactical Smoke","point");
    add(0,5,1,0,"Tactical Smoke Ground Burst","point");
    add(0,5,1,1,"Tactical Smoke Ground Burst Artillery","point");
    add(0,5,2,0,"Tactical Smoke Air Burst","point");
    add(0,5,2,1,"Tactical Smoke Air Burst Grenade","point");
    add(0,5,3,0,"Tactical Smoke Canister","point");
    add(0,5,3,1,"M83, White","point");
    add(0,5,3,2,"M18, Green","point");
    add(0,5,3,3,"M18, Violet","point");
    add(0,5,3,4,"M18, Yellow","point");
    add(0,5,3,5,"M18, Red","point");
    add(1,1,0,0,"Obstacle","point");
    add(1,1,1,0,"Abatis","point");
    add(1,1,1,1,"Abatis 8 Tree","point");
    add(1,1,1,2,"Abatis 14 Tree","point");
    add(1,1,2,0,"Log Crib","point");
    add(1,1,2,1,"Log Crib Rectangular","point");
    add(1,1,2,2,"Log Crib Triangular","point");
    add(1,1,3,0,"Crater","point");
    add(1,1,3,1,"Crater Small","point");
    add(1,1,3,2,"Crater Medium","point");
    add(1,1,3,3,"Crater Large","point");
    add(1,1,4,0,"Barrier","point");
    add(1,1,4,1,"Dragons Teeth","point");
    add(1,1,4,2,"Barrier, HESCO Basket, Small","point");
    add(1,1,4,3,"Barrier, HESCO Basket, Medium","point");
    add(1,1,4,4,"Barrier, HESCO Basket, Large","point");
    add(1,1,4,5,"Barrier, HESCO Basket, Double-Stacked","point");
    add(1,1,4,6,"Barrier, Construction","point");
    add(1,1,4,7,"Barrier, Jersey, Plastic","point");
    add(1,1,4,8,"Barrier, Fence, Chain, 6-foot","point");
    add(1,1,4,9,"Barrier, Fence, Wood, 6-foot","point");
    add(1,1,4,10,"Barrier, Texas","point");
    add(1,1,5,0,"Rock Drop","point");
    add(1,1,5,1,"Rock Drop Covered","point");
    add(1,1,5,2,"Rock Drop Uncovered","point");
    add(1,1,9,0,"Pot Hole","point");
    add(1,2,0,0,"Prepared Position","point");
    add(1,2,1,0,"Vehicle Defilade","point");
    add(1,2,1,1,"Armored Vehicle Defilade","point");
    add(1,2,1,2,"Fighting Vehicle Defilade","point");
    add(1,2,1,3,"Mortar Carrier Defilade","point");
    add(1,2,1,4,"Tank Defilade","point");
    add(1,2,2,0,"Infantry Fighting Position","point");
    add(1,2,2,1,"Covered Machine Gun Bunker","point");
    add(1,2,2,2,"Overhead Covered Infantry Position","point");
    add(1,2,2,3,"Non Covered Infantry Position","point");
    add(1,2,2,4,"Non Covered Machine Gun Bunker","point");
    add(1,2,2,5,"Hasty Fighting Position","point");
    add(1,3,0,0,"Cultural Feature","point");
    add(1,3,1,0,"Building","point");
    add(1,3,1,1,"Church","point");
    add(1,3,1,2,"Apartment Building","point");
    add(1,3,1,3,"Government Building","point");
    add(1,3,1,4,"Industrial Building","point");
    add(1,3,1,5,"Hangar","point");
    add(1,3,1,6,"Microwave Tower","point");
    add(1,3,1,7,"Power Pylon","point");
    add(1,3,1,8,"Radio TV Tower","point");
    add(1,3,1,9,"School","point");
    add(1,3,1,10,"Transformer Yard","point");
    add(1,3,1,11,"Insurgent Building","point");
    add(1,3,1,12,"Radio Tower, 100ft","point");
    add(1,3,1,13,"Radio Tower, 500ft","point");
    add(1,3,1,14,"Radio Tower, 1000ft","point");
    add(1,3,1,15,"Porta Potty","point");
    add(1,3,1,16,"FOB, Trailer Office","point");
    add(1,3,1,17,"FOB, Guard Tower","point");
    add(1,3,1,18,"Guard House","point");
    add(1,3,1,200,"Destructible Building","point");
    add(1,3,2,0,"Building Rubble","point");
    add(1,3,3,0,"Disturbed Earth","point");
    add(1,3,4,0,"Disturbed Road","point");
    add(1,3,4,1,"Disturbed Road, Gravel","point");
    add(1,3,4,2,"Disturbed Road, Asphalt","point");
    add(1,3,4,3,"Disturbed Road, Concrete","point");
    add(1,3,5,0,"Tent","point");
    add(1,3,5,1,"Tent, Small","point");
    add(1,3,5,2,"Tent, Medium","point");
    add(1,3,5,3,"Tent, Large","point");
    add(1,3,5,4,"Tent, Modular General Purpose Tent System (MGPTS)","point");
    add(1,3,5,5,"Tent, Arctic","point");
    add(1,3,5,6,"Tent, TEMPER","point");
    add(1,3,5,7,"Tent, Expandable Frame","point");
    add(1,3,5,8,"Tent, Fritsche","point");
    add(1,3,5,9,"Tent, Bedouin","point");
    add(1,3,5,10,"Tent, Chemically and Biological Protected Shelter (CBPS)","point");
    add(1,3,5,11,"Tent, Kuchi","point");
    add(1,3,6,0,"Maintenance Structure","point");
    add(1,3,6,1,"Lightweight Maintenance Enclosure (LME), Bradley","point");
    add(1,3,6,2,"Lightweight Maintenance Enclosure (LME), M1","point");
    add(1,3,6,3,"Large Area Maintenance Shelter (LAMS) Vehicle Maintenance (VM)","point");
    add(1,3,6,4,"Large Area Maintenance Shelter (LAMS) Aviation Maintenance (AM)","point");
    add(1,3,7,0,"MOUT Building","point");
    add(1,3,7,1,"MOUT Building, House","point");
    add(1,3,7,2,"MOUT Building, Hospital","point");
    add(1,3,7,3,"MOUT Building, Gas Station","point");
    add(1,3,7,4,"MOUT Building, Store","point");
    add(1,3,7,5,"MOUT Building, Office Building","point");
    add(1,3,7,6,"MOUT Building, Warehouse","point");
    add(1,3,7,7,"MOUT Building, Control Tower","point");
    add(1,3,7,8,"MOUT Building, Water Tower","point");
    add(1,3,7,9,"MOUT Building, Police Station","point");
    add(1,3,7,10,"MOUT Building, Fire Station","point");
    add(1,3,7,11,"MOUT Building, Power Station","point");
    add(1,3,7,12,"MOUT Building, Apartment","point");
    add(1,3,7,13,"MOUT Building, School","point");
    add(1,3,7,14,"MOUT Building, Church","point");
    add(1,3,7,15,"MOUT Building, Farm House","point");
    add(1,3,7,16,"MOUT Building, Mudbrick House","point");
    add(1,3,7,17,"MOUT Building, Mudbrick Store","point");
    add(1,3,8,0,"Container-Based Building","point");
    add(1,3,8,1,"Container-Based Building, Store, Single","point");
    add(1,3,8,2,"Container-Based Building, House, Single","point");
    add(1,3,8,3,"Container-Based Building, House, Single, Railing","point");
    add(1,3,8,4,"Container-Based Building, House, Double","point");
    add(1,3,8,5,"Container-Based Building, Mosque, Single","point");
    add(1,3,8,6,"Container-Based Building, Bridge, Single","point");
    add(1,3,8,7,"Container-Based Building, FOB","point");
    add(1,3,10,0,"Tree, Deciduous","point");
    add(1,3,10,1,"Tree, Deciduous, Small","point");
    add(1,3,10,2,"Tree, Deciduous, Medium","point");
    add(1,3,10,3,"Tree, Deciduous, Large","point");
    add(1,3,11,0,"Tree, Evergreen","point");
    add(1,3,11,1,"Tree, Evergreen, Small","point");
    add(1,3,11,2,"Tree, Evergreen, Medium","point");
    add(1,3,11,3,"Tree, Evergreen, Large","point");
    add(1,3,12,0,"Pump","point");
    add(1,3,12,1,"Pump, Gas","point");
    add(1,3,14,0,"Utility Pole","point");
    add(1,4,0,0,"Passageway","point");
    add(1,4,1,0,"Stationary Bridge","point");
    add(1,4,1,1,"Stationary Bridge 2 Lane","point");
    add(1,4,1,2,"Stationary Bridge 4 Lane","point");
    add(1,4,1,200,"Destructible Bridge","point");
    add(1,4,1,201,"Destructible Bridge Span","point");
    add(1,4,2,0,"Launched Bridge","point");
    add(1,4,2,1,"AVLB M60A1","point");
    add(1,4,2,2,"AVLB MTU20","point");
    add(1,4,2,3,"Joint Assault Bridge (JAB)","point");
    add(1,4,3,0,"Ribbon Bridge","point");
    add(1,4,3,1,"Ribbon Bridge 2 Lane","point");
    add(1,4,3,2,"Ribbon Bridge 4 Lane","point");
    add(1,4,4,0,"Pier","point");
    add(1,6,2,0,"NBC Hazard Marker","point");
    add(1,8,0,0,"Environmental Object","point");
    add(1,8,1,0,"Flood","point");
    add(1,8,1,1,"Flood, Small","point");
    add(1,8,1,2,"Flood, Medium","point");
    add(1,8,1,3,"Flood, Large","point");
    add(0,5,0,0,"Tactical Smoke","linear");
    add(0,5,1,0,"Exhaust Smoke","linear");
    add(1,1,0,0,"Obstacle","linear");
    add(1,1,1,0,"Tank Ditch","linear");
    add(1,1,2,0,"Concertina Wire","linear");
    add(1,1,2,1,"Concertina Wire 2 Roll","linear");
    add(1,1,2,2,"Concertina Wire 3 Roll","linear");
    add(1,1,3,0,"Concrete Barrier","linear");
    add(1,1,4,0,"Speed Bump","linear");
    add(1,1,5,0,"Rut","linear");
    add(1,3,0,0,"Cultural Feature","linear");
    add(1,3,1,0,"Wire","linear");
    add(1,3,1,1,"Wire, Crush","linear");
    add(1,3,2,0,"Tracks, Tire","linear");
    add(1,6,0,0,"Obstacle Marker","linear");
    add(1,6,1,0,"Minefield Lane Marker","linear");
    add(1,7,0,0,"Obstacle Breach","linear");
    add(1,7,1,0,"Breach","linear");
    add(0,1,0,0,"Obstacle","areal");
    add(0,1,1,0,"Minefield","areal");
    add(0,1,1,1,"Minefield Hasty","areal");
    add(0,1,1,2,"Minefield Prepared","areal");
    add(0,1,1,3,"Minefield Scattered","areal");
    add(0,1,1,4,"Minefield Solitary","areal");
}

