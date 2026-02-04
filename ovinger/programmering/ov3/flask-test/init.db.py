import sqlite3

connection = sqlite3.connect("database.db")

with open("schema.sql") as f:
    connection.executescript(f.read())

cur = connection.cursor()

cur.execute("INSERT INTO users (name, age) VALUES(?,?)", 
            ("Tri", 21)
            )

cur.execute("INSERT INTO users (name, age) VALUES(?,?)",
            ("Person", 22)
            ) 

connection.commit()
connection.close()
