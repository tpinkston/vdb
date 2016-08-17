// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#ifndef LF_CAT_LAND_H
#define LF_CAT_LAND_H

namespace vdb
{
    typedef enum
    {
        LF_CAT_LAND_OTH = 0,
        LF_CAT_LAND_DIW = 1,
        LF_CAT_LAND_DINV = 2,
        LF_CAT_LAND_CIVU = 3,
        LF_CAT_LAND_CIVW = 4,
        LF_CAT_LAND_ANI = 5,
        LF_CAT_LAND_MILITARY = 6,
        LF_CAT_LAND_ARMY = 7,
        LF_CAT_LAND_NAVY = 8,
        LF_CAT_LAND_MARINES = 9,
        LF_CAT_LAND_AIR_FORCE = 10,
        LF_CAT_LAND_COAST_GUARD = 11,
        LF_CAT_LAND_SPECIAL_FORCES = 12,
        LF_CAT_LAND_MERCHANT_MARINE = 13,
        LF_CAT_LAND_NATIONAL_GUARD = 14,
        LF_CAT_LAND_RESERVED_15 = 15,
        LF_CAT_LAND_RESERVED_16 = 16,
        LF_CAT_LAND_RESERVED_17 = 17,
        LF_CAT_LAND_RESERVED_18 = 18,
        LF_CAT_LAND_RESERVED_19 = 19,
        LF_CAT_LAND_RESERVED_20 = 20,
        LF_CAT_LAND_RESERVED_21 = 21,
        LF_CAT_LAND_RESERVED_22 = 22,
        LF_CAT_LAND_RESERVED_23 = 23,
        LF_CAT_LAND_RESERVED_24 = 24,
        LF_CAT_LAND_PARAMILITARY = 25,
        LF_CAT_LAND_MILITIA = 26,
        LF_CAT_LAND_GUERILLAS = 27,
        LF_CAT_LAND_INSURGENTS = 28,
        LF_CAT_LAND_TERRORIST_COMBATANTS = 29,
        LF_CAT_LAND_REBELS = 30,
        LF_CAT_LAND_WARLORD_ARMY_PERSONNEL = 31,
        LF_CAT_LAND_RESERVED_32 = 32,
        LF_CAT_LAND_RESERVED_33 = 33,
        LF_CAT_LAND_RESERVED_34 = 34,
        LF_CAT_LAND_RESERVED_35 = 35,
        LF_CAT_LAND_RESERVED_36 = 36,
        LF_CAT_LAND_RESERVED_37 = 37,
        LF_CAT_LAND_RESERVED_38 = 38,
        LF_CAT_LAND_RESERVED_39 = 39,
        LF_CAT_LAND_GENERAL = 40,
        LF_CAT_LAND_MANAGEMENT = 41,
        LF_CAT_LAND_BUSINESS = 42,
        LF_CAT_LAND_FINANCIAL = 43,
        LF_CAT_LAND_COMPUTER = 44,
        LF_CAT_LAND_ARCHITECTURE = 45,
        LF_CAT_LAND_ENGINEERING = 46,
        LF_CAT_LAND_SCIENCE = 47,
        LF_CAT_LAND_SOCIAL_SCIENCES = 48,
        LF_CAT_LAND_LEGAL = 49,
        LF_CAT_LAND_EDUCATION = 50,
        LF_CAT_LAND_ARTS_DESIGN = 51,
        LF_CAT_LAND_ENTERTAINMENT = 52,
        LF_CAT_LAND_SPORTS = 53,
        LF_CAT_LAND_MEDIA = 54,
        LF_CAT_LAND_HEALTHCARE = 55,
        LF_CAT_LAND_PROTECTIVE_SERVICE = 56,
        LF_CAT_LAND_FOOD_SERVICES = 57,
        LF_CAT_LAND_CLEANING_MAINTENANCE = 58,
        LF_CAT_LAND_PERSONAL_CARE = 59,
        LF_CAT_LAND_SALES = 60,
        LF_CAT_LAND_OFFICE_SUPPORT = 61,
        LF_CAT_LAND_AGRICULTURE = 62,
        LF_CAT_LAND_CONSTRUCTION = 63,
        LF_CAT_LAND_EXTRACTION = 64,
        LF_CAT_LAND_INSTALL_MAINTENANCE = 65,
        LF_CAT_LAND_PRODUCTION = 66,
        LF_CAT_LAND_TRANSPORTATION = 67,
        LF_CAT_LAND_MATERIAL_MOVING = 68,
        LF_CAT_LAND_RELIGION = 69,
        LF_CAT_LAND_GOVERNMENT = 70,
        LF_CAT_LAND_NON_GOVERNMENT_ORG = 71,
        LF_CAT_LAND_SPACE_OPERATIONS = 72,
        LF_CAT_LAND_SPECIAL_STATUS = 73,
        LF_CAT_LAND_RESERVED_74 = 74,
        LF_CAT_LAND_RESERVED_75 = 75,
        LF_CAT_LAND_RESERVED_76 = 76,
        LF_CAT_LAND_RESERVED_77 = 77,
        LF_CAT_LAND_RESERVED_78 = 78,
        LF_CAT_LAND_RESERVED_79 = 79,
        LF_CAT_LAND_ALLIGATOR_CAPTIVE = 80,
        LF_CAT_LAND_ALPACA = 81,
        LF_CAT_LAND_BUFFALO = 82,
        LF_CAT_LAND_CAMEL = 83,
        LF_CAT_LAND_CARIBOU = 84,
        LF_CAT_LAND_CAT = 85,
        LF_CAT_LAND_CATTLE = 86,
        LF_CAT_LAND_CHICKEN = 87,
        LF_CAT_LAND_CHINCHILLA_CAPTIVE_BRED = 88,
        LF_CAT_LAND_CRICKET_CAPT_BRED = 89,
        LF_CAT_LAND_DEER = 90,
        LF_CAT_LAND_DOG = 91,
        LF_CAT_LAND_DONKEY = 92,
        LF_CAT_LAND_DUCK = 93,
        LF_CAT_LAND_ELEPHANT_CAPTIVE_BRED = 94,
        LF_CAT_LAND_ERMINE_CAPTIVE_BRED = 95,
        LF_CAT_LAND_FERRET = 96,
        LF_CAT_LAND_FOX_CAPTIVE_BRED = 97,
        LF_CAT_LAND_GEESE = 98,
        LF_CAT_LAND_GERBIL = 99,
        LF_CAT_LAND_GOAT = 100,
        LF_CAT_LAND_GUINEA_PIG = 101,
        LF_CAT_LAND_HAMSTER = 102,
        LF_CAT_LAND_HORSE = 103,
        LF_CAT_LAND_KANGAROO = 104,
        LF_CAT_LAND_LLAMA = 105,
        LF_CAT_LAND_MINK_CAPTIVE_BRED = 106,
        LF_CAT_LAND_MOUSE = 107,
        LF_CAT_LAND_NIGHTCRAWLER = 108,
        LF_CAT_LAND_OSTRICH_CAPTIVE_BRED = 109,
        LF_CAT_LAND_OTTER_CAPTIVE_BRED = 110,
        LF_CAT_LAND_OXEN = 111,
        LF_CAT_LAND_PIG = 112,
        LF_CAT_LAND_PIGEON = 113,
        LF_CAT_LAND_POSSUM = 114,
        LF_CAT_LAND_PRIMATE = 115,
        LF_CAT_LAND_RABBIT = 116,
        LF_CAT_LAND_RAT = 117,
        LF_CAT_LAND_SHEEP = 118,
        LF_CAT_LAND_SILKWORM = 119,
        LF_CAT_LAND_SNAIL = 120,
        LF_CAT_LAND_SWAN_CAPTIVE_BRED = 121,
        LF_CAT_LAND_TURKEYS = 122,
        LF_CAT_LAND_WORM_CAPTIVE_BRED = 123,
        LF_CAT_LAND_YAK = 124,
        LF_CAT_LAND_RESERVED_125 = 125,
        LF_CAT_LAND_RESERVED_126 = 126,
        LF_CAT_LAND_RESERVED_127 = 127,
        LF_CAT_LAND_RESERVED_128 = 128,
        LF_CAT_LAND_RESERVED_129 = 129,
        LF_CAT_LAND_RESERVED_130 = 130,
        LF_CAT_LAND_RESERVED_131 = 131,
        LF_CAT_LAND_RESERVED_132 = 132,
        LF_CAT_LAND_RESERVED_133 = 133,
        LF_CAT_LAND_RESERVED_134 = 134,
        LF_CAT_LAND_RESERVED_135 = 135,
        LF_CAT_LAND_RESERVED_136 = 136,
        LF_CAT_LAND_RESERVED_137 = 137,
        LF_CAT_LAND_RESERVED_138 = 138,
        LF_CAT_LAND_RESERVED_139 = 139,
        LF_CAT_LAND_GENERAL_DUPLICATE_140 = 140,
        LF_CAT_LAND_AARDVARKS = 141,
        LF_CAT_LAND_ARMADILLOS_AND_RELATED = 142,
        LF_CAT_LAND_BANDICOOTS_AND_RELATED = 143,
        LF_CAT_LAND_ELEPHANTS_AND_RELATED = 144,
        LF_CAT_LAND_EVEN_TOED_UNGULATES = 145,
        LF_CAT_LAND_HYRAXES = 146,
        LF_CAT_LAND_INSECT_EATERS = 147,
        LF_CAT_LAND_RABBITS_AND_RELATED = 148,
        LF_CAT_LAND_KANGAROOS_AND_RELATED = 149,
        LF_CAT_LAND_MEAT_EATERS = 150,
        LF_CAT_LAND_MOLES = 151,
        LF_CAT_LAND_ODD_TOED_UNGULATES = 152,
        LF_CAT_LAND_OPOSSUMS = 153,
        LF_CAT_LAND_PANGOLINS_AND_RELATED = 154,
        LF_CAT_LAND_PLATYPUS = 155,
        LF_CAT_LAND_PRIMATES = 156,
        LF_CAT_LAND_RODENTS = 157,
        LF_CAT_LAND_SPINY_ANTEATERS = 158,
        LF_CAT_LAND_RESERVED_159 = 159,
        LF_CAT_LAND_RESERVED_160 = 160,
        LF_CAT_LAND_RESERVED_161 = 161,
        LF_CAT_LAND_RESERVED_162 = 162,
        LF_CAT_LAND_RESERVED_163 = 163,
        LF_CAT_LAND_RESERVED_164 = 164,
        LF_CAT_LAND_RESERVED_165 = 165,
        LF_CAT_LAND_OTHER = 166,
        LF_CAT_LAND_PENGUINS = 167,
        LF_CAT_LAND_OSTRICHES = 168,
        LF_CAT_LAND_RESERVED_169 = 169,
        LF_CAT_LAND_SNAKES = 170,
        LF_CAT_LAND_ALLIGATORS_CROCODILES = 171,
        LF_CAT_LAND_TUATARAS = 172,
        LF_CAT_LAND_LIZARDS = 173,
        LF_CAT_LAND_WORM_LIZARDS = 174,
        LF_CAT_LAND_TURTLES = 175,
        LF_CAT_LAND_RESERVED_176 = 176,
        LF_CAT_LAND_RESERVED_177 = 177,
        LF_CAT_LAND_RESERVED_178 = 178,
        LF_CAT_LAND_RESERVED_179 = 179,
        LF_CAT_LAND_GENERAL_DUPLICATE_180 = 180,
        LF_CAT_LAND_BEETLES = 181,
        LF_CAT_LAND_ANTS = 182,
        LF_CAT_LAND_TRUE_BUGS = 183,
        LF_CAT_LAND_GRASSHOPPER = 184,
        LF_CAT_LAND_CRICKETS = 185,
        LF_CAT_LAND_COCKROACHES = 186,
        LF_CAT_LAND_MANTIDS = 187,
        LF_CAT_LAND_CICADAS_APHIDS = 188,
        LF_CAT_LAND_SCALE_INSECTS = 189,
        LF_CAT_LAND_RESERVED_190 = 190,
        LF_CAT_LAND_RESERVED_191 = 191,
        LF_CAT_LAND_RESERVED_192 = 192,
        LF_CAT_LAND_GENERAL_DUPLICATE_193 = 193,
        LF_CAT_LAND_RESERVED_194 = 194,
        LF_CAT_LAND_RESERVED_195 = 195,
        LF_CAT_LAND_SPIDERS_AND_RELATED = 196,
        LF_CAT_LAND_RESERVED_197 = 197,
        LF_CAT_LAND_RESERVED_198 = 198,
        LF_CAT_LAND_RESERVED_199 = 199,
        LF_CAT_LAND_GRAINS = 200,
        LF_CAT_LAND_FRUIT_TREES = 201,
        LF_CAT_LAND_VEGETABLES = 202,
        LF_CAT_LAND_FLOWERS = 203,
        LF_CAT_LAND_RESERVED_204 = 204,
        LF_CAT_LAND_RESERVED_205 = 205,
        LF_CAT_LAND_RESERVED_206 = 206,
        LF_CAT_LAND_RESERVED_207 = 207,
        LF_CAT_LAND_RESERVED_208 = 208,
        LF_CAT_LAND_RESERVED_209 = 209,
        LF_CAT_LAND_RESERVED_210 = 210,
        LF_CAT_LAND_RESERVED_211 = 211,
        LF_CAT_LAND_RESERVED_212 = 212,
        LF_CAT_LAND_RESERVED_213 = 213,
        LF_CAT_LAND_RESERVED_214 = 214,
        LF_CAT_LAND_RESERVED_215 = 215,
        LF_CAT_LAND_RESERVED_216 = 216,
        LF_CAT_LAND_RESERVED_217 = 217,
        LF_CAT_LAND_RESERVED_218 = 218,
        LF_CAT_LAND_RESERVED_219 = 219,
        LF_CAT_LAND_RESERVED_220 = 220,
        LF_CAT_LAND_RESERVED_221 = 221,
        LF_CAT_LAND_RESERVED_222 = 222,
        LF_CAT_LAND_RESERVED_223 = 223,
        LF_CAT_LAND_RESERVED_224 = 224,
        LF_CAT_LAND_RESERVED_225 = 225,
        LF_CAT_LAND_RESERVED_226 = 226,
        LF_CAT_LAND_RESERVED_227 = 227,
        LF_CAT_LAND_RESERVED_228 = 228,
        LF_CAT_LAND_RESERVED_229 = 229,
        LF_CAT_LAND_SHRUBS_BUSHES = 230,
        LF_CAT_LAND_WILD_FLOWERS = 231,
        LF_CAT_LAND_WILD_GRASSES = 232,
        LF_CAT_LAND_MOSSES = 233,
        LF_CAT_LAND_FERNS = 234,
        LF_CAT_LAND_LICHENS = 235,
        LF_CAT_LAND_OTHER_FLOWERING_DICOT_PLANTS = 236,
        LF_CAT_LAND_OTHER_FLOWERING_MONOCOT_PLANTS = 237,
        LF_CAT_LAND_RESERVED_238 = 238,
        LF_CAT_LAND_RESERVED_239 = 239,
        LF_CAT_LAND_DICOTS_BROADLEAF_HARDWOODS = 240,
        LF_CAT_LAND_MONOCOTS = 241,
        LF_CAT_LAND_CONIFERS_SOFTWOODS = 242,
        LF_CAT_LAND_GINKGOS = 243,
        LF_CAT_LAND_CYCADS = 244,
        LF_CAT_LAND_TREE_FERNS = 245,
        LF_CAT_LAND_RESERVED_246 = 246,
        LF_CAT_LAND_RESERVED_247 = 247,
        LF_CAT_LAND_RESERVED_248 = 248,
        LF_CAT_LAND_RESERVED_249 = 249,
        LF_CAT_LAND_OTHER_DUPLICATE_250 = 250,
        LF_CAT_LAND_BACTERIA = 251,
        LF_CAT_LAND_VIRUSES = 252,
        LF_CAT_LAND_FUNGI = 253,
        LF_CAT_LAND_ALGAE = 254,
        LF_CAT_LAND_RESERVED_255 = 255,
        LF_CAT_LAND_END
    } lf_cat_land_e;
}

#endif
