import sqlite3
from flask import Flask, render_template

def get_db_connection():
    conn = sqlite3.connect("database.db")
    conn.row_factory = sqlite3.Row
    return conn

app = Flask(__name__)

@app.route("/")
def index():
    conn = get_db_connection()
    users = conn.execute("SELECT * FROM users").fetchall()
    conn.close()
    return render_template("index.html", users=users)

@app.route("/page1")
def page1():
    return render_template("page1.html")


@app.route("/page2")
def page2():
    conn = get_db_connection()
    users = conn.execute("SELECT * FROM users").fetchall()
    conn.close()

    rows = ""
    for user in users:
        rows += f"""
            <tr>
                <h1>User id: {user['id']}</h1> <br>
                <h1><strong>{user['name']}</strong></h1> <br>
                <h2>{user['age']}</span></h2><br>
                <h2>{user['created']}</h2><br><br>
            </tr>
        """

    return f"<p>{rows}</p>"

if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0")
