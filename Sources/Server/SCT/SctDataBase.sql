INSERT INTO users VALUES(1, "user1", "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8"); /* password */
INSERT INTO users VALUES(2, "user2", "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8");
INSERT INTO users VALUES(3, "user3", "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8");

/*                         id   type        class  model             x     y     z      owner integrity carbon helium */
INSERT INTO objects VALUES(1,   "Ship",     1,     NULL,             1,    1,    1,     1,    100,      10,    10);
INSERT INTO objects VALUES(2,   "Ship",     2,     NULL,             1,    1,    1,     2,    100,      11,     6);
INSERT INTO objects VALUES(4,   "Ship",     1,     NULL,             1,    1,    1,     2,    100,      0,     0);
INSERT INTO objects VALUES(5,   "Ship",     1,     NULL,             10000,1,    1,     2,    100,      0,     0);

INSERT INTO objects VALUES(3,   "Asteroid", 1,     "Asteroid.model", 100,  100,  100,   NULL, 100,      100,   100);

/*                              id   speed   max_integrity  name             model                 max_capacity  required_carbon  required_helium */
INSERT INTO ship_classes VALUES(1,   1,      100,           "Ship1",         "Ship1.model",        1000,         10,              5);
INSERT INTO ship_classes VALUES(2,   20,     200,           "Ship2",         "Ship2.model",        1000,         10,              5);

/*                                      id  ship_class_id  action_id  action_parameter */
/* Ship1 */
INSERT INTO ship_classes_actions VALUES(1,  1,             1,         1);
INSERT INTO ship_classes_actions VALUES(2,  1,             1,         2);
INSERT INTO ship_classes_actions VALUES(3,  1,             2,         1);
INSERT INTO ship_classes_actions VALUES(4,  1,             3,         1);
INSERT INTO ship_classes_actions VALUES(5,  1,             4,         1);

