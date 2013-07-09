INSERT INTO users VALUES(1, "user1", "passwordhash");
INSERT INTO users VALUES(2, "user2", "passwordhash");

/*                         id   type        class  x     y     z      owner integrity carbon helium */
INSERT INTO objects VALUES(1,   "Ship",     1,     1,    2000, 1,     1,    100,      0,     0);
INSERT INTO objects VALUES(2,   "Ship",     2,     1,    4000, 1,     1,    100,      0,     0);
INSERT INTO objects VALUES(3,   "Ship",     3,     1,    5000, 1,     1,    100,      0,     0);
INSERT INTO objects VALUES(4,   "Ship",     4,     1,    6000, 1,     1,    100,      0,     0);
INSERT INTO objects VALUES(5,   "Ship",     5,     1,    7000, 1,     1,    100,      0,     0);

INSERT INTO objects VALUES(11,  "Ship",     1,     1,    2000, 10000, 2,    100,      0,     0);
INSERT INTO objects VALUES(12,  "Ship",     2,     1,    4000, 10000, 2,    100,      0,     0);
INSERT INTO objects VALUES(13,  "Ship",     3,     1,    5000, 10000, 2,    100,      0,     0);
INSERT INTO objects VALUES(14,  "Ship",     4,     1,    6000, 10000, 2,    100,      0,     0);
INSERT INTO objects VALUES(15,  "Ship",     5,     1,    7000, 10000, 2,    100,      0,     0);

INSERT INTO objects VALUES(100, "Asteroid", 1,     2000, 1,    1,     NULL, 100,      1000,  1000);
INSERT INTO objects VALUES(101, "Asteroid", 1,     2000, 1000, 1,     NULL, 100,      1000,  1000);

/*                              id   speed   max_integrity   model                 max_capacity  required_carbon  required_helium */
INSERT INTO ship_classes VALUES(1,   500,    1000,           "Fighter.model",      100,          10,              5);
INSERT INTO ship_classes VALUES(5,   5000,   100,            "Carnwennan.model",   100,          10,              5);
INSERT INTO ship_classes VALUES(4,   80,     10000,          "Fregate.model",      1000,         1000,            50);
INSERT INTO ship_classes VALUES(2,   100,    1000,           "Constructor.model",  10000,        1000,            50);
INSERT INTO ship_classes VALUES(3,   100,    1000,           "Gatherer.model",     1000,         100,             50);

/*                                      id  ship_class_id  action_id  action_parameter */
/* Fighter */
INSERT INTO ship_classes_actions VALUES(1,  1,             1,         1);
INSERT INTO ship_classes_actions VALUES(2,  1,             1,         2);
INSERT INTO ship_classes_actions VALUES(3,  1,             4,         1);

/* Carnwennan */
INSERT INTO ship_classes_actions VALUES(50, 5,             1,         1);
INSERT INTO ship_classes_actions VALUES(51, 5,             4,         1);

/* Fregate */
INSERT INTO ship_classes_actions VALUES(40, 4,             1,         1);
INSERT INTO ship_classes_actions VALUES(41, 4,             4,         1);

/* Constructor */
INSERT INTO ship_classes_actions VALUES(20, 2,             2,         1);
INSERT INTO ship_classes_actions VALUES(21, 2,             2,         2);
INSERT INTO ship_classes_actions VALUES(22, 2,             2,         3);
INSERT INTO ship_classes_actions VALUES(23, 2,             4,         1);

/* Gatherer */
INSERT INTO ship_classes_actions VALUES(30, 3,             3,         1);
INSERT INTO ship_classes_actions VALUES(31, 3,             4,         1);

