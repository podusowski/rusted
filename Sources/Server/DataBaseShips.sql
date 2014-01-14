/*                         id   type        class  model             x     y     z      owner integrity carbon helium */
INSERT OR REPLACE INTO objects VALUES(1,   "Ship",     1,     NULL,             1,    2000, 1,     1,    100,      0,     0);
INSERT OR REPLACE INTO objects VALUES(2,   "Ship",     2,     NULL,             1,    4000, 1,     1,    100,      0,     0);
INSERT OR REPLACE INTO objects VALUES(3,   "Ship",     3,     NULL,             1,    5000, 1,     1,    100,      0,     0);
INSERT OR REPLACE INTO objects VALUES(4,   "Ship",     4,     NULL,             1,    6000, 1,     1,    100,      0,     0);
INSERT OR REPLACE INTO objects VALUES(5,   "Ship",     5,     NULL,             1,    7000, 1,     1,    100,      0,     0);

INSERT OR REPLACE INTO objects VALUES(11,  "Ship",     1,     NULL,             1,    2000, 10000, 2,    100,      0,     0);
INSERT OR REPLACE INTO objects VALUES(12,  "Ship",     2,     NULL,             1,    4000, 10000, 2,    100,      0,     0);
INSERT OR REPLACE INTO objects VALUES(13,  "Ship",     3,     NULL,             1,    5000, 10000, 2,    100,      0,     0);
INSERT OR REPLACE INTO objects VALUES(14,  "Ship",     4,     NULL,             1,    6000, 10000, 2,    100,      0,     0);
INSERT OR REPLACE INTO objects VALUES(15,  "Ship",     5,     NULL,             1,    7000, 10000, 2,    100,      0,     0);

INSERT OR REPLACE INTO objects VALUES(100, "Asteroid", NULL,  "Asteroid.model", 2000, 1,    1,     NULL, 100,      1000,  1000);
INSERT OR REPLACE INTO objects VALUES(101, "Asteroid", NULL,  "Asteroid.model", 2000, 1000, 1,     NULL, 100,      1000,  1000);

/*     OR REPLACE                          id   speed   max_integrity  name             model                 max_capacity  required_carbon  required_helium */
INSERT OR REPLACE INTO ship_classes VALUES(1,   500,    1000,          "Figter",        "Fighter.model",      100,          10,              5);
INSERT OR REPLACE INTO ship_classes VALUES(5,   5000,   100,           "Carnwennan",    "Carnwennan.model",   100,          10,              5);
INSERT OR REPLACE INTO ship_classes VALUES(4,   80,     10000,         "Fregate",       "Fregate.model",      1000,         1000,            50);
INSERT OR REPLACE INTO ship_classes VALUES(2,   100,    1000,          "Constructor",   "Constructor.model",  10000,        1000,            50);
INSERT OR REPLACE INTO ship_classes VALUES(3,   100,    1000,          "Gatherer",      "Gatherer.model",     1000,         100,             50);

/*     OR REPLACE                                  id  ship_class_id  action_id  action_parameter */
/* FighOR REPLACE ter */
INSERT OR REPLACE INTO ship_classes_actions VALUES(1,  1,             1,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(2,  1,             1,         2);
INSERT OR REPLACE INTO ship_classes_actions VALUES(3,  1,             4,         1);

/* CarnOR REPLACE wennan */
INSERT OR REPLACE INTO ship_classes_actions VALUES(50, 5,             1,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(51, 5,             4,         1);

/* FregOR REPLACE ate */
INSERT OR REPLACE INTO ship_classes_actions VALUES(40, 4,             1,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(41, 4,             4,         1);

/* ConsOR REPLACE tructor */
INSERT OR REPLACE INTO ship_classes_actions VALUES(20, 2,             2,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(21, 2,             2,         2);
INSERT OR REPLACE INTO ship_classes_actions VALUES(22, 2,             2,         3);
INSERT OR REPLACE INTO ship_classes_actions VALUES(23, 2,             4,         1);

/* GathOR REPLACE erer */
INSERT OR REPLACE INTO ship_classes_actions VALUES(30, 3,             3,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(31, 3,             4,         1);

