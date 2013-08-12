CREATE TABLE users(id INT PRIMARY KEY, login TEXT, password TEXT);

CREATE TABLE objects(
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    type TEXT NOT NULL,
    class INTEGER,
    model TEXT,
    x INT NOT NULL,
    y INT NOT NULL,
    z INT NOT NULL,
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

