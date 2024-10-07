#ifndef IR_DATA_H
#define IR_DATA_H
#pragma once

// IR Test Data

// Proteus
std::vector<float> ir_data1 = {
    0.09165135,    0.34494776,    0.642427,      0.8733099,     0.9765655,
    0.90381545,    0.64580977,    0.26979384,    -0.09133818,   -0.33001012,
    -0.38087615,   -0.27518257,   -0.09180161,   0.07009281,    0.13477188,
    0.1134176,     0.05394234,    0.0002800684,  -0.01746423,   0.0023448383,
    0.037010457,   0.07013612,    0.08224031,    0.0646829,     0.026357336,
    -0.0038456772, -0.00917907,   0.010505134,   0.037371267,   0.03840451,
    0.018965935,   -0.020979231,  -0.0719064,    -0.123280674,  -0.17116594,
    -0.19054614,   -0.18061672,   -0.14881288,   -0.102494515,  -0.04864169,
    0.0048738876,  0.04574761,    0.069491655,   0.073702306,   0.06250842,
    0.037841693,   -0.0013760217, -0.0457628,    -0.09093866,   -0.13268682,
    -0.1635205,    -0.18142268,   -0.18144807,   -0.16410044,   -0.13526863,
    -0.10523564,   -0.08354254,   -0.07703036,   -0.07966023,   -0.08314092,
    -0.08711798,   -0.082402855,  -0.07069823,   -0.060387973,  -0.05511471,
    -0.05297593,   -0.05506546,   -0.061078455,  -0.064046904,  -0.06443261,
    -0.06321003,   -0.064143464,  -0.06725661,   -0.06987344,   -0.071799636,
    -0.0710794,    -0.06976334,   -0.06853695,   -0.06835803,   -0.0698773,
    -0.06964096,   -0.0669661,    -0.06253795,   -0.060600787,  -0.062732615,
    -0.06995352,   -0.081729166,  -0.09046745,   -0.09488154,   -0.094281204,
    -0.089662544,  -0.08452549,   -0.083357014,  -0.09053672,   -0.10228267,
    -0.112124674,  -0.11511527,   -0.109826796,  -0.10042146,   -0.08960831,
    -0.079272754,  -0.076487735,  -0.08254217,   -0.094931655,  -0.107865356,
    -0.11363837,   -0.11187402,   -0.10532005,   -0.095720805,  -0.08345185,
    -0.071409926,  -0.060202427,  -0.04782326,   -0.03344093,   -0.01948797,
    -0.009585296,  -0.0065911557, -0.011831561,  -0.022803213,  -0.037139785,
    -0.049482487,  -0.056022998,  -0.056035332,  -0.051204406,  -0.044367153,
    -0.037329253,  -0.031810008,  -0.025931384,  -0.01798113,   -0.0058025466,
    0.008753816,   0.0212772,     0.029299777,   0.031279836,   0.027175553,
    0.016919078,   0.0028551049,  -0.011740603,  -0.024240287,  -0.03401027,
    -0.04003737,   -0.04131747,   -0.03749122,   -0.02958256,   -0.021509392,
    -0.015716761,  -0.0129135195, -0.011967915,  -0.011235863,  -0.010230864,
    -0.008329283,  -0.00408161,   0.0018289342,  0.0070602293,  0.010045952,
    0.010475637,   0.0092166895,  0.007399299,   0.005249437,   0.0019942587,
    -0.0023101277, -0.0072071664, -0.012060744,  -0.016935531,  -0.021020649,
    -0.022889642,  -0.021009525,  -0.014375131,  -0.0047140247, 0.005474857,
    0.014507624,   0.021367949,   0.023821149,   0.020447709,   0.012130102,
    0.0024319293,  -0.004913604,  -0.008870569,  -0.009489351,  -0.007629974,
    -0.0037156516, 0.001060485,   0.005665943,   0.010041704,   0.014878822,
    0.02112296,    0.0273475,     0.031416226,   0.032417998,   0.032098353,
    0.03296269,    0.035877626,   0.040683776,   0.046800036,   0.053564813,
    0.05914606,    0.061286274,   0.05919661,    0.054442685,   0.049703162,
    0.04636829,    0.04396994,    0.041776415,   0.039922163,   0.038467713,
    0.03657376,    0.033497334,   0.030306501,   0.028770005,   0.029629637,
    0.03229057,    0.03524298,    0.03729755,    0.03793816,    0.03712821,
    0.034826882,   0.03126039,    0.0279462,     0.026440628,   0.026734091,
    0.02762649,    0.028561203,   0.030229844,   0.03302887,    0.036502216,
    0.039408617,   0.041369576,   0.04343628,    0.046191294,   0.04928704,
    0.051745165,   0.053291935,   0.054384712,   0.05455833,    0.052571084,
    0.04855015,    0.044583116,   0.042659927,   0.043115146,   0.04414983,
    0.043795798,   0.04119478,    0.03585369,    0.027858194,   0.018152118,
    0.008916754,   0.0027295686,  0.00074899243, 0.0019292365,  0.0043796874,
    0.0071724323,  0.009773309,   0.011557546,   0.012009226,   0.01110372,
    0.009688959,   0.008699506,   0.008329629,   0.007940848,   0.007272189,
    0.00680828,    0.0067435172,  0.0068604015,  0.0069762403,  0.007867849,
    0.010683702,   0.015545136,   0.021463517,   0.027169043,   0.032217573,
    0.036653917,   0.040313497,   0.042723008,   0.043977235,   0.0449698,
    0.046105023,   0.04706817,    0.047397412,   0.047204096,   0.04696398,
    0.046654396,   0.04556595,    0.043137535,   0.03988467,    0.036823932,
    0.03446145,    0.032549195,   0.030769397,   0.029347012,   0.028637875,
    0.028364282,   0.02800607,    0.027471108,   0.026964674,   0.026620567,
    0.025839966,   0.023894513,   0.020846987,   0.017366048,   0.014165705,
    0.011368496,   0.009008979,   0.007559766,   0.007517336,   0.008661434,
    0.010120896,   0.011428849,   0.012626815,   0.013845919,   0.0146744205,
    0.014336534,   0.012651985,   0.01008577,    0.0071581784,  0.0039129085,
    0.0005635425,  -0.0021761484, -0.0036216264, -0.0036629362, -0.0027807339,
    -0.001482819,  7.563153e-05,  0.002103992,   0.004418948,   0.0064223562,
    0.007866643,   0.008928606,   0.00980413,    0.0104318755,  0.0105820345,
    0.01050031,    0.010787014,   0.011676834,   0.012972123,   0.014149356,
    0.014627745,   0.014052944,   0.012501514,   0.010191595,   0.007678722,
    0.0057652965,  0.0047764783,  0.0043149022,  0.0033456723,  0.0012752704,
    -0.0015440945, -0.004404262,  -0.0069624637, -0.009157596,  -0.010737355,
    -0.011538574,  -0.011737263,  -0.011770245,  -0.011839801,  -0.011672521,
    -0.010874318,  -0.009469784,  -0.008051697,  -0.0070956284, -0.006680956,
    -0.0067819455, -0.0072662174, -0.007840403,  -0.007903105,  -0.0069280844,
    -0.005052687,  -0.0027962897, -0.0006266202, 0.0013131618,  0.0032626598,
    0.0055451146,  0.008049342,   0.010387563,   0.012228167,   0.013016348,
    0.012363326,   0.010588523,   0.008504281,   0.006856431,   0.0058220886,
    0.0047860597,  0.002814133,   -0.0005990605, -0.005205054,  -0.010060786,
    -0.014092206,  -0.016563775,  -0.017255284,  -0.016593799,  -0.0155841755,
    -0.015348273,  -0.016462123,  -0.018690843,  -0.021405507,  -0.024050314,
    -0.026340691,  -0.028283231,  -0.030039148,  -0.031766366,  -0.03344352};

// US Deluxe
std::vector<float> ir_data2 = {
    0.0034908056,   0.3523475,     0.60503685,    0.95999277,    0.9286411,
    0.9942601,      0.5302459,     0.29374087,    -0.16792,      -0.1814208,
    -0.3856635,     -0.28140163,   -0.24453771,   -0.03430164,   -0.014578223,
    0.028817415,    0.053222418,   0.05444622,    0.016842604,   0.032137156,
    0.066504955,    0.082255125,   0.04495418,    0.086707234,   0.08999586,
    0.07729125,     0.008366823,   0.006036639,   -0.07392311,   -0.10841155,
    -0.15197158,    -0.12579143,   -0.13457584,   -0.1239537,    -0.14833474,
    -0.11523831,    -0.13403797,   -0.12985802,   -0.10929978,   -0.044311166,
    -0.01559937,    0.02953422,    0.060052276,   0.0918895,     0.0705477,
    0.042483687,    0.0050832033,  -0.011725068,  -0.07401705,   -0.112733245,
    -0.1482836,     -0.13304639,   -0.12432706,   -0.06644559,   -0.03312707,
    -0.007929921,   -0.030416131,  -0.025596976,  -0.038918734,  -0.026919365,
    -0.026245475,   0.020679593,   0.041544914,   0.06317687,    0.042120457,
    0.031234264,    -0.016168594,  -0.058145642,  -0.09446871,   -0.07810295,
    -0.06428921,    -0.033769846,  0.0010832548,  0.04238522,    0.042004704,
    0.03635466,     0.015765786,   0.009633303,   -0.011256218,  -0.012475491,
    -0.023204088,   -0.029212594,  -0.0601542,    -0.07464349,   -0.09852147,
    -0.11526668,    -0.135095,     -0.12136924,   -0.10606623,   -0.07955718,
    -0.05698359,    -0.02991879,   -0.029208183,  -0.032895803,  -0.051694274,
    -0.060173273,   -0.06856835,   -0.060687542,  -0.05111146,   -0.03369987,
    -0.03733623,    -0.04555881,   -0.055636406,  -0.05298114,   -0.055659056,
    -0.044862628,   -0.028801799,  -0.0017154217, 0.010845423,   0.01713121,
    0.004822731,    -0.011127949,  -0.04259026,   -0.060756564,  -0.07517874,
    -0.07852352,    -0.077587485,  -0.053107142,  -0.025283217,  0.0030859709,
    0.013730526,    0.021886468,   0.016690612,   -0.0032480955, -0.039693832,
    -0.06219518,    -0.07916355,   -0.084699154,  -0.08225083,   -0.05849719,
    -0.036077976,   -0.020155191,  -0.021954179,  -0.025768995,  -0.039601088,
    -0.0559026,     -0.06732476,   -0.05816984,   -0.045262575,  -0.032297134,
    -0.021949053,   -0.010449767,  -0.009745598,  -0.019760609,  -0.032734036,
    -0.03770554,    -0.039441228,  -0.03492737,   -0.026156187,  -0.018304586,
    -0.021251917,   -0.030637383,  -0.04135263,   -0.053743124,  -0.06294179,
    -0.060462594,   -0.046948075,  -0.034631252,  -0.02174604,   -0.012224555,
    -0.009338856,   -0.01651454,   -0.02762568,   -0.039643884,  -0.04897487,
    -0.05612707,    -0.058932662,  -0.052454114,  -0.043878555,  -0.036735654,
    -0.03259313,    -0.027591348,  -0.02599144,   -0.024257898,  -0.025446773,
    -0.025572896,   -0.028790236,  -0.030592084,  -0.036227107,  -0.039076805,
    -0.04072857,    -0.03416097,   -0.026471734,  -0.016138554,  -0.007434845,
    0.0017493963,   0.0042752028,  0.0010277033,  -0.005280018,  -0.008223057,
    -0.0075218678,  -0.0033237934, 0.0017014742,  0.0044099092,  0.0023477077,
    -0.006031275,   -0.014598489,  -0.022361279,  -0.029232025,  -0.02923143,
    -0.02336955,    -0.01599729,   -0.009979725,  -0.0057735443, -0.005424261,
    -0.006463766,   -0.012001753,  -0.015451312,  -0.01901412,   -0.020989299,
    -0.023274183,   -0.020198703,  -0.02039206,   -0.021266699,  -0.022531867,
    -0.022469044,   -0.025269508,  -0.026475906,  -0.025243282,  -0.01808834,
    -0.009191394,   -0.0011498928, 0.007754922,   0.01632464,    0.017757535,
    0.013885379,    0.0072869062,  -0.0016311407, -0.012192249,  -0.018782139,
    -0.01937139,    -0.016841888,  -0.013502955,  -0.008062124,  -0.00091791153,
    0.0037784576,   0.004133582,   0.0032060146,  0.0020331144,  6.3061714e-05,
    -0.00023555756, 0.0015386343,  0.0065078735,  0.010544658,   0.014102578,
    0.014136553,    0.015601397,   0.013221383,   0.010071993,   0.0070796013,
    0.010366678,    0.010910869,   0.01474309,    0.016852975,   0.018993855,
    0.016916752,    0.014052153,   0.010658741,   0.01047492,    0.011841774,
    0.015697837,    0.02161026,    0.025615811,   0.028541923,   0.028490663,
    0.02742374,     0.023717642,   0.022655606,   0.022224545,   0.024525046,
    0.02707851,     0.02882719,    0.02816856,    0.02576375,    0.022264719,
    0.018282652,    0.016137838,   0.012643456,   0.011794448,   0.010300636,
    0.011246085,    0.010383606,   0.010789752,   0.008945465,   0.009842515,
    0.0076446533,   0.008565426,   0.007864594,   0.009451747,   0.008964777,
    0.011538744,    0.013460636,   0.017579794,   0.02121067,    0.025349021,
    0.028649092,    0.030283332,   0.031076193,   0.03160155,    0.032464147,
    0.03135979,     0.03288138,    0.033884287,   0.036432505,   0.036322355,
    0.037168264,    0.03582096,    0.03550613,    0.03268552,    0.031231046,
    0.028749824,    0.028330803,   0.026944041,   0.027256966,   0.02791953,
    0.029008389,    0.028328419,   0.028022885,   0.026829123,   0.026013374,
    0.024523973,    0.024266481,   0.024891257,   0.02662301,    0.028439045,
    0.030757904,    0.032938838,   0.034246802,   0.033588648,   0.03208649,
    0.030010939,    0.027166009,   0.022800207,   0.01933384,    0.016840696,
    0.015998721,    0.016658425,   0.018427849,   0.020996451,   0.023503304,
    0.02570641,     0.02746272,    0.029088974,   0.030838728,   0.031552553,
    0.033169985,    0.033848047,   0.035838366,   0.034903765,   0.035102725,
    0.033385754,    0.03158772,    0.027222276,   0.024120927,   0.02064538,
    0.018165112,    0.016584635,   0.01677835,    0.019063115,   0.022302628,
    0.024907589,    0.02802074,    0.029728651,   0.030268312,   0.028403282,
    0.02735734,     0.02481842,    0.022879362,   0.020369172,   0.019721866,
    0.018938303,    0.018055558,   0.017082095,   0.016770005,   0.016198158,
    0.015114546,    0.014338493,   0.015130281,   0.015685916,   0.017270923,
    0.019113183,    0.02241993,    0.023706555,   0.025639057,   0.025776744,
    0.026248455,    0.024643064,   0.024245858,   0.02239716,    0.022108555,
    0.020202756,    0.01940012,    0.016961098,   0.016058564,   0.014431,
    0.015957952,    0.017501473,   0.02040577,    0.023226023,   0.025257349};

// Vox Bright
std::vector<float> ir_data3 = {
    0.52926904,     0.9913671,      0.7762714,      0.30122554,
    0.02760484,     -0.09334413,    -0.17949381,    -0.28187993,
    -0.40910017,    -0.47256044,    -0.37820905,    -0.050660703,
    0.25327346,     0.28935027,     0.27218527,     0.2686282,
    0.15352686,     0.07436823,     0.086013064,    0.03662069,
    -0.03765196,    -0.025271958,   0.055809543,    0.08330272,
    0.005136005,    -0.047806144,   -0.046810385,   -0.09686475,
    -0.122591466,   -0.038212035,   0.082973816,    0.10409813,
    -0.03086842,    -0.15451741,    -0.08823838,    0.057823457,
    0.10582014,     0.104023054,    0.123351686,    0.11348963,
    0.05198277,     0.012219376,    0.0530658,      0.10188548,
    0.06831236,     0.02357167,     0.003458026,    -0.03002572,
    -0.06139074,    -0.07732701,    -0.08320034,    -0.07976689,
    -0.05513872,    -0.027620982,   5.9221995e-05,  0.03597262,
    0.10206238,     0.1552416,      0.119939454,    0.052263536,
    0.04553789,     0.047794525,    -0.024861239,   -0.08589686,
    -0.06894487,    -0.049504116,   -0.08097945,    -0.091603704,
    -0.050114006,   -0.008138964,   0.011634001,    0.02815759,
    0.046459682,    0.04021363,     -0.0025765595,  -0.05292374,
    -0.063403666,   -0.051900905,   -0.04517607,    -0.040179342,
    -0.01687591,    0.0025827899,   0.0015933553,   -0.0077868304,
    -0.011693262,   0.0016688746,   0.013408942,    0.026192946,
    0.043882515,    0.05782675,     0.046051428,    0.015057223,
    -0.033069357,   -0.087710895,   -0.12391825,    -0.12685457,
    -0.107922286,   -0.08932046,    -0.06736319,    -0.04245921,
    -0.01376761,    0.003013659,    0.0090396525,   0.00580002,
    0.0088967895,   0.012176508,    0.009094895,    -0.00056891335,
    -0.025668317,   -0.055932987,   -0.077429086,   -0.07214683,
    -0.053591814,   -0.036590207,   -0.035276435,   -0.037301477,
    -0.039745424,   -0.03694031,    -0.026427237,   -0.012534732,
    -0.0009651981,  0.0033178604,   0.008918116,    0.016139258,
    0.028735144,    0.025887907,    0.011255307,    -0.009296355,
    -0.029485876,   -0.052700993,   -0.070198014,   -0.07382528,
    -0.06756471,    -0.057344,      -0.04913982,    -0.035966925,
    -0.027394315,   -0.01616257,    -0.0055658454,  0.008349506,
    0.013733038,    0.010655799,    0.006627273,    0.0061754375,
    0.005410965,    -0.001606019,   -0.009299938,   -0.021861441,
    -0.033020556,   -0.045208976,   -0.04828105,    -0.04975661,
    -0.04553784,    -0.038557436,   -0.029994765,   -0.023018982,
    -0.017157892,   -0.0073536127,  0.0016858559,   0.0122239655,
    0.01814008,     0.023726355,    0.018521804,    0.009714624,
    -0.0039998363,  -0.014515867,   -0.021151088,   -0.02523976,
    -0.028338034,   -0.03162936,    -0.031167278,   -0.032228053,
    -0.0294936,     -0.027232802,   -0.017944131,   -0.009939017,
    -0.00018279706, 0.0051716785,   0.0065148743,   0.0022374608,
    -0.0036576446,  -0.0067219594,  -0.011335223,   -0.014490279,
    -0.020637117,   -0.02480784,    -0.0368385,     -0.048281204,
    -0.057237,      -0.05606737,    -0.0513736,     -0.04256058,
    -0.029577868,   -0.016967624,   -0.0071672793,  -0.0047189044,
    -0.0011060799,  -0.0015702252,  -0.00068193534, -0.0024546364,
    0.00021605985,  -0.00046574697, -0.0032484876,  -0.010052465,
    -0.01644502,    -0.01847845,    -0.019027654,   -0.017289212,
    -0.01860751,    -0.01616141,    -0.016271086,   -0.013475646,
    -0.014034381,   -0.0109506985,  -0.0064197057,  0.0011757357,
    0.0078107705,   0.012406932,    0.015948093,    0.015341446,
    0.014860423,    0.007814397,    0.0009218245,   -0.009413779,
    -0.015673634,   -0.02428315,    -0.029007021,   -0.030489093,
    -0.026972514,   -0.021621373,   -0.016382419,   -0.0071591455,
    0.0009678352,   0.01176981,     0.018807078,    0.026909633,
    0.028422244,    0.028758021,    0.021824915,    0.013639684,
    0.0051578274,   -0.0022255362,  -0.006938266,   -0.009707628,
    -0.0066682836,  -0.0050262185,  -0.00085137656, 0.0010077503,
    0.008035068,    0.011132667,    0.014174533,    0.014764709,
    0.015495033,    0.012719825,    0.0067539196,   0.0027567134,
    -0.0005644386,  0.0014059843,   0.0023458877,   0.0071812924,
    0.0077453526,   0.00999803,     0.008477202,    0.008506544,
    0.007944122,    0.007850256,    0.00646486,     0.002716932,
    0.0011136974,   -0.0022061665,  -0.002739972,   -0.005793608,
    -0.0036937797,  -0.0038322166,  -0.00059866195, 0.0009850085,
    0.0033178101,   0.003510766,    0.0033602053,   0.004634685,
    0.0039288765,   0.004229767,    0.0013694005,   0.0022825706,
    -5.4751865e-05, 0.0015666845,   0.0029697455,   0.008427301,
    0.011243964,    0.01405468,     0.016323682,    0.016948657,
    0.017633554,    0.016392874,    0.018759776,    0.017851664,
    0.019516293,    0.01815088,     0.02097178,     0.021799464,
    0.024721343,    0.026564552,    0.028416224,    0.030289982,
    0.030292954,    0.02969104,     0.02375046,     0.019392343,
    0.011663934,    0.008559333,    0.005651712,    0.00911754,
    0.012044573,    0.01663087,     0.020363517,    0.024040546,
    0.028231975,    0.029300109,    0.031336002,    0.029723218,
    0.030829672,    0.027378455,    0.025361167,    0.019936353,
    0.017550642,    0.014200253,    0.012465927,    0.01186325,
    0.012952039,    0.01736744,     0.020043042,    0.024836503,
    0.025172714,    0.028083289,    0.02714002,     0.02801101,
    0.025466144,    0.024951627,    0.02380085,     0.022702223,
    0.020738177,    0.016801108,    0.01485368,     0.010576159,
    0.010504789,    0.009383841,    0.013670317,    0.015526365,
    0.019829392,    0.021967396,    0.0251931,      0.026227878,
    0.025081003,    0.024055451,    0.021428837,    0.020767787,
    0.016187211,    0.014446774,    0.009726275,    0.008847436,
    0.0059528626,   0.0067875218,   0.007500149,    0.009873512,
    0.012122091,    0.013608628,    0.016368914,    0.016112251,
    0.018498972,    0.018424312,    0.022197433,    0.02220375,
    0.024142576,    0.02267084,     0.022745766,    0.02174164,
    0.020829141,    0.020211931,    0.018440062,    0.019169701,
    0.017672582,    0.01892792,     0.016710838,    0.018604191,
    0.018346699,    0.021145618,    0.02162157,     0.023625748,
    0.025533015,    0.026963178,    0.028700838,    0.027634833,
    0.028351722,    0.02585701,     0.026522428,    0.023505192,
    0.023094479,    0.019726042,    0.019039115,    0.017455809};

std::vector<std::vector<float>> ir_collection = {ir_data1, ir_data2, ir_data3};

#endif