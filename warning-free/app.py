import mysql.connector
from flask import Flask, render_template

def connect_to_database():
    return mysql.connector.connect(
        host="127.0.0.1",
        user="flask_user",
        password="flask_password123",
        database="company"
    )

app = Flask(__name__, template_folder='templates')

@app.route('/')
def index():
    try:
        conn = connect_to_database()
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM employees")
        table_data = cursor.fetchall()
        
        cursor = conn.cursor(dictionary=True)
        cursor.execute("SELECT name, salary FROM employees")
        salaries = cursor.fetchall()
        
        cursor.close()
        conn.close()
        return render_template('index.html', table_data=table_data, salaries=salaries)
    except Exception as e:
        return f"Database error: {str(e)}"

@app.route('/table')
def table():
    try:
        conn = connect_to_database()
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM employees")
        data = cursor.fetchall()
        cursor.close()
        conn.close()
        return render_template('table.html', table_data=data)
    except Exception as e:
        return f"Database error: {str(e)}"
@app.route('/salary_graph')
def salary_graph():
    try:
        conn = connect_to_database()
        cursor = conn.cursor(dictionary=True)
        cursor.execute("SELECT name, salary FROM employees")
        salaries = cursor.fetchall()
        cursor.close()
        conn.close()
        return render_template('salary_graph.html', salaries=salaries)
    except Exception as e:
        return f"Database error: {str(e)}"
if __name__ == '__main__':
    app.run(debug=True, port=8000, host='0.0.0.0')
@app.route('/databases')
def list_databases():
    try:
        conn = connect_to_database()
        cursor = conn.cursor()
        cursor.execute("SHOW DATABASES")
        databases = cursor.fetchall()
        cursor.close()
        conn.close()
        return f"Databases: {databases}"
    except Exception as e:
        return f"Database error: {str(e)}"