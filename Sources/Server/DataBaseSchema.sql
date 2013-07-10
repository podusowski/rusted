CREATE TABLE users(id INT PRIMARY KEY, login TEXT, password TEXT);

CREATE TABLE objects(
    id INT PRIMARY KEY,
    type TEXT,
    class INT,
    model TEXT,
    x INT,
    y INT,
    z INT,
    owner INT,
    integrity INT,
    carbon INT,
    helium INT
);

CREATE TABLE ship_classes(
    id INT PRIMARY KEY,
    speed INT,
    max_integrity INT,
    name TEXT,
    model TEXT,
    max_capacity INT,
    required_carbon INT,
    required_helium INT
);

CREATE TABLE ship_classes_actions(
    id INT PRIMARY KEY,
    ship_class_id INT,
    action_id INT,
    action_parameter INT
);

