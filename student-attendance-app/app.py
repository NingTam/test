
from flask import Flask, render_template, jsonify, request
import mysql.connector
from config import DB_CONFIG

app = Flask(__name__)

def get_connection():
    return mysql.connector.connect(**DB_CONFIG)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/students')
def students():
    conn = get_connection()
    cursor = conn.cursor(dictionary=True)
    cursor.execute("SELECT * FROM students")
    data = cursor.fetchall()
    cursor.close()
    conn.close()
    return jsonify(data)

@app.route('/api/mark', methods=['POST'])
def mark():
    data = request.json
    roll = data['roll']
    status = data['status']

    conn = get_connection()
    cursor = conn.cursor()
    cursor.execute("UPDATE students SET status = %s WHERE roll = %s", (status, roll))
    conn.commit()
    cursor.close()
    conn.close()
    return jsonify({"success": True})

if __name__ == '__main__':
    app.run(debug=True)
