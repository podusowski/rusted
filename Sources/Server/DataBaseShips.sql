/*                                         id   speed   max_integrity  name             model                 max_capacity  required_carbon  required_helium */
INSERT OR REPLACE INTO ship_classes VALUES(1,   500,    1000,          "Figter",        "Fighter.model",      100,          10,              5);
INSERT OR REPLACE INTO ship_classes VALUES(5,   5000,   100,           "Carnwennan",    "Carnwennan.model",   100,          10,              5);
INSERT OR REPLACE INTO ship_classes VALUES(4,   80,     10000,         "Fregate",       "Fregate.model",      1000,         1000,            50);
INSERT OR REPLACE INTO ship_classes VALUES(2,   100,    1000,          "Constructor",   "Constructor.model",  10000,        1000,            50);
INSERT OR REPLACE INTO ship_classes VALUES(3,   100,    1000,          "Gatherer",      "Gatherer.model",     1000,         100,             50);

/*                                                 id  ship_class_id  action_id  action_parameter */
/* Fighter */
INSERT OR REPLACE INTO ship_classes_actions VALUES(1,  1,             1,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(2,  1,             1,         2);
INSERT OR REPLACE INTO ship_classes_actions VALUES(3,  1,             4,         1);

/* Carnwennan */
INSERT OR REPLACE INTO ship_classes_actions VALUES(50, 5,             1,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(51, 5,             4,         1);

/* Fregate */
INSERT OR REPLACE INTO ship_classes_actions VALUES(40, 4,             1,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(41, 4,             4,         1);

/* Constructor */
INSERT OR REPLACE INTO ship_classes_actions VALUES(20, 2,             2,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(21, 2,             2,         2);
INSERT OR REPLACE INTO ship_classes_actions VALUES(22, 2,             2,         3);
INSERT OR REPLACE INTO ship_classes_actions VALUES(23, 2,             4,         1);

/* Gatherer */
INSERT OR REPLACE INTO ship_classes_actions VALUES(30, 3,             3,         1);
INSERT OR REPLACE INTO ship_classes_actions VALUES(31, 3,             4,         1);

